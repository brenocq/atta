//--------------------------------------------------
// Atta Algorithms - Image Processing
// homography.cpp
// Date: 2021-04-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/imgProc/homography.h>
#include <atta/algorithms/linAlg/SVD/SVD.h>
#include <atta/helpers/log.h>

namespace atta::imgproc
{
	Homography::Homography()
	{

	}

	void Homography::solveDLT(std::vector<std::pair<vec2i, vec2i>> points)
	{
		if(points.size()!=4)
		{
			Log::warning("Homography", "Must send 4 points to the DLT(Direct Linear Transformation) solver. H not calculated");
			return;
		}

		//---------- Normalization ----------//
		// We want the new coordinate system origin be the points centroid
		// We want the points in the new coordinate system to have an average distance of sqrt(2) to the origin
		// Applied to each image individually
		mat3 T1;
		mat3 T2;
		for(size_t image=0;image<2;image++)
		{
			// Calculate translation (for each of the 8 points)
			vec2 centroid(0,0);
			for(size_t i=0;i<points.size();i++)
			{
				vec2i point = image==0 ? points[i].first : points[i].second;
				centroid += point;
			}
			centroid/=float(points.size());

			// Calculate scale
			float currScale=0;
			for(size_t i=0;i<points.size();i++)
			{
				vec2 point = image==0 ? points[i].first : points[i].second;
				point -= centroid;
				currScale += sqrt(point.x*point.x+point.y*point.y);
			}
			currScale/=float(points.size());

			// Calculate transform
			mat3& T = image==0?T1:T2;
			// Define scale
			float scale = sqrt(2)/currScale;
			T.mat[0][0] = scale;
			T.mat[1][1] = scale;
			T.mat[2][2] = 1;
			// Define translation
			T.mat[0][2] = -centroid.x*scale;
			T.mat[1][2] = -centroid.y*scale;
		}

		std::vector<std::pair<vec2, vec2>> pointsNorm(4);
		// Transform points
		for(size_t i=0;i<pointsNorm.size();i++)
		{
			pointsNorm[i].first = vec2(T1*vec3(points[i].first, 1));
			pointsNorm[i].second = vec2(T2*vec3(points[i].second, 1));
		}
		//---------- Direct Linear Transformation ----------//
		mat A(8,9);
		vec h(9);
		for(size_t i=0;i<4;i++)
		{
			A[i*2][0] = 0;
			A[i*2][1] = 0;
			A[i*2][2] = 0;
			A[i*2][3] = -pointsNorm[i].first.x;
			A[i*2][4] = -pointsNorm[i].first.y;
			A[i*2][5] = -1;
			A[i*2][6] = pointsNorm[i].second.y*pointsNorm[i].first.x;
			A[i*2][7] = pointsNorm[i].second.y*pointsNorm[i].first.y;
			A[i*2][8] = pointsNorm[i].second.y;

			A[i*2+1][0] = pointsNorm[i].first.x;
			A[i*2+1][1] = pointsNorm[i].first.y;
			A[i*2+1][2] = 1;
			A[i*2+1][3] = 0;
			A[i*2+1][4] = 0;
			A[i*2+1][5] = 0;
			A[i*2+1][6] = -pointsNorm[i].second.x*pointsNorm[i].first.x;
			A[i*2+1][7] = -pointsNorm[i].second.x*pointsNorm[i].first.y;
			A[i*2+1][8] = -pointsNorm[i].second.x;
		}
		// Solving Ah=0
		{
			// Solve svd to find h where Ah~=0
			linalg::SVD svd(A);
			//Log::debug("Homography", "S:\n$0\nV:$1", svd.S.toString(), svd.V.toString());

			float sumSqr=0;
			for(size_t i=0;i<8;i++)
			{
				h[i] = svd.V[i][8];
				H.mat[i/3][i%3] = h[i];
				sumSqr += h[i]*h[i];
			}
			h[8] = sqrt(1-sumSqr);// Find last coordinate with constraint ||h||=1
			H.mat[2][2] = h[8];
			//Log::debug("Homography", "A: $0", A.toString());
			//Log::debug("Homography", "h: $0", h.toString());
			//Log::debug("Homography", "Ah: $0", (A*h).toString());
		}
		//---------- Denormalization ----------//
		H = inverse(T2)*(H*T1);

		// Check points
		//for(auto ppair : points)
		//{
		//	vec3 pp = H*vec3(ppair.first,1);
		//	pp/=pp.z;

		//	Log::debug("Homography", "Hp -- p':\n$0\n$1", pp.toString(), vec3(ppair.second, 1).toString());
		//}
	}

