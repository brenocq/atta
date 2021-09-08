//--------------------------------------------------
// Atta Algorithms - Linear Algebra
// QR.cpp
// Date: 2021-04-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/linearAlgebra/QR/QR.h>
#include <limits>

namespace atta::linalg
{
	inline float SIGN(const float &a, const float &b)
	{
		return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
	}

	QR::QR(const mat& A):
		m(A.nrows), n(A.ncols), Qt(m,m), R(A), sing(false)
	{
		// Reference: Numerical Recipes: The Art of Scientific Computing - Third Edition - Cap 2.10
		vec c(n);
		vec d(n);
		//---------- Holseholder reflections ----------//
		// For each column
		for(size_t k=0;k<n;k++)
		{
			float scale = 0.0f;
			// Find maximum value from diagonal to bottom
			for(size_t i=k;i<m;i++) scale = std::max(scale, std::abs(R[i][k]));
			if(scale == 0.0f)
			{
				sing = true;
				c[k] = d[k] = 0.0f;
			}
			else
			{
			 	// Divide values below diagonal by highest value (all values between 0 and 1)
				for(size_t i=k;i<m;i++) R[i][k]/=scale;
				float sum = 0.0f;
				// Calculate vector length
				for(size_t i=k;i<m;i++) sum += R[i][k]*R[i][k];
				float beta = SIGN(sqrt(sum), R[k][k]);// Choose to sum or subtract if first value >0 or <0 (good to avoid catastrofic cancelation)
				// v_k = R[k:m,k] (householder vector)
				R[k][k] += beta;// Add +-||x|| to first coord
				c[k] = beta*R[k][k];
				d[k] = beta*(-scale);
				// Reflect remaining vectors
				for(size_t j=k+1; j<n; j++)
				{
					// Get projection of this column in v_k
					sum = 0.0f;
					for(size_t i=k;i<m;i++) sum += R[i][k]*R[i][j];
					float tau = sum/c[k];
					// Reflect
					for(size_t i=k;i<m;i++) R[i][j] -= tau*R[i][k];
				}
			}
		}
		//---------- Form Q^T explicitly ----------//
		//d[n-1] = R[n-1][n-1];
		if(d[n-1]==0.0f) sing = true;
		// Set Q^T as identity
		for(size_t i=0;i<m;i++)
		{
			for(size_t j=0;j<m;j++) Qt[i][j] = 0.0;
			Qt[i][i]=1.0f;
		}
		// Multiply identity by P_0, P_1...P_(n-1)
		for(size_t k=0;k<n;k++)
		{
			if(c[k]!=0.0f)
			{
				for(size_t j=0; j<m; j++)
				{
					// Distance Qt column to plane v_i
					float sum = 0.0f;
					for(size_t i=k;i<m;i++)
						sum += R[i][k]*Qt[i][j];
					// Adjust distance with plane vector length
					sum /= c[k];
					// Reflect orthonormal base vector to new orientation
					for(size_t i=k;i<m;i++)
						Qt[i][j] -= sum*R[i][k];
				}
			}
		}
		//---------- Form R explicitly ----------//
		for(size_t i=0;i<n;i++)
		{
			R[i][i] = d[i];
			for(size_t j=0; j<i; j++) R[i][j] = 0.0f;
			for(size_t j=i+1; j<m; j++) R[j][i] = 0.0f;
		}
	}

	void QR::solve(const vec& b, vec& x)
	{
		// Rx = Q^Tb
		// y = Q^Tb
		vec qtb = Qt*b;// Storing in x to use less memory
		// Solve Rx = y
		rsolve(qtb, x);
	}

	void QR::rsolve(vec& b, vec& x)
	{
		if(sing) Log::warning("QR", "Attempting solve in a singular QR");
		// Solve each equation
		float sum = 0.0f;
		for(int i=n-1; i>=0; i--)
		{
			sum = b[i];
			for(int j=i+1;j<m;j++) sum -= R[i][j]*x[j];
			x[i]=sum/R[i][i];
		}
	}
}
