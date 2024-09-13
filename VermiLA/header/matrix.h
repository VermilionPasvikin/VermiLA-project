#pragma once
#include <iostream>
#include <iomanip> 
#include <cstring>

namespace VermiLA
{
	template<typename TYPE> //TYPE：矩阵每个数的类型；RowCount：矩阵行数；ColumnCount：矩阵列数；
	class matrix
	{
	public:
		matrix() : row_count(1), column_count(1) //默认构造函数生成一个1*1的零矩阵
		{
			matrix_array = new TYPE[1];
			*matrix_array = 0;
			row_position = new TYPE * [1];
			*row_position = matrix_array;
		}

		matrix(size_t RowCount, size_t ColumnCount) :row_count(RowCount), column_count(ColumnCount) //只输入行数或者列数就初始化为零矩阵
		{
			matrix_array = new TYPE[column_count * row_count]; //为了避免内存碎片问题，用一维数组分配内存空间；
			for (size_t i = 0; i < row_count; i++)
			{
				for (size_t j = 0; j < column_count; j++)
				{
					matrix_array[i * column_count + j] = 0;
				}
			}
			row_position = new TYPE * [row_count]; //存储矩阵每行开头位置以记录矩阵形状
			for (size_t i = 0; i < row_count; i++)
			{
				row_position[i] = matrix_array + column_count * i;
			}
		}

		matrix(size_t RowCount, size_t ColumnCount, TYPE** Matrix) :row_count(RowCount), column_count(ColumnCount)
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
		}

