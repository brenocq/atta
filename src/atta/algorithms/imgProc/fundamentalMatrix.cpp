//--------------------------------------------------
// Atta Algorithms - Image Processing
// fundamentalMatrix.cpp
// Date: 2021-04-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/imgProc/fundamentalMatrix.h>
#include <atta/algorithms/linAlg/SVD/SVD.h>
#include <atta/helpers/log.h>

namespace atta::imgproc
{
	FundamentalMatrix::FundamentalMatrix()
	{

	}

	void FundamentalMatrix::solve8Points(std::vector<std::pair<vec2i, vec2i>> points)
	{
		if(points.size()!=8)
		{
			Log::warning("FundamentalMatrix", "Must send 8 points to the solve8Points solver. F not calculated");
			return;
		}

		// Reference: Multiple View Geometry in Computer Vision - Second Edition - Part II, Cap 11, Cap 4.4
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

		// Check normalized coords
		//vec2 centroid(0,0);
		//float scale=0;
		//for(size_t i=0;i<points.size();i++)
		//	centroid += vec2(T1*vec3(points[i].first,1.0f));
		//centroid/=points.size();
		//for(size_t i=0;i<points.size();i++)
		//{
		//	vec2 point = vec2(T1*vec3(points[i].first,1.0f));
		//	point -= centroid;
		//	scale += sqrt(point.x*point.x+point.y*point.y);
		//}
		//scale/=points.size();
		//Log::debug("FundamentalMatrix", "Centroid: $0, Scale: $1", centroid.toString(), scale);

		std::vector<std::pair<vec2, vec2>> pointsNorm(8);
		// Transform points
		for(size_t i=0;i<pointsNorm.size();i++)
		{
			pointsNorm[i].first = vec2(T1*vec3(points[i].first, 1));
			pointsNorm[i].second = vec2(T2*vec3(points[i].second, 1));
		}

		//---------- Find solution f from A ----------//
		mat A(8, 9);
		vec f(9);
		for(size_t i=0;i<8;i++)
		{
			A[i][0] = pointsNorm[i].second.x*pointsNorm[i].first.x;
			A[i][1] = pointsNorm[i].second.x*pointsNorm[i].first.y;
			A[i][2] = pointsNorm[i].second.x;
			A[i][3] = pointsNorm[i].second.y*pointsNorm[i].first.x;
			A[i][4] = pointsNorm[i].second.y*pointsNorm[i].first.y;
			A[i][5] = pointsNorm[i].second.y;
			A[i][6] = pointsNorm[i].first.x;
			A[i][7] = pointsNorm[i].first.y;
			A[i][8] = 1;
		}

		mat Fsvd(3,3);
		{
			// Solve svd to find f where Af~=0
			linalg::SVD svd(A);
			//Log::debug("FundamentalMatrix", "S:\n$0\nV:$1", svd.S.toString(), svd.V.toString());

			float sumSqr=0;
			for(size_t i=0;i<8;i++)
			{
				f[i] = svd.V[i][8];
				Fsvd[i/3][i%3] = f[i];
				sumSqr += f[i]*f[i];
			}
			f[8] = sqrt(1-sumSqr);// Find last coordinate with constraint ||f||=1
			Fsvd[2][2] = f[8];
			//Log::debug("FundamentalMatrix", "A: $0", A.toString());
			//Log::debug("FundamentalMatrix", "f: $0", f.toString());
			//Log::debug("FundamentalMatrix", "Af: $0", (A*f).toString());
		}
		//---------- Singularity constraint enforcement ----------//
		{
			//Log::debug("FundamentalMatrix", "Fprev: $0", F.toString());
			linalg::SVD svd(Fsvd);
			//Log::debug("FundamentalMatrix", "----SVD----\nU:$0\nS:\n$1\nV:$2", 
			//	svd.U.toString(), svd.S.toString(), svd.V.toString());

			// Set last eigenvalue to zero
			mat S(3,3);
			S[0][0] = svd.S[0];
			S[1][1] = svd.S[1];

			// Reconstruct F
			mat newF = svd.U*(S*svd.V);
			for(size_t i=0;i<9;i++)
				F.mat[i/3][i%3] = newF[i/3][i%3];
			//Log::debug("FundamentalMatrix", "Fafter $0", F.toString());
		}
		//---------- Denormalization ----------//
		F = transpose(T2)*(F*T1);

		// Test if points lie in the same line
		//Log::debug("FundamentalMatrix", "two points: $0 -> $1", vec3(points[0].first,1).toString(), vec3(points[0].second, 1).toString());
		//Log::debug("FundamentalMatrix", "p'*F*p: $0", vec3(points[0].second,1).dot(F*vec3(points[0].first,1)));
	}

	void solve8PointsRANSAC(std::vector<std::pair<vec2i, vec2i>> points)
	{
		// TODO
	}
}
