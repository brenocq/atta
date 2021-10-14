//--------------------------------------------------
// Atta Algorithms - Linear Algebra
// LU.cpp
// Date: 2021-04-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/linearAlgebra/LU/LU.h>

namespace atta::linalg
{
    LU::LU(mat& A):
        n(A.nrows), lu(A), Aref(A), d(1.0f)
    {
        // Reference: Numerical Recipes: The Art of Scientific Computing - Third Edition - Cap 2.3
        const float TINY = 1.0e-10;
        vec rs(n);// Implicit scaling of each row
        indx = vec(n);

        float big, temp;
        size_t imax;

        // Find implicit scaling of each row
        for(size_t r = 0; r < n; r++)
        {
            big = 0.0f;
            for(size_t c = 0; c < n; c++)
                if((temp = std::abs(lu[r][c])) > big) big=temp;
            if(big == 0.0f)
            {
                LOG_WARN("LU", "Trying to perform LU on a singular matrix: $0", A);
                return;
            }
            rs[r] = 1.0f/big;
        }

        // kij loop
        for(size_t k = 0;k < n; k++)
        {
            big = 0.0f;
            // Find largest pivot inside column k
            for(size_t r = k; r < n; r++)
            {
                temp = rs[r]*std::abs(lu[r][k]);
                if(temp > big)
                {
                    big = temp;
                    imax = r;
                }
            }

            // Interchange rows if necessary (moving pivot to diagonal)
            if(k != imax)
            {
                for(size_t c = 0; c < n; c++)
                    std::swap(lu[imax][c], lu[k][c]);
                d = -d;// Change parity
                rs[imax] = rs[k];// Change implicit scale factor for row imax
            }
            indx[k] = imax;

            if(lu[k][k] == 0) lu[k][k] = TINY;

            // For each row > k (lower triangular)
            for(size_t i = k+1; i < n; i++)
            {
                // αik = aik/βkk
                temp = (lu[i][k] /= lu[k][k]);// Divide by pivot element (lower triangular)
                // Reduce column > k
                // βij -= Σαik*βkj
                for(size_t j = k+1; j < n; j++)
                    lu[i][j] -= temp*lu[k][j];
            }
        }
    }
}
