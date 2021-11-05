//--------------------------------------------------
// Atta Algorithms - Linear Algebra
// SVD.cpp
// Date: 2021-04-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/linearAlgebra/SVD/SVD.h>
#include <limits>

namespace atta::linalg
{
    inline float SIGN(const float &a, const float &b)
    {
        return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
    }

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
        bool flag;
        int i,its,j,jj,k,l,nm;
        float anorm,c,f,g,h,s,scale,x,y,z;
        g = scale = anorm = 0.0f;
        vec rv1(n);

        // Householder reduction to bidiagonal form
        for(i=0;i<n;i++)
        {
            l = i+2;
            rv1[i] = scale*g;
            g = s = scale = 0.0f;
            if(i<m)
            {
                for(k=i;k<m;k++) scale += std::abs(U[k][i]);
                if(scale != 0.0f)
                {
                    for(k=i;k<m;k++)
                    {
                        U[k][i] /= scale;
                        s += U[k][i]*U[k][i];
                    }
                    f = U[i][i];
                    g = -SIGN(sqrt(s), f);
                    h=f*g-s;
                    U[i][i] = f-g;
                    for(j=l-1;j<n;j++)
                    {
                        s = 0.0f;
                        for(k=i;k<m;k++) s+= U[k][i]*U[k][j];
                        f = s/h;
                        for(k=i;k<m;k++) U[k][j] += f*U[k][i];
                    }
                    for(k=i; k<m; k++) U[k][i]*=scale;
                }
            }
            S[i] = scale*g;
            g=s=scale=0.0f;
            if(i+1<= m && i+1!= n)
            {
                for(k=l-1;k<n;k++) scale += std::abs(U[i][k]);
                if(scale != 0.0)
                {
                    for(k=l-1;k<n;k++)
                    {
                        U[i][k]/= scale;
                        s += U[i][k]*U[i][k];
                    }
                    f=U[i][l-1];
                    g = -SIGN(sqrt(s),f);
                    h = f*g-s;
                    U[i][l-1]=f-g;
                    for(k=l-1;k<n;k++) rv1[k]=U[i][k]/h;
                    for(j=l-1;j<m;j++)
                    {
                        s = 0.0;
                        for(k=l-1;k<n;k++) s += U[j][k]*U[i][k];
                        for(k=l-1;k<n;k++) U[j][k] += s*rv1[k];
                    }
                    for(k=l-1;k<n;k++) U[i][k] *= scale;
                }
            }
            anorm=std::max(anorm,(std::abs(S[i])+std::abs(rv1[i])));
        }

        // Accumulation of right-hand transformations
        for(i=n-1;i>=0;i--)
        {
            if(i < n-1)
            {
                if(g != 0.0)
                {
                    for(j=l;j<n;j++)
                        V[j][i]=(U[i][j]/U[i][l])/g;
                    for(j=l;j<n;j++)
                    {
                        s = 0.0;
                        for(k=l;k<n;k++) s += U[i][k]*V[k][j];
                        for(k=l;k<n;k++) V[k][j]+= s*V[k][i];
                    }
                }
                for(j=l;j<n;j++) V[i][j] = V[j][i] = 0.0;
            }
            V[i][i] = 1.0;
            g = rv1[i];
            l = i;
        }

        // Accumulation of left-hand transformations.
        for(i=std::min(m,n)-1;i>=0;i--)
        {
            l = i+1;
            g = S[i];
            for(j=l;j<n;j++) U[i][j]=0.0;
            if(g != 0.0)
            {
                g = 1.0/g;
                for(j=l;j<n;j++)
                {
                    s = 0.0;
                    for(k=l;k<m;k++) s += U[k][i]*U[k][j];
                    f = (s/U[i][i])*g;
                    for(k=i;k<m;k++) U[k][j]+= f*U[k][i];
                }
                for(j=i;j<m;j++) U[j][i]*= g;
            } else for (j=i;j<m;j++) U[j][i]=0.0;
            ++U[i][i];
        }

