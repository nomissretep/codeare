#ifndef __ALGOS_HPP__
#define __ALGOS_HPP__

/**
 * @brief    Number of non-zero elements
 *
 * @param M  Matrix
 * @return   Number of non-zero elements of matrix M
 */
template <class T> inline size_t 
nnz (const Matrix<T>& M) {
	
	size_t nz   = 0;
	T      zero = T(0);
	
	for (int i = 0; i < M.Size(); i++)
		if (M[i] != T(0))
			nz++;
	
	return nz;
	
}


/**
 * @brief    Is matrix 1D?
 *
 * @param M  Matrix
 * @return   1D?
 */
template <class T>  inline bool 
Is1D (const Matrix<T>& M) {
	
	return IsXD(M, 1);
	
}


/**
 * @brief    Is matrix 2D?
 *
 * @param M  Matrix
 * @return   2D?
 */
template <class T>  inline bool 
Is2D (const Matrix<T>& M) {
	
	return IsXD(M, 2);
	
}


/**
 * @brief    Is matrix 3D?
 *
 * @param M  Matrix
 * @return   3D?
 */
template <class T>  inline bool 
Is3D (const Matrix<T>& M) {
	
	return IsXD(M, 3);
	
}



/**
 * @brief    Is matrix 4D?
 *
 * @param M  Matrix
 * @return   4D?
 */
template <class T>  inline bool 
Is4D (const Matrix<T>& M) {
	
	return IsXD(M, 4);
	
}


/**
 * @brief     Is matrix X-dimensional?
 *
 * @param  M  Matrix
 * @param  d  Dimension
 * @return    X-dimensional?
 */
template <class T>  inline bool 
IsXD (const Matrix<T>& M, const size_t d) {
	
	size_t l = 0;
	
	for (size_t i = 0; i < INVALID_DIM; i++)
		if (M.Dim(i) > 1) l++;
	
	return (l == d);
	
}


/**
 * @brief       All elements zero?
 * 
 * @param  M    Matrix
 * @return      All elements zero?
 */
template <class T>  inline bool 
IsZero (const Matrix<T>& M) {
	
	for (size_t i = 0; i < M.Size(); i++)
		if (M[i] != T(0)) return false;
	
	return true;
	
}


/**
 * @brief       Empty matrix?
 * 
 * @param  M    Matrix
 * @return      Empty?
 */
template <class T>  inline bool
IsEmpty (const Matrix<T>& M) {
	
	return (numel(M) == 0);
	
}


/**
 * @brief       Sum of squares over a dimension
 * 
 * @param  M    Matrix
 * @param  d    Dimension
 * @return      Sum of squares
 */
template <class T>  inline Matrix<T> 
SOS (Matrix<T>& M, const size_t d) {
	
	assert (M.Dim(d) > 1);
	
	unsigned short nd = HDim(M);
	size_t dim [INVALID_DIM];
		
	for (size_t i = 0; i < INVALID_DIM; i++)
		dim[i] = (i != nd) ? M.Dim(i) : 1;
	
	Matrix<T> res (dim);
		
#pragma omp parallel default (shared) 
	{
		
#pragma omp for
		
		for (size_t i = 0; i < res.Size(); i++) {
			for (size_t j = 0; j < M.Dim(nd); j++)
				res[i] = pow (M[i + j * res.Size()], 2.0);
			pow (res[i], 0.5);
		}
		
	}
	
	return res;
	
}


/*
  template <class T> inline  Matrix<T> 
  SOS (const Matrix<T>& M, const size_t d) {
  
  Matrix<T> res = M;
  
  assert (_dim[d] > 1);
  
  #pragma omp parallel default (shared) 
  {
  
  #pragma omp for
  
  for (int i = 0; i < Size(); i++)
  res[i] = M[i]*M[i];
  
  }
  
  return Sum(res, d);
  
  }
*/


/**
 * @brief          Get rid of unused dimension 
 *
 * @param  M       Matrix
 * @return         Squeezed matrix
 */
