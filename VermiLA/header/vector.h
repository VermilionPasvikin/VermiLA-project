#pragma once
#include "matrix.h"
#include "test.h"

namespace VermiLA
{
	template<typename TYPE>
	class vector : public matrix<TYPE>
	{
	public:
		vector() : matrix<TYPE>() //Ĭ�Ϲ��캯������һ��1*1��������
		{}

		vector(size_t RowCount) : matrix<TYPE>(RowCount, 1) //�ù��캯����������һ��RowCount�е�������
		{}

		vector(size_t RowCount, TYPE** Array) : matrix<TYPE>(RowCount, 1, Array) //�ù��캯������ֱ�ӵ���matrix��Ĺ��캯������ΪArray�Ǹ���ά���飬����Ч�ʱȽϵͣ����Ƽ�ʹ�ô˷�ʽ
		{}

		vector(size_t RowCount, TYPE* Array) //�ù��캯�����ؽ���һ���޷������ͱ�����һ��һά���飬Ч�ʸ���
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