        // Diagonalization of the bidiagonal form: Loop over singular values, and over allowed iterations
        for(k=n-1;k>=0;k--)
        {
            for(its=0;its<30;its++)
            {
                flag=true;
                for(l=k;l>=0;l--)
                {
                    nm=l-1;
                    if(l == 0 || std::abs(rv1[l])<= eps*anorm)
                    {
                        flag = false;
                        break;
                    }
                    if(std::abs(S[nm])<= eps*anorm) break;
                }
                if(flag)
                {
                    c = 0.0;
                    s = 1.0;
                    for(i=l;i<k+1;i++)
                    {
                        f = s*rv1[i];
                        rv1[i] = c*rv1[i];
                        if(std::abs(f)<= eps*anorm) break;
                        g = S[i];
                        h = pythag(f,g);
                        S[i] = h;
                        h = 1.0/h;
                        c = g*h;
                        s = -f*h;
                        for(j=0;j<m;j++)
                        {
                            y = U[j][nm];
                            z = U[j][i];
                            U[j][nm] = y*c+z*s;
                            U[j][i] = z*c-y*s;
                        }
                    }
                }
                // Convergence
                z = S[k];
                if(l == k)
                {
                    if(z < 0.0)
                    {
                        S[k]= -z;
                        for(j=0;j<n;j++) V[j][k]= -V[j][k];
                    }
                    break;
                }
                // No convergence
                if(its == 29)
                {
                    Log::warning("SVD", "No convergence to diagonal form in 30 iterations");
                    return;
                }
                x = S[l];
                nm = k-1;
                y = S[nm];
                g = rv1[nm];
                h = rv1[k];
                f = ((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
                g = pythag(f,1.0);
                f = ((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
                c = s = 1.0;
                for(j=l;j<=nm;j++)
                {
                    i = j+1;
                    g = rv1[i];
                    y = S[i];
                    h = s*g;
                    g = c*g;
                    z = pythag(f,h);
                    rv1[j] = z;
                    c = f/z;
                    s = h/z;
                    f = x*c+g*s;
                    g = g*c-x*s;
                    h = y*s;
                    y *= c;
                    for(jj=0;jj<n;jj++)
                    {
                        x = V[jj][j];
                        z = V[jj][i];
                        V[jj][j] = x*c+z*s;
                        V[jj][i] = z*c-x*s;
                    }
                    z = pythag(f,h);
                    S[j] = z;
                    if(z)
                    {
                        z = 1.0/z;
                        c = f*z;
                        s = h*z;
                    }
                    f = c*g+s*y;
                    x = c*y-s*g;
                    for(jj=0;jj<m;jj++)
                    {
                        y = U[jj][j];
                        z = U[jj][i];
                        U[jj][j] = y*c+z*s;
                        U[jj][i] = z*c-y*s;
                    }
                }
                rv1[l] = 0.0;
                rv1[k] = f;
                S[k] = x;
            }
        }
    }

    void SVD::reorder()
    {
        // Reference: Numerical Recipes: The Art of Scientific Computing - Third Edition - Webnote No.2, Rev. 1
        // Sort the result of decompose with Shell's sort. U and V columns are ordered by decreasing magnitude 
        // and the sign are flipped to maximuze the number of positive elements
        int i,j,k,s,inc=1;
        float sw;
        vec su(m);
        vec sv(n);
        // Sort
        do
        {
            inc *= 3;
            inc++;
        } 
        while(inc<=n);
        do
        {
            inc /= 3;
            for(i=inc;i<n;i++)
            {
                sw = S[i];
                for(k=0;k<m;k++)
                    su[k] = U[k][i];
                for(k=0;k<n;k++) sv[k] = V[k][i];
                j = i;
                while(S[j-inc]< sw)
                {
                    S[j] = S[j-inc];
                    for(k=0;k<m;k++) U[k][j] = U[k][j-inc];
                    for(k=0;k<n;k++) V[k][j] = V[k][j-inc];
                    j -= inc;
                    if(j<inc) break;
                }
                S[j] = sw;
                for(k=0;k<m;k++) U[k][j] = su[k];
                for(k=0;k<n;k++) V[k][j] = sv[k];
            }
        }
        while(inc>1);
        // Flip signs
        for(k=0;k<n;k++)
        {
            s=0;
            for(i=0;i<m;i++)
                if(U[i][k]< 0.) s++;
            for(j=0;j<n;j++)
                if(V[j][k]< 0.) s++;
            if(s>(m+n)/2)
            {
                for(i=0;i<m;i++) U[i][k] = -U[i][k];
                for(j=0;j<n;j++) V[j][k] = -V[j][k];
            }
        }
    }

    double SVD::pythag(const double a, const double b)
    {
        // Computes (a^2+b^2)^(1/2) without destructive underflow or overflow.
        double absa = std::abs(a), absb=std::abs(b);
        return (absa>absb ? 
                absa*sqrt(1.0+(absb/absa)*(absb/absa)) : 
                (absb==0.0 ? 0.0 : absb*sqrt(1.0+(absa/absb)*(absa/absb))));
    }

    unsigned SVD::rank(float thresh)
    {
        unsigned nr = 0;
        tsh = thresh>0.0f ? thresh : 0.5f*sqrt(m+n+1.0f)*S[0]*eps;
        for(int i=0;i<n;i++) if(S[i]>tsh)nr++;
        return nr;
    }

    unsigned SVD::nullity(float thresh)
    {
        unsigned nr = 0;
        tsh = thresh>0.0f ? thresh : 0.5f*sqrt(m+n+1.0f)*S[0]*eps;
        for(int i=0;i<n;i++) if(S[i]<=tsh)nr++;
        return nr;
    }

    mat SVD::colSpace(float thresh)
    {
        int nr = 0;
        mat colSp(m,rank(thresh));
        for(int i=0;i<n;i++) if(S[i]>tsh)
        {
            for(int j=0;j<m;j++) 
                colSp[j][nr] = U[j][i];
            nr++;
        }
        return colSp;
    }

    mat SVD::nullSpace(float thresh)
    {
        int nn = 0;
        mat nullSp(m,nullity(thresh));
        for(int i=0;i<n;i++) if(S[i]<=tsh)
        {
            for(int j=0;j<n;j++) 
                nullSp[j][nn] = V[j][i];
            nn++;
        }
        return nullSp;
    }
}
