//--------------------------------------------------
// Atta Math
// matrix.cpp
// Date: 2021-04-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/math/matrix.h>

namespace atta
{

	template <typename T>
	matrix<T>::matrix(size_t _m, size_t _n):
		m(_m), n(_n)
	{
		rows = std::vector<vector<T>>(m);
		for(int i=0;i<m;i++)
			rows[i] = vector<T>(n);
	}

	template <typename T>
	matrix<T>::matrix(size_t _m, size_t _n, T val):
		m(_m), n(_n)
	{
		rows = std::vector<atta::vector<T>>(m);
		for(int i=0;i<m;i++)
			rows[i] = atta::vector<T>(n, val);
	}

	template <typename T>
	matrix<T>::~matrix()
	{
	}

	template <typename T>
	vector<T>& matrix<T>::operator[](size_t i)
	{
		return rows[i];
	}

	template <typename T>
	template <typename U>
	matrix<T> matrix<T>::operator+(const matrix<U>& o) const
	{
		matrix<T> res = *this;
		for(size_t i=0; i<m; i++)
			res.rows[i]+=o.rows[i];
		return res;
	}

	template <typename T>
	template <typename U>
	void matrix<T>::operator+=(const matrix<U>& o)
	{
		for(size_t i=0; i<m; i++)
			rows[i]+=o.rows[i];
	}

	template <typename T>
	template <typename U>
	matrix<T> matrix<T>::operator-(const matrix<U>& o) const
	{
		matrix<T> res = *this;
		for(size_t i=0; i<m; i++)
			res.rows[i]-=o.rows[i];
		return res;
	}

	template <typename T>
	template <typename U>
	void matrix<T>::operator-=(const matrix<U>& o)
	{
		for(size_t i=0; i<m; i++)
			rows[i]-=o.rows[i];
	}

	template <typename T>
	template <typename U>
	matrix<T> matrix<T>::operator*(const matrix<U>& o)
	{
		matrix<T> res = matrix<T>(m, o.n);
		
		size_t i, j, k;
		for(i=0; i<res.m; i++)
		{
			for(j=0; j<res.n; j++)
			{
				res[i][j] = 0;
				for(k=0; k<n; k++)
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

		m = res.m;
		n = res.n;
		rows = res.rows;
	}

	template <typename T>
	matrix<T>& matrix<T>::transpose()
	{
		std::swap(m, n);

		std::vector<vector<T>> cols = std::vector<vector<T>>(m);

		for(int i=0;i<m;i++)
		{
			cols[i] = vector<T>(n);
			for(int j=0;j<n;j++)
				cols[i][j] = rows[j][i];
		}

		rows = std::move(cols);	

		return *this;
	}

	template <typename T>
	std::string matrix<T>::toString()
	{
		std::string res = "\n[";

		for(size_t i=0; i<m; i++)
		{
			res+="[";
			for(size_t j=0; j<n; j++)
				res += std::to_string(rows[i][j]) + (j!=n-1 ? ", " : "]");
			res += i!=m-1 ? ",\n" : "]";
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
