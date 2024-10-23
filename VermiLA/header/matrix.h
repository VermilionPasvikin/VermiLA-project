#pragma once
#include <iostream>
#include <iomanip> 
#include <cstring>
#include "spawnMatrix.h"

namespace VermiLA
{
	template<typename TYPE> //TYPE：矩阵每个数的类型；RowCount：矩阵行数；ColumnCount：矩阵列数；
	class matrix
	{
	public:
		matrix() : row_count(1), column_count(1),is_square_matrix(true),exist_inverse(true),inverse(nullptr) //默认构造函数生成一个1*1的零矩阵
		{
			matrix_array = new TYPE[1];
			*matrix_array = 0;
			row_position = new TYPE * [1];
			*row_position = matrix_array;
		}

		matrix(size_t RowCount, size_t ColumnCount) :row_count(RowCount), column_count(ColumnCount),is_square_matrix(false),exist_inverse(true),inverse(nullptr) //只输入行数或者列数就初始化为零矩阵
		{
			matrix_array = new TYPE[column_count * row_count]; //为了避免内存碎片问题，用一维数组分配内存空间；
			memset(matrix_array, 0, sizeof(int) * row_count * column_count);
			row_position = new TYPE * [row_count]; //存储矩阵每行开头位置以记录矩阵形状
			for (size_t i = 0; i < row_count; i++)
			{
				row_position[i] = matrix_array + column_count * i;
			}
			if (row_count == column_count)
			{
				is_square_matrix = true;
			}
		}

		matrix(size_t RowCount, size_t ColumnCount, TYPE** Matrix) :row_count(RowCount), column_count(ColumnCount),is_square_matrix(false),exist_inverse(true),inverse(nullptr)
		{
			matrix_array = new TYPE[column_count * row_count]; //为了避免内存碎片问题，用一维数组分配内存空间；
			for (size_t i = 0; i < row_count; i++)
			{
				for (size_t j = 0; j < column_count; j++)
				{
					matrix_array[i * column_count + j] = Matrix[i][j];
				}
			}
			row_position = new TYPE * [row_count]; //存储矩阵每行开头位置以记录矩阵形状
			for (size_t i = 0; i < row_count; i++)
			{
				row_position[i] = matrix_array + column_count * i;
			}
			if (row_count == column_count)
			{
				is_square_matrix = true;
			}
		}

		matrix(const matrix& Matrix) :row_count(Matrix.getRowCount()), column_count(Matrix.getColumnCount()),is_square_matrix(Matrix.isSquareMatrix())
		{
			size_t size = row_count * column_count;
			TYPE* copy_from = Matrix.getArray();

			matrix_array = new TYPE[row_count * column_count];
			std::memcpy(matrix_array, copy_from, sizeof(TYPE) * size);
			row_position = new TYPE * [row_count];
			for (size_t i = 0; i < row_count; i++)
			{
				row_position[i] = matrix_array + column_count * i;
			}

			if (Matrix.inverse != nullptr)
			{
				this->inverse = new matrix(*(Matrix.inverse));
				exist_inverse = Matrix.exist_inverse;
			}
			else
			{
				inverse = nullptr;
				exist_inverse = true;
			}
		}

		~matrix()
		{
			delete[] matrix_array;
			delete[] row_position;
			if (inverse != nullptr)
			{
				delete inverse;
			}
		}

		size_t getRowCount() const;
		size_t getColumnCount() const;
		size_t getMatrixSize() const;
		void printMatrix() const;
		void printMatrix(unsigned Width, unsigned Precision) const;
		bool isSquareMatrix() const;
		bool isInverseExist();

		void setNumber(size_t Index, TYPE value);
		void setNumber(size_t RowIndex, size_t ColumnIndex, TYPE value);

		void add(const matrix& AddendMatrix);
		void sub(const matrix& AddendMatrix);
		void multiply(int Scalar);
		void multiply(double Scalar);
		void multiply(const matrix<TYPE>& MultiplierMatrix);

		matrix getInverse();

		matrix transpose();

		void rowInterchange(size_t RowIndex1, size_t RowIndex2);
		void rowAddition(size_t SourceRowIndex, size_t OperatedRowIndex, TYPE times);
		void rowMultiplication(size_t RowIndex, TYPE Scalar);

