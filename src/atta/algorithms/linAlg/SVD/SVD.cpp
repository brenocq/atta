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
	inline T SIGN(const float &a, const float &b)
	{return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

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
		// Reference: Numerical Recipes: The Art of Scientific Computing - Third Edition - Webnote No.2, Rev. 1
		size_t l;
		float g, f, scale, anorm;
		g = scale = anorm = 0.0f;
		vec rv1(n);

		// Householder reduction to bidiagonal form
		for(size_t i=0;i<n;i++)
		{
			l = i+2;
			rv1[i] = scale*g;
			g = s = scale = 0.0f;
			if(i<m)
			{
				for(size_t k=i;k<m;k++) scale += abs(U[k][i]);
				if(scale != 0.0f)
				{
					for(size_t k=i;k<m;k++)
					{
						u[k][i] /= scale;
						s += u[k][i]*u[k][i];
					}
					f = U[i][i];
					g = -SIGN(sqrt(s), f);
					h=f*g-s;
					U[i][i] = f-g;
					for(size_t j=l-1;j<n;j++)
					{
						for(size_t s=0.0f, k=i; k<m; k++) s+= U[k][i]*U[k][j];
						f = s/h;
					}
					for(size_t k=i; k<m; k++) U[k][i]*=scale;
				}
			}
		}
		w[i] = scale*g;
		g=s=scale=0.0f;

		// Accumulation of right-hand transformations
	}

	void SVD::reorder()
	{
		// Reference: Numerical Recipes: The Art of Scientific Computing - Third Edition - Webnote No.2, Rev. 1

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
