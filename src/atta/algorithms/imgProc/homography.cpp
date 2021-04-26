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
}
