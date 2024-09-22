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

int main()
{
	VermiLA::matrix<double> m1(3, 3, VermiLA::generate2DArray_manully<double>(3, 3));
	m1.printMatrix(8,6);

	std::cout << (bool)m1.isInverseExist() << std::endl;

	if (m1.isInverseExist())
	{
		m1.getInverse().printMatrix(12,6);
	}

	std::cin.get();
}