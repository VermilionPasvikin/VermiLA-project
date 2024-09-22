#pragma once
namespace VermiLA
{
	template<typename TYPE>
	TYPE** generate2DArray_randomly(size_t RowCount, size_t ColumnCount)
	{
		srand(time(0));
		int max = 1000;
		int min = -1000;
		TYPE** matrix_array = new TYPE * [RowCount];
		for (size_t i = 0; i < RowCount; i++)
		{
			matrix_array[i] = new TYPE[ColumnCount];
			for (size_t j = 0; j < ColumnCount; j++)
			{
				matrix_array[i][j] = min + rand() % (max - min + 1);
			}
		}
		return matrix_array;
	}

	template<typename TYPE>
	TYPE** generate2DArray_manully(size_t RowCount, size_t ColumnCount)
	{
		TYPE** matrix_array = new TYPE * [RowCount];
		for (size_t i = 0; i < RowCount; i++)
		{
			matrix_array[i] = new TYPE[ColumnCount];
			for (size_t j = 0; j < ColumnCount; j++)
			{
				std::cin >> matrix_array[i][j];
			}
		}
		return matrix_array;
	}

	template<typename TYPE>
	TYPE** generate2DArray_identity(size_t RowCount, size_t ColumnCount)
	{
		TYPE** matrix_array = new TYPE * [RowCount];
		for (size_t i = 0; i < RowCount; i++)
		{
			matrix_array[i] = new TYPE[ColumnCount];
			for (size_t j = 0; j < ColumnCount; j++)
			{
				if (i == j)
				{
					matrix_array[i][j] = 1;
				}
				else
				{
					matrix_array[i][j] = 0;
				}
			}
		}
		return matrix_array;
	}
}