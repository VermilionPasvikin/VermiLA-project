#pragma once
#include "matrix.h"
#include "test.h"

namespace VermiLA
{
	template<typename TYPE>
	class vector : public matrix<TYPE>
	{
	public:
		vector() : matrix<TYPE>() //默认构造函数生成一个1*1的零向量
		{}

		vector(size_t RowCount) : matrix<TYPE>(RowCount, 1) //该构造函数重载生成一个RowCount行的零向量
		{}

		vector(size_t RowCount, TYPE** Array) : matrix<TYPE>(RowCount, 1, Array) //该构造函数重载直接调用matrix类的构造函数，因为Array是个二维数组，可能效率比较低，不推荐使用此方式
		{}

		vector(size_t RowCount, TYPE* Array) //该构造函数重载接受一个无符号整型变量和一个一维数组，效率更高
		{
			matrix<TYPE>::row_count = RowCount;
			matrix<TYPE>::column_count = 1;

			matrix<TYPE>::matrix_array = new TYPE[RowCount];
			memcpy(matrix<TYPE>::matrix_array, Array, RowCount);

			matrix<TYPE>::row_position = new TYPE * [RowCount];
			for (size_t i = 0; i < RowCount; i++)
			{
				matrix<TYPE>::row_position[i] = matrix<TYPE>::matrix_array + i;
			}
		}

		~vector()
		{}

		size_t getDimension() const;
		void printVector() const;
		void printVector(unsigned Width, unsigned Precision) const;

		TYPE& operator[](size_t index)
		{
			return *matrix<TYPE>::row_position[index];
		}

	protected:
		using matrix<TYPE>::printMatrix;
		using matrix<TYPE>::getRowCount;
		using matrix<TYPE>::getColumnCount;
		using matrix<TYPE>::getMatrixSize;
	};

	template<typename TYPE>
	inline size_t vector<TYPE>::getDimension() const
	{
		return matrix<TYPE>::getRowCount();
	}

	template<typename TYPE>
	void vector<TYPE>::printVector() const
	{
		matrix<TYPE>::printMatrix();
	}

	template<typename TYPE>
	void vector<TYPE>::printVector(unsigned Width, unsigned Precision) const
	{
		matrix<TYPE>::printMatrix(Width, Precision);
	}
}