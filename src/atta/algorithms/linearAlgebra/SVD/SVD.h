//--------------------------------------------------
// Atta Algorithms - Linear Algebra
// SVD.h
// Date: 2021-04-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/core/math/math.h>

namespace atta::linalg
{
    class SVD
    {
    public:
        int m,n;// Number of rows/columns
        mat U, V;// Matrices U and V
        vec S;// Sigma/Diagonal matrix (eigenvalues)
        float eps, tsh;

        SVD(mat& A);

        unsigned rank(float thresh=-1.0f);
        unsigned nullity(float thresh=-1.0f);
        mat colSpace(float thresh=-1.0f);
        mat nullSpace(float thresh=-1.0f);

    private: 
        void decompose();
        void reorder();
        double pythag(const double a, const double b);
    };
}
