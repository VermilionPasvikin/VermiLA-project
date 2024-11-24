#include "test.h"
#include "matrix.h"
#include "vector.h"
#include <iostream>
#include "spawnMatrix.h"

template<typename TYPE>
VermiLA::matrix<TYPE> matrixMultiply(VermiLA::matrix<int>& M1, VermiLA::matrix<int>& M2)
{
	VermiLA::timer timer1;
	return M1 * M2;
}

int main()
{
	VermiLA::matrix<double> m1(3, 3, VermiLA::generate2DArray_randomly<double>(3, 3));
	VermiLA::matrix<double> m2(3, 3, VermiLA::generate2DArray_randomly<double>(3, 3));
	VermiLA::matrix<double> m3;

	m1.printMatrix();

	std::cout << m1.isInverseExist() << std::endl;
	if (m1.isInverseExist())
	{
		m2 = m1.getInverse();
		m2.printMatrix();
	}

	m3 = m1;

	m3.printMatrix();

	m3.clearInverse();
	m3.isInverseExist();
}