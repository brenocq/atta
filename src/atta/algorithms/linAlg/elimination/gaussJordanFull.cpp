//--------------------------------------------------
// Atta Algorithms - Linear Algebra
// gaussJordanFull.cpp
// Date: 2021-04-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/linAlg/elimination/gaussJordanFull.h>
#include <atta/helpers/log.h>

namespace atta::linalg
{
	void gaussJordanFull(mat& A, mat& b)
	{
		// Reference: Numerical Recipes: The Art of Scientific Computing - Third Edition - Cap 2.1
		// Info:
		// 	Solving Ax = b with x and b being matrices (A1x1 = b1, A2x2 = b2, A3x3 = b3, ...)
		// 	Performing row and column operations:
		// 		(R2R1R0A)x = R2R1R0b -> Ix = R2R1R0b
		// 		(AC2C1C0)Ci2Ci1Ci0x = b -> Ix = C0C1C2b
		// 		So, we perform row and column operations on A until A turns to I
		// 		When we perform row operations on A, we perform the same row operations no I
		// 		When we perform column operations on A, we perform the same column operations no I *in reverse order*
		// Input:
		// A -> n by n matrix
		// b -> n by m equations to solve
		// Output:
		// A <- A is replaced by A inverse
		// b <- b is replaces by x solutions
		size_t n = A.ncols;
		size_t m = b.ncols;
		// Vectors used for bookkeeping on the pivoting
		veci indxc(n);// Original pivoti column (used to perform the column operations at the end)
		veci indxr(n);// Original pivoti row (used to perform the column operations at the end)
		veci ipiv(n, 0);// 1 if column has already been used as pivot, 0 if not

		int irow, icol;
		for(size_t i=0;i<n;i++)// Main loop to reduce (i isnt the current col being reduced, but the pivot index)
		{
			// Find pivot (highest value)
			float big = 0.0f;
			for(size_t j=0;j<n;j++) if(ipiv[j]!=1)// Ignores lines with pivot
				for(size_t k=0;k<n;k++)
					if(ipiv[k]==0 && abs(A[j][k])>=big)// If pivot in this column was never used
					{
						big = abs(A[j][k]);
						irow = j;
						icol = k;
					}
			ipiv[icol]++;

			// Interchange rows if necessary (Sending pivot to diagonal)
			if(irow != icol)
			{
				// Reordering equations
				for(size_t c=0;c<n;c++) std::swap(A[irow][c], A[icol][c]);
				for(size_t c=0;c<m;c++) std::swap(b[irow][c], b[icol][c]);
			}

			indxr[i]=irow;
			indxc[i]=icol;

			if(A[icol][icol]==0.0f)
			{
				Log::warning("GaussJordanFull", "Pivot=0, singular matrix");
				return;
			}

			float pivinv = 1.0f/A[icol][icol];
			// Divide row by pivot
			A[icol][icol] = 1.0f;// Setting diagonal as 1 (changing A to inverse)
			for(size_t c=0;c<n;c++) A[icol][c]*=pivinv;
			for(size_t c=0;c<m;c++) b[icol][c]*=pivinv;
			// Reduce rows
			for(size_t r=0;r<n;r++)
				if(r!=icol)
				{
					float dum = A[r][icol];
					A[r][icol] = 0.0f;// Setting to zero becouse started with indentity matrix (changing A to inverse)
					for(size_t c=0;c<n;c++) A[r][c] -= A[icol][c]*dum;
					for(size_t c=0;c<m;c++) b[r][c] -= b[icol][c]*dum;
				}
		}
		// Unscramble A column in the reverse order (Changing A to inverse)
		for(int c=n-1;c>=0;c--) if(indxr[c]!=indxc[c])
			for(size_t r=0;r<n;r++)
				std::swap(A[r][indxr[c]], A[r][indxc[c]]);
	}
}
