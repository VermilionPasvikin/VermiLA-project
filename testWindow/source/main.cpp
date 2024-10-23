#include "test.h"
#include "matrix.h"
#include "vector.h"
#include <iostream>
#include "spawnMatrix.h"

template<typename TYPE>
VermiLA::matrix<TYPE> matrixMultiply(VermiLA::matrix<int>& M1, VermiLA::matrix<int>& M2)
{
	VermiLA::Timer timer1;
	return M1 * M2;
}

template<typename TYPE>
VermiLA::matrix<TYPE> func()
{
	VermiLA::matrix<int> a;
	return a;
}

int main()
{
	VermiLA::matrix<int> m1(3, 3, VermiLA::generate2DArray_randomly<int>(3, 3));
	m1.printMatrix(8,6);

	VermiLA::matrix<int> m2(3, 4, VermiLA::generate2DArray_randomly<int>(3, 4));
	m2.printMatrix(8, 6);

	VermiLA::matrix<int> m3 = m1 * m2;
	m3.printMatrix(12,6);

	m1.multiply(m2);

	m1.printMatrix(12, 6);

	std::cout << "//" << std::endl;

	std::cout << std::endl;
}