		matrix toUpperTriangleForm(bool SyncToInverse = false);
		matrix toEcheloForm(bool SyncToInverse = false);
		matrix toReducedEcheloForm(bool SyncToInverse = false);

		matrix operator+(const matrix& AddendMatrix)
		{
			if (not sameForm(AddendMatrix))
			{
				throw "MatrixAddition::Exceptional matrix form(Row count and column count).";
			}

			matrix<TYPE> local_matrix(row_count, column_count);
			TYPE* addend_matrix_array = AddendMatrix.getArray();
			for (size_t i = 0; i < this->getMatrixSize(); i++)
			{
				local_matrix.setNumber(i, matrix_array[i] + addend_matrix_array[i]);
			}
			return local_matrix;
		}

		matrix operator-(const matrix& SubtrahendMatrix)
		{
			if (not sameForm(SubtrahendMatrix))
			{
				throw "MatrixSubtraction::Exceptional matrix form(Row count and column count).";
			}

			matrix<TYPE> local_matrix(row_count, column_count);
			TYPE* subtrahend_matrix_array = SubtrahendMatrix.getArray();
			for (size_t i = 0; i < this->getMatrixSize(); i++)
			{
				local_matrix.setNumber(i, matrix_array[i] - subtrahend_matrix_array[i]);
			}
			return local_matrix;
		}

		matrix operator*(int Scalar)
		{
			matrix<TYPE> local_matrix(row_count, column_count);
			for (size_t i = 0; i < this->getMatrixSize(); i++)
			{
				local_matrix.setNumber(i, matrix_array[i] * Scalar);
			}
			return local_matrix;
		}

		matrix operator*(double Scalar)
		{
			matrix<TYPE> local_matrix(row_count, column_count);
			for (size_t i = 0; i < this->getMatrixSize(); i++)
			{
				local_matrix.setNumber(i, matrix_array[i] * Scalar);
			}
			return local_matrix;
		}

		matrix operator* (const matrix& MultiplierMatrix)
		{
			size_t multiplier_matrix_column_count = MultiplierMatrix.getColumnCount();
			size_t multiplier_matrix_row_count = MultiplierMatrix.getRowCount();
			TYPE* multiplier_matrix_array = MultiplierMatrix.getArray();
			matrix<TYPE> product_matrix(row_count, multiplier_matrix_column_count);
			TYPE* product_matrix_array = product_matrix.getArray();

			if (column_count != multiplier_matrix_row_count)
			{
				throw "MatrixMultiply::Exceptional matrix form(Row count and column count).";
			}

			for (size_t i = 0; i < row_count; ++i)
			{
				for (size_t k = 0; k < column_count; ++k)
				{
					TYPE temp = matrix_array[i * column_count + k];
					for (size_t j = 0; j < multiplier_matrix_column_count; ++j)
					{
						product_matrix_array[i * multiplier_matrix_column_count + j] +=
							temp * multiplier_matrix_array[k * multiplier_matrix_column_count + j];
					}
				}
			}

			/*size_t i(0), j(0), k(0);   //release模式下（优选速度O（2））for循环版本效率更高些
			do
			{
				do
				{
					TYPE temp = matrix_array[i * column_count + k];
					do
					{
						product_matrix_array[i * multiplier_matrix_column_count + j] += temp * multiplier_matrix_array[k * multiplier_matrix_column_count + j];
						++j;
					} while (j < multiplier_matrix_column_count);
					j = 0;
					++k;
				} while (k < column_count);
				k = 0;
				++i;
			} while (i < row_count);*/
			return product_matrix;
		}

		matrix operator=(const matrix& Matrix)
		{
			if (&Matrix != this)
			{
				this->row_count = Matrix.getRowCount();
				this->column_count = Matrix.getColumnCount();
				size_t size = row_count * column_count;
				TYPE* copy_from = Matrix.getArray();

				delete matrix_array;
				matrix_array = new TYPE[row_count * column_count];
				std::memcpy(matrix_array, copy_from, sizeof(TYPE) * size);

				delete[] row_position;
				row_position = new TYPE * [row_count];
				for (size_t i = 0; i < row_count; i++)
				{
					row_position[i] = matrix_array + column_count * i;
				}

				this->is_square_matrix = Matrix.is_square_matrix;
				this->exist_inverse = Matrix.exist_inverse;
				if (Matrix.inverse != nullptr)
				{
					if (inverse != nullptr)
					{
						delete inverse;
					}
					this->inverse = new matrix((*Matrix.inverse));
				}
			}
			return*this;
		}

