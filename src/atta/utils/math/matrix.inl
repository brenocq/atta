//--------------------------------------------------
// Atta Math
// matrix.inl
// Date: 2021-04-09
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    template <typename T>
    matrix<T>::matrix(unsigned _nrows, unsigned _ncols):
        nrows(_nrows), ncols(_ncols)
    {
        rows = std::vector<vector<T>>(nrows);
        for(unsigned i=0;i<nrows;i++)
            rows[i] = vector<T>(ncols);
    }

    template <typename T>
    matrix<T>::matrix(unsigned _nrows, unsigned _ncols, T val):
        nrows(_nrows), ncols(_ncols)
    {
        rows = std::vector<atta::vector<T>>(nrows);
        for(unsigned i=0;i<nrows;i++)
            rows[i] = atta::vector<T>(ncols, val);
    }

    template <typename T>
    template <typename U>
    matrix<T>::matrix(const matrix<U>& m):
        nrows(m.nrows), ncols(m.ncols)
    {
        rows = std::vector<atta::vector<T>>(nrows);
        for(unsigned i=0;i<nrows;i++)
            rows[i] = atta::vector<T>(ncols);

        for(unsigned i=0;i<nrows;i++)
            for(unsigned j=0;j<ncols;j++)
                rows[i][j] = m.rows.at(i).at(j);
    }

    template <typename T>
    matrix<T>::~matrix()
    {
    }

    template <typename T>
    vector<T>& matrix<T>::operator[](unsigned i)
    {
        return rows[i];
    }

    template <typename T>
    template <typename U>
    matrix<T> matrix<T>::operator+(const matrix<U>& o) const
    {
        matrix<T> res = *this;
        for(unsigned i=0; i<nrows; i++)
            res.rows[i]+=o.rows[i];
        return res;
    }

    template <typename T>
    template <typename U>
    void matrix<T>::operator+=(const matrix<U>& o)
    {
        for(unsigned i=0; i<nrows; i++)
            rows[i]+=o.rows[i];
    }

    template <typename T>
    template <typename U>
    matrix<T> matrix<T>::operator-(const matrix<U>& o) const
    {
        matrix<T> res = *this;
        for(unsigned i=0; i<nrows; i++)
            res.rows[i]-=o.rows[i];
        return res;
    }

    template <typename T>
    template <typename U>
    void matrix<T>::operator-=(const matrix<U>& o)
    {
        for(unsigned i=0; i<nrows; i++)
            rows[i]-=o.rows[i];
    }

    template <typename T>
    template <typename U>
    matrix<T> matrix<T>::operator*(const matrix<U>& o)
    {
        matrix<T> res = matrix<T>(nrows, o.ncols);

        unsigned i, j, k;
        for(i=0; i<res.nrows; i++)
        {
            for(j=0; j<res.ncols; j++)
            {
                res[i][j] = 0;
                for(k=0; k<ncols; k++)
                    res[i][j] += rows[i][k] * o.rows.at(k).at(j);
            }
        }

        return res;
    }

    template <typename T>
    template <typename U>
    void matrix<T>::operator*=(const matrix<U>& o)
    {
        matrix<T> res = (*this)*o;

        nrows = res.nrows;
        ncols = res.ncols;
        rows = res.rows;
    }

    template <typename T>
    template <typename U>
    void matrix<T>::operator*=(U v)
    {
        (*this) = (*this)*v;
    }

    template <typename T>
    template <typename U>
    vector<U> matrix<T>::operator*(const vector<U>& v)
    {
        vector<U> res(nrows);
        for(unsigned i=0;i<nrows;i++)
        {
            U sum = 0;
            for(unsigned j=0;j<ncols;j++)
                sum += rows[i][j]*v.at(j);
            res[i] = sum;
        }
        return res;
    }

    template <typename T>
    template <typename U>
    matrix<T> matrix<T>::operator*(U v)
    {
        matrix<T> res = matrix<T>(nrows, ncols);
        for(unsigned i=0;i<res.nrows;i++)
            for(unsigned j=0;j<res.ncols;j++)
                res.rows[i][j] = rows[i][j]*v;

        return res;
    }

    template <typename T>
    matrix<T>& matrix<T>::transpose()
    {
        std::swap(nrows, ncols);

        std::vector<vector<T>> cols = std::vector<vector<T>>(nrows);

        for(unsigned i = 0; i < nrows; i++)
        {
            cols[i] = vector<T>(ncols);
            for(unsigned j = 0; j < ncols; j++)
                cols[i][j] = rows[j][i];
        }

        rows = std::move(cols);	

        return *this;
    }

    template <typename T>
    std::string matrix<T>::toString() const
    {
        std::string res = "\n[";

        for(unsigned i=0; i<nrows; i++)
        {
            res+="[";
            for(unsigned j=0; j<ncols; j++)
                res += std::to_string(rows.at(i).at(j)) + (j!=ncols-1 ? ", " : "]");
            res += i!=nrows-1 ? ",\n" : "]";
        }

        return res;
    }

    //------------------------------------------------------------//
    //-------------------------- Inline --------------------------//
    //------------------------------------------------------------//
    template <typename T>
    inline matrix<T> transpose(const matrix<T>& m)
    {
        matrix<T> t = m;
        t.transpose();
        return t;
    }
}