		matrix(const matrix& Matrix) :row_count(Matrix.getRowCount()), column_count(Matrix.getColumnCount())
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
		}

		~matrix()
		{
			delete[] matrix_array;
			delete[] row_position;
		}

		size_t getRowCount() const;
		size_t getColumnCount() const;
		size_t getMatrixSize() const;
		void printMatrix() const;
		void printMatrix(unsigned Width, unsigned Precision) const;

		matrix transpose();

		void rowInterchange(size_t RowIndex1, size_t RowIndex2);
		void rowAddition(size_t SourceRowIndex, size_t OperatedRowIndex, TYPE times);
		void rowMultiplication(size_t RowIndex, TYPE Scalar);

		void toUpperTriangleForm();
		void toEcheloForm();
		void toReducedEcheloForm();

		matrix operator+(const matrix& AddendMatrix)
		{
			return this->add(AddendMatrix);
		}

		matrix operator-(const matrix& SubtrahendMatrix)
		{
			return this->sub(SubtrahendMatrix);
		}

		matrix operator*(int Scalar)
		{
			return this->multiply(Scalar);
		}

		matrix operator*(double Scalar)
		{
			return this->multiply(Scalar);
		}

		matrix operator* (const matrix& MultiplierMatrix)
		{
			return this->multiply(MultiplierMatrix);
		}

		matrix operator=(const matrix& Matrix)
		{
			if (&Matrix != this)
			{
				row_count = Matrix.getRowCount();
				column_count = Matrix.getColumnCount();
				size_t size = row_count * column_count;
				TYPE* copy_from = Matrix.getArray();

				std::memcpy(matrix_array, copy_from, sizeof(TYPE) * size);
			}
			return*this;
		}

		TYPE* operator[](size_t index)
		{
			return getRowPosition(index);
		}

	protected:
		matrix add(const matrix& AddendMatrix);
		matrix sub(const matrix& AddendMatrix);
		matrix multiply(int Scalar);
		matrix multiply(double Scalar);
		matrix multiply(const matrix<TYPE>& MultiplierMatrix);
		void setNumber(size_t Index, TYPE value);
		void setNumber(size_t RowIndex, size_t ColumnIndex, TYPE value);
		TYPE* getArray() const;
		TYPE* getRowPosition(int RowIndex) const;

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
	matrix<TYPE> matrix<TYPE>::add(const matrix& AddendMatrix)
	{
		matrix<TYPE> local_matrix(row_count, column_count);
		TYPE* addend_matrix_array = AddendMatrix.getArray();
		for (size_t i = 0; i < this->getMatrixSize(); i++)
		{
			local_matrix.setNumber(i, matrix_array[i] + addend_matrix_array[i]);
		}
		return local_matrix;
	}

	template<typename TYPE>
	matrix<TYPE> matrix<TYPE>::sub(const matrix& SubtrahendMatrix)
	{
		matrix<TYPE> local_matrix(row_count, column_count);
		TYPE* subtrahend_matrix_array = SubtrahendMatrix.getArray();
		for (size_t i = 0; i < this->getMatrixSize(); i++)
		{
			local_matrix.setNumber(i, matrix_array[i] - subtrahend_matrix_array[i]);
		}
		return local_matrix;
	}

	template<typename TYPE>
	matrix<TYPE> matrix<TYPE>::multiply(int Scalar)
	{
		matrix<TYPE> local_matrix(row_count, column_count);
		for (size_t i = 0; i < this->getMatrixSize(); i++)
		{
			local_matrix.setNumber(i, matrix_array[i] * Scalar);
		}
		return local_matrix;
	}

	template<typename TYPE>
	matrix<TYPE> matrix<TYPE>::multiply(double Scalar)
	{
		matrix<TYPE> local_matrix(row_count, column_count);
		for (size_t i = 0; i < this->getMatrixSize(); i++)
		{
			local_matrix.setNumber(i, matrix_array[i] * Scalar);
		}
		return local_matrix;
	}

	template<typename TYPE>
	matrix<TYPE> matrix<TYPE>::multiply(const matrix<TYPE>& MultiplierMatrix)
	{
		size_t multiplyier_matrix_column_count = MultiplierMatrix.getColumnCount();
		TYPE* multiplyier_matrix_array = MultiplierMatrix.getArray();
		matrix<TYPE> product_matrix(row_count, multiplyier_matrix_column_count);
		TYPE* product_matrix_array = product_matrix.getArray();

		size_t i(0), j(0), k(0);
		do
		{
			do
			{
				do
				{
					product_matrix_array[i * multiplyier_matrix_column_count + j] += matrix_array[i * column_count + k] * multiplyier_matrix_array[k * multiplyier_matrix_column_count + j];
					k++;
				} while (k < column_count);
				k = 0;
				j++;
			} while (j < multiplyier_matrix_column_count);
			j = 0;
			i++;
		} while (i < row_count);

		return product_matrix;
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
			row_position[RowIndex][i] *= Scalar;
		}
	}

	template<typename TYPE> //这个仅仅是重新排列每一行的顺序！并不包括化简！
	void matrix<TYPE>::toUpperTriangleForm()
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
						this->rowInterchange(j, max_arranged_row_index);
						size_t temp_store = row_zero_count_array[j];
						row_zero_count_array[j] = row_zero_count_array[max_arranged_row_index];
						row_zero_count_array[max_arranged_row_index] = temp_store;
						max_arranged_row_index++;
					}
					else continue;
				}
			}
		}

		delete[] row_zero_count_array;
	}

	template<typename TYPE>
	void matrix<TYPE> ::toEcheloForm()
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
			TYPE first_nonzero_element = row_position[focus_row_index][first_nonzero_element_column_index];
			for (size_t i = first_nonzero_element_column_index; i < column_count; i++)
			{
				row_position[focus_row_index][i] /= first_nonzero_element;
			}

			//对焦点行首个非零元素同一列以下的所有元素进行消去
			for (size_t i = focus_row_index+1; i < row_count; i++)
			{
				this->rowAddition(focus_row_index, i, -1 * row_position[i][first_nonzero_element_column_index]);
			}

			this->toUpperTriangleForm();//将可能的新产生的零行移到最下
		}
	}
	template<typename TYPE>
	void matrix<TYPE>::toReducedEcheloForm()
	{
		//化简成行阶梯型
		this->toEcheloForm(); 

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
				this->rowAddition(row_index, i, -1 * row_position[i][first_nonzero_element_column_index]);
			}
		}
	}
}