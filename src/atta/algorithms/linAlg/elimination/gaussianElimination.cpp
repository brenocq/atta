//--------------------------------------------------
// Atta Algorithms - Linear Algebra
// gaussianElimination.cpp
// Date: 2021-04-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/linAlg/elimination/gaussianElimination.h>
#include <atta/helpers/log.h>

namespace atta::linalg
{
	void gaussianElimination(mat& A)
	{
		// Reference: Numerical Recipes: The Art of Scientific Computing - Third Edition - Cap 2.2
		// Info:
		// 	Performing row operations until we get A in the row echelon form:
		// 		R2R1R0A
		// Input:
		// A -> n by n matrix
		// Output:
		// A <- A is replaced by the gaussian elimination result

		size_t n = A.ncols;
		// Vectors used for bookkeeping on the pivoting
		veci ipiv(n, 0);// 1 if column has already been used as pivot, 0 if not

		int irow, icol;
		for(size_t i=0;i<n;i++)// Main loop to reduce (i isnt the current col being reduced, but the pivot index)
		{
			// Find pivot (highest value)
			float big = 0.0f;
			for(size_t j=0;j<n;j++) if(ipiv[j]!=1)// Ignores lines with pivot
				if(ipiv[i]==0 && abs(A[j][i])>=big)// If pivot in this column was never used
				{
					big = abs(A[j][i]);
					irow = j;
					icol = i;
				}
			ipiv[icol]++;

			// Interchange rows if necessary (Sending pivot to diagonal)
			if(irow != icol)
			{
				// Reordering equations
				for(size_t c=0;c<n;c++) std::swap(A[irow][c], A[icol][c]);
			}

			if(A[icol][icol]==0.0f)
			{
				Log::warning("GaussianElimination", "Pivot=0, singular matrix");
				return;
			}

			float pivinv = 1.0f/A[icol][icol];
			// Divide row by pivot
			for(size_t c=0;c<n;c++) A[icol][c]*=pivinv;
			// Reduce rows
			for(size_t r=icol+1;r<n;r++)
			{
				float dum = A[r][icol];
				for(size_t c=0;c<n;c++) A[r][c] -= A[icol][c]*dum;
			}
		}
	}
}
