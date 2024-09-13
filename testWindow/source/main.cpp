#include "test.h"
#include "matrix.h"
#include "vector.h"
#include <iostream>

int main()
{
	VermiLA::vector<double> C(3,VermiLA::generate2DArray_randomly<double>(3,1));
	C.printVector();
	
	VermiLA::matrix<double> A(3, 3, VermiLA::generate2DArray_randomly<double>(3, 3));
	A.rowInterchange(0,1);

	std::cout << C[2] << std::endl;
	C[2] = 0;
	C.printVector();

	std::cin.get();
}