	void Homography::solveRANSAC(std::vector<std::pair<vec2i, vec2i>> points, size_t width, size_t height)
	{
		//---------- Distribute samples in tiles (8x8) ----------//
		size_t tiles1D = 8;
		size_t numTiles = tiles1D*tiles1D;
		std::vector<std::vector<std::pair<vec2i, vec2i>>> tiles(numTiles);
		std::vector<size_t> tileIndexes;// Indexes of tiles with points (to avoid testing empty tiles)
		size_t tileWidth = width/tiles1D;
		size_t tileHeight = height/tiles1D;

		// Add point pairs to tiles
		for(auto& ppair : points)
		{
			size_t tileIndex = ppair.first.x/tileWidth + tiles1D*(ppair.first.y/tileHeight);
			tiles[tileIndex].push_back(ppair);
		}

		// Check which tiles are not empty
		for(size_t i=0;i<tiles.size();i++)
		{
			if(tiles[i].size()>0)
			{
				//Log::debug("Homography", "solveRANSAC $0 -> $1", i, tiles[i].size());
				tileIndexes.push_back(i);
			}
		}

		if(tileIndexes.size()<4)
		{
			//Log::warning("Homography", "Could not distribute points in at least four tiles");
			return;
		}

		//---------- First stage (find homography approximation) ----------//
		mat3 bestH;
		size_t bestNumInliers = 0;

		float p = 0.99;// Probability to choose sample without outliers within the iterations
		float e = 0.5;// Chance of a point being an outlier
		size_t N = ceil(log10(1-p)/log10(1-pow(1-e, 4)));
		for(size_t it=0;it<N;it++)
		{
			// Choose random sample of 4 points
			std::vector<std::pair<vec2i, vec2i>> pointsDLT;
			std::vector<size_t> tilesChosen;
			while(pointsDLT.size()<4)
			{
				// Select tile without repeating
				size_t tileIndex;
				while(true)
				{
					// Checking only tiles with points
					tileIndex = tileIndexes[rand()%tileIndexes.size()];
					bool newTile=true;
					// Try again if tile was already chosen
					for(size_t i=0;i<tilesChosen.size();i++)
					{
						if(tileIndex==tilesChosen[i])
						{
							newTile = false;
							break;
						}
					}
					// If found suitable tile
					if(newTile)
					{
						tilesChosen.push_back(tileIndex);
						break;
					}
				}

				// Select random point from tile
				size_t ppairIndex = rand()%(tiles[tileIndex].size());
				std::pair<vec2i, vec2i> ppair = tiles[tileIndex][ppairIndex];
				pointsDLT.push_back(ppair);
			}

			// Calculate homography with DLT
			solveDLT(pointsDLT);

			// Check number of inliers (symmetric transfer error)
			size_t numInliers = 0;
			mat3 Hinv = inverse(H);
			for(auto ppair : points)
			{
				float error = 0;
				// dist(x, H^-1*x')^2
				{
					vec3 H1xp = Hinv*vec3(ppair.second, 1);
					H1xp/=H1xp.z;
					float dx = H1xp.x-ppair.first.x;
					float dy = H1xp.y-ppair.first.y;
					error += dx*dx+dy*dy;
				}
				// dist(x', H*x)^2
				{
					vec3 Hx = H*vec3(ppair.first, 1);
					Hx/=Hx.z;
					float dx = Hx.x-ppair.second.x;
					float dy = Hx.y-ppair.second.y;
					error += dx*dx+dy*dy;
				}
				if(error<5.99)
					numInliers++;
			}
			if(numInliers>bestNumInliers)
			{
				// Update best H found so far
				bestH = H;
				bestNumInliers = numInliers;

				// Update N with from proportion of outliers
				e = 1-(bestNumInliers/float(points.size()));// Chance of a point being an outlier
				N = std::min(N, (size_t)ceil(log10(1-p)/log10(1-pow(1-e, 4))));
			}
		}

		//Log::debug("Homogrpahy", "N: $0 ($1/$2)", N, bestNumInliers, points.size());
		//---------- Second stage (Refines homography with inliers) ----------//
	
		
	}
}
