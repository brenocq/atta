//--------------------------------------------------
// Atta Algorithms - Linear Algebra
// SVD.cpp
// Date: 2021-04-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/linAlg/SVD/SVD.h>
#include <atta/helpers/log.h>
#include <limits>

namespace atta::linalg
{
	SVD::SVD(mat& A):
		m(A.nrows), n(A.ncols), 
		U(A), V(n,n), S(n)
	{
		eps = std::numeric_limits<float>::epsilon();
		decompose();
		reorder();
		tsh = 0.5f*sqrt(m+n+1.0f)*S[0]*eps;
	}

	void SVD::decompose()
	{

	}

	void SVD::reorder()
	{

	}

	unsigned SVD::rank(float thresh)
	{
		unsigned nr;
		tsh = thresh>0.0f ? thresh : 0.5f*sqrt(m+n+1.0f)*S[0]*eps;
		for(size_t i=0;i<n;i++) if(S[i]>tsh)nr++;
		return nr;
	}

	unsigned SVD::nullity(float thresh)
	{
		unsigned nr;
		tsh = thresh>0.0f ? thresh : 0.5f*sqrt(m+n+1.0f)*S[0]*eps;
		for(size_t i=0;i<n;i++) if(S[i]<=tsh)nr++;
		return nr;
	}

	mat SVD::colSpace(float thresh)
	{
		size_t nr;
		mat colSp(m,rank(thresh));
		for(size_t i=0;i<n;i++) if(S[i]>tsh)
		{
			for(size_t j=0;j<m;j++) 
				colSp[j][nr] = U[j][i];
			nr++;
		}
		return colSp;
	}

	mat SVD::nullSpace(float thresh)
	{
		size_t nn;
		mat nullSp(m,nullity(thresh));
		for(size_t i=0;i<n;i++) if(S[i]<=tsh)
		{
			for(size_t j=0;j<n;j++) 
				nullSp[j][nn] = V[j][i];
			nn++;
		}
		return nullSp;
	}
}
