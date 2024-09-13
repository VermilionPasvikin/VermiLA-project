#pragma once
#include <iostream>
#include <ctime>
#include <cstdlib>

namespace VermiLA
{
	void printHello();

	template<typename TYPE>
	TYPE** generate2DArray_randomly(size_t RowCount, size_t ColumnCount)
	{
		srand(time(0));
		int max = 1000;
		int min = -1000;
		TYPE** array = new TYPE * [RowCount];
		for (size_t i = 0; i < RowCount; i++)
		{
			array[i] = new TYPE[ColumnCount];
			for (size_t j = 0; j < ColumnCount; j++)
			{
				array[i][j] = min + rand() % (max - min + 1);
			}
		}
		return array;
	}

	template<typename TYPE>
	TYPE** generate2DArray_manully(size_t RowCount, size_t ColumnCount)
	{
		TYPE** array = new TYPE * [RowCount];
		for (size_t i = 0; i < RowCount; i++)
		{
			array[i] = new TYPE[ColumnCount];
			for (size_t j = 0; j < ColumnCount; j++)
			{
				std::cin >> array[i][j];
			}
		}
		return array;
	}
}