		TYPE* operator[](size_t index) //你可以像访问二维数组一样访问矩阵中对应行列的元素！
		{
			return getRowPosition(index);
		}

	protected:
		TYPE* getArray() const;
		TYPE* getRowPosition(int RowIndex) const;
		bool sameForm(const matrix<TYPE>& Matrix);

		mutable matrix* inverse;
		bool exist_inverse;
		bool is_square_matrix;

		size_t row_count;
		size_t column_count;
		TYPE* matrix_array;
		TYPE** row_position;
	};

	template<typename TYPE>
	inline size_t matrix<TYPE>::getRowCount() const
	{
		return row_count;
	}

	template<typename TYPE>
	inline size_t matrix<TYPE>::getColumnCount() const
	{
		return column_count;
	}

	template<typename TYPE>
	inline TYPE* matrix<TYPE>::getArray() const
	{
		return matrix_array;
	}

	template<typename TYPE>
	inline size_t matrix<TYPE>::getMatrixSize() const
	{
		return row_count * column_count;
	}

	template<typename TYPE>
	inline TYPE* matrix<TYPE>::getRowPosition(int RowIndex) const
	{
		return row_position[RowIndex];
	}

	template<typename TYPE>
	bool matrix<TYPE>::sameForm(const matrix<TYPE>& Matrix)
	{
		if (row_count == Matrix.row_count and column_count == Matrix.column_count)
			return true;
		else
			return false;
	}