template <class T> inline  Matrix<T>
Squeeze (Matrix<T>& M) {
	
	size_t found = 0;
	
	for (size_t i = 0; i < INVALID_DIM; i++)
		if (M.Dim(i) > 1) {
			M.Res(found)   = M.Res(i);
			M.Dim(found++) = M.Dim(i);
		}
	
	for (size_t i = found; i < INVALID_DIM; i++) {
		M.Dim(i) = 1;
		M.Res(i) = 1.0;
	}
	
}


/**
 * @brief      Mean reducing a dimension
 *
 * @param  M   Matrix
 * @param  d   Dimension
 * @return     Average of M reducing d matrix
 */
template <class T> inline  Matrix<T>
Mean (const Matrix<T>& M, const size_t d) {
	
	Matrix<T> res  = M;
	float     quot = (float) res.Dim(d);
		
	res = Sum (res, d);
	
	return res / quot;
	
}


/**
 * @brief     Sum along a dimension
 *
 * @param  M  Matrix
 * @param  d  Dimension
 * @return    Sum of M along dimension d
 */
template <class T> inline  Matrix<T>
Sum (Matrix<T>& M, const size_t d) {
	
	Matrix<T> res = M;
	
	assert (d < INVALID_DIM);
	
	// No meaningful sum over particular dimension
	if (res.Dim(d) == 1) return res;
	
	// Empty? allocation 
	if (IsEmpty(M))    return res;
	
	// Save old data and resize matrix 
	T* tmp = (T*) malloc (res.Size() * sizeof (T));
	memcpy (tmp, &M[0], res.Size() * sizeof (T));
	
	// Inner size 
	size_t insize = 1;
	for (size_t i = 0; i < d; i++)
		insize *= res.Dim(i);
	
	// Outer size
	size_t outsize = 1;
	for (size_t i = d+1; i < INVALID_DIM; i++)
		outsize *= res.Dim(i);
	
	res.Resize();
	
	// Sum
#pragma omp parallel default (shared) 
	{
		
		size_t tid      = omp_get_thread_num();
		
		for (size_t i = 0; i < outsize; i++) {
			
#pragma omp for
			
			for (size_t j = 0; j < insize; j++) {
				res[j+i*insize] = 0;
				for (size_t k = 0; k < res.Dim(d); k++)
					res[j+i*insize] += tmp[j+i*insize*res.Dim(d)+k*insize];
			}
			
		}
			
	}
	
	// Adjust dminesions and clear tmp
		res.Dim(d) = 1;
		free (tmp);
		
		return res;
		
}



/**
 * @brief     Highest dimension unequal 1
 * 
 * @param  M  Matrix
 * @return    Highest non-one dimension
 */
template <class T> inline  size_t
HDim (const Matrix<T>& M) {
	
	size_t nd = 0;
	
	for (size_t i = 0; i < INVALID_DIM; i++)
		nd  = (M.Dim(i) > 1) ? i : nd;
	
	return nd;
	
}


/**
 * @brief           RAM occupied
 *
 * @param  M        Matrix
 * @return          Size in RAM in bytes.
 */
template <class T>  inline size_t
SizeInRAM          (const Matrix<T>& M) {
	
	return numel(M) * sizeof (T);
	
}


/**
 * @brief           Get the number of matrix cells, i.e. dim_0*...*dim_16.
 *
 * @param   M       Matrix
 * @return          Number of cells.
 */
template <class T>  size_t
numel               (const Matrix<T>& M) {
	
	size_t s = 1;
    
	for (size_t i = 0; i < INVALID_DIM; i++)
		s *= M.Dim(i);
    
	return s;
	
}


/**
 * @brief           Get vector of dimensions
 *
 * @param   M       Matrix
 * @return          Number of cells.
 */
template <class T>  Matrix<size_t>
size               (const Matrix<T>& M) {
	
	Matrix<size_t> res (INVALID_DIM, 1);
	size_t ones = 0;
    
	for (size_t i = 0; i < INVALID_DIM; i++) {
		
		res[i] = M.Dim(i);
		ones   = (res[i] == 1) ? ones + 1 : ones = 0;
		
	}
    
	return res;
	
}


#endif 