	template<typename TYPE>
	void matrix<TYPE>::printMatrix() const
	{
		size_t matrix_size = getMatrixSize();
		for (size_t i = 0; i < matrix_size; i++)
		{
			std::cout << matrix_array[i] << " ";
			if ((i + 1) % column_count == 0)
			{
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;
	}

	template<typename TYPE>
	void matrix<TYPE>::printMatrix(unsigned Width, unsigned Precision) const
	{
		for (size_t i = 0; i < getMatrixSize(); i++)
		{
			std::cout.precision(Precision);
			std::cout << std::setiosflags(std::ios::left) << std::setw(Width) << matrix_array[i];
			if ((i + 1) % column_count == 0 and i != 0)
			{
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;
		std::cout.precision(6);
		std::cout << std::resetiosflags(std::ios::left);
	}

	template<typename TYPE>
	inline bool matrix<TYPE>::isSquareMatrix() const
	{
		return is_square_matrix;
	}

	template<typename TYPE>
	bool matrix<TYPE>::isInverseExist()
	{
		//如果不是方阵直接返回false
		//如果是方阵且Inverse是nullptr就对自己进行行化简，同时将Inverse初始化成一个单位矩阵
		//在对自己行化简的同时对Inverse采取相同操作
		//最终看Inverse有没有0行，如果有的话就把Inverse_exist设置成false
		//返回Inverse_exist

		if (not is_square_matrix)
		{
			return false;
		}
		else
		{
			if (inverse == nullptr)
			{
				size_t matrix_size = this->getMatrixSize();
				inverse = new matrix<TYPE>(row_count,column_count,generate2DArray_identity<TYPE>(row_count,column_count));
				TYPE* tempStore = new TYPE[matrix_size];
				memcpy(tempStore, matrix_array, sizeof(TYPE) * matrix_size);
				this->toReducedEcheloForm(true);
				memcpy(matrix_array, tempStore, sizeof(TYPE) * matrix_size);

				for (size_t i = 0; i < row_count; ++i)
				{
					bool zero_row = true;
					for (size_t j = 0; j < column_count; ++j)
					{
						if (matrix_array[i*column_count + j] != 0)
						{
							zero_row = false;
						}
					}
					if (zero_row == true)
					{
						exist_inverse = false;
						break;
					}
				}

				return exist_inverse;
			}
			else
			{
				return exist_inverse;
			}
		}
	}

	template<typename TYPE>
	matrix<TYPE> matrix<TYPE>::getInverse()
	{
		if (is_square_matrix == false)
		{
			throw "MatrixInverse::Matrix inverse dose not exist.";
		}
		else
		{
			if (exist_inverse == false)
			{
				throw "MatrixInverse::Matrix inverse dose not exist.";
			}
			else
			{
				if (inverse == nullptr)
				{
					this->isInverseExist();
					if (exist_inverse == false)
					{
						throw "MatrixInverse::Matrix inverse dose not exist.";
					}
					else
					{
						return *inverse;
					}
				}
				else
				{
					return *inverse;
				}
			}
		}
	}

	template<typename TYPE>
	inline void matrix<TYPE>::setNumber(size_t Index, TYPE value)
	{
		matrix_array[Index] = value;
	}

	template<typename TYPE>
	inline void matrix<TYPE>::setNumber(size_t RowIndex, size_t ColumnIndex, TYPE value)
	{
		this->setNumber(RowIndex * column_count + ColumnIndex, value);
	}

	template<typename TYPE>
	void matrix<TYPE>::add(const matrix& AddendMatrix)
	{
		if (not sameForm(AddendMatrix))
		{
			throw "MatrixAddition::Exceptional matrix form(Row count and column count).";
		}

		TYPE* addend_matrix_array = AddendMatrix.getArray();
		for (size_t i = 0; i < this->getMatrixSize(); i++)
		{
			this->setNumber(i, matrix_array[i] + addend_matrix_array[i]);
		}
	}

	template<typename TYPE>
	void matrix<TYPE>::sub(const matrix& SubtrahendMatrix)
	{
		if (not sameForm(SubtrahendMatrix))
		{
			throw "MatrixSubtraction::Exceptional matrix form(Row count and column count).";
		}

		TYPE* subtrahend_matrix_array = SubtrahendMatrix.getArray();
		for (size_t i = 0; i < this->getMatrixSize(); i++)
		{
			this->setNumber(i, matrix_array[i] - subtrahend_matrix_array[i]);
		}
	}

	template<typename TYPE>
	void matrix<TYPE>::multiply(int Scalar)
	{
		for (size_t i = 0; i < this->getMatrixSize(); i++)
		{
			this->setNumber(i, matrix_array[i] * Scalar);
		}
	}

	template<typename TYPE>
	void matrix<TYPE>::multiply(double Scalar)
	{
		for (size_t i = 0; i < this->getMatrixSize(); i++)
		{
			this->setNumber(i, matrix_array[i] * Scalar);
		}
	}

	template<typename TYPE>
	void matrix<TYPE>::multiply(const matrix<TYPE>& MultiplierMatrix)
	{
		(*this) = ((*this) * MultiplierMatrix);
	}

	template<typename TYPE>
	matrix<TYPE> matrix<TYPE>::transpose()
	{
		matrix<TYPE> transpose_matrix(column_count, row_count);
		for (size_t i = 0; i < column_count; i++)
		{
			for (size_t j = 0; j < row_count; j++)
			{
				transpose_matrix.setNumber(i, j, matrix_array[j * column_count + i]);
			}
		}
		return transpose_matrix;
	}

	template<typename TYPE>
	void matrix<TYPE>::rowInterchange(size_t RowIndex1, size_t RowIndex2)
	{
		TYPE temporaily_storage;
		for (size_t i = 0; i < column_count; i++)
		{
			temporaily_storage = row_position[RowIndex1][i];
			row_position[RowIndex1][i] = row_position[RowIndex2][i];
			row_position[RowIndex2][i] = temporaily_storage;
		}
	}

	template<typename TYPE>
	void matrix<TYPE>::rowAddition(size_t SourceRowIndex, size_t OperatedRowIndex, TYPE times)
	{
		for (size_t i = 0; i < column_count; i++)
		{
			row_position[OperatedRowIndex][i] += times * row_position[SourceRowIndex][i];
		}
	}

	template<typename TYPE>
	void matrix<TYPE>::rowMultiplication(size_t RowIndex, TYPE Scalar)
	{
		for (size_t i = 0; i < column_count; i++)
		{
			if (row_position[RowIndex][i] != 0)
			{
				row_position[RowIndex][i] *= Scalar;
			}
		}
	}

	template<typename TYPE> //这个仅仅是重新排列每一行的顺序！并不包括化简！
	matrix<TYPE> matrix<TYPE>::toUpperTriangleForm(bool SyncToInverse)
	{
		//记录每一行开头的连续0数
		size_t* row_zero_count_array = new size_t[row_count];
		size_t zero_count = 0;
		size_t max_zero_count = 0;
		size_t max_arranged_row_index = 0;

		for (size_t i = 0; i < row_count; i++)
		{
			for (size_t j = 0; j < column_count; j++)
			{
				if (row_position[i][j] == 0)
				{
					zero_count++;
					if (row_position[i][j + 1] != 0)
						break;
				}
				else break;
			}
			row_zero_count_array[i] = zero_count;
			if (max_zero_count < zero_count)
				max_zero_count = zero_count;
			zero_count = 0;
		}

		if (max_zero_count != 0) //把每一行开头连续0数较小的行交换到靠前的位置
		{
			for (size_t i = 0; i < max_zero_count; i++)
			{
				for (size_t j = 0; j < row_count; j++)
				{
					if (row_zero_count_array[j] == i)
					{
						if (j != max_arranged_row_index)
						{
							this->rowInterchange(j, max_arranged_row_index);
							if (SyncToInverse)
							{
								inverse->rowInterchange(j, max_arranged_row_index);
							}
							size_t temp_store = row_zero_count_array[j];
							row_zero_count_array[j] = row_zero_count_array[max_arranged_row_index];
							row_zero_count_array[max_arranged_row_index] = temp_store;
						}
						max_arranged_row_index++;
					}
					else continue;
				}
			}
		}

		delete[] row_zero_count_array;
		return *this;
	}

	template<typename TYPE>
	matrix<TYPE> matrix<TYPE>::toEcheloForm(bool SyncToInverse)
	{
		for (size_t focus_row_index = 0; focus_row_index < row_count; focus_row_index++)
		{
			//将目前的焦点行所有元素除以首个非零元素
			//获得焦点行首个非零元素的列索引
			size_t first_nonzero_element_column_index = 0;
			while (row_position[focus_row_index][first_nonzero_element_column_index] == 0)
			{
				first_nonzero_element_column_index++;
			}

			//将焦点行所有元素除以首个非零元素
			if (first_nonzero_element_column_index != column_count)
			{
				TYPE first_nonzero_element = row_position[focus_row_index][first_nonzero_element_column_index];
				if (first_nonzero_element != 1)
				{
					TYPE times = 1 / first_nonzero_element;
					rowMultiplication(focus_row_index, times);
					if (SyncToInverse)
					{
						inverse->rowMultiplication(focus_row_index, times);
					}
				}
			}

			//对焦点行首个非零元素同一列以下的所有元素进行消去
			for (size_t i = focus_row_index+1; i < row_count; i++)
			{
				TYPE times = -1 * row_position[i][first_nonzero_element_column_index];
				this->rowAddition(focus_row_index, i, times);
				if (SyncToInverse)
				{
					inverse->rowAddition(focus_row_index, i, times);
				}
			}

			this->toUpperTriangleForm(SyncToInverse);//将可能的新产生的零行移到最下
		}

		return *this;
	}

	template<typename TYPE>
	matrix<TYPE> matrix<TYPE>::toReducedEcheloForm(bool SyncToInverse)
	{
		//化简成行阶梯型
		this->toEcheloForm(SyncToInverse); 

		//找到最大索引非零行
		bool not_all_zero = false;
		size_t nonzero_row_count;

		for (nonzero_row_count = 0; nonzero_row_count < row_count; nonzero_row_count++)
		{
			for (size_t column_index = 0; column_index < column_count; column_index++)
			{
				if (row_position[nonzero_row_count][column_index] != 0)
				{
					not_all_zero = true;
					break;
				}
			}
			if (!not_all_zero)
				break;
		}

		//非零行的数量-1 = 最大非零行索引，从该行开始，开始逐行向上消去主元位置上方的非零元素
		for (size_t row_index = nonzero_row_count - 1; row_index > 0; row_index--)
		{
			size_t first_nonzero_element_column_index = 0;
			while (row_position[row_index][first_nonzero_element_column_index] == 0)
			{
				first_nonzero_element_column_index++;
			}

			for (size_t i = 0; i < row_index; i++)
			{
				TYPE times = -1 * row_position[i][first_nonzero_element_column_index];
				this->rowAddition(row_index, i, times);
				if (SyncToInverse)
				{
					inverse->rowAddition(row_index, i, times);
				}
			}
		}
		return *this;
	}
}