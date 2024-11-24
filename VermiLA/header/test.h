#pragma once
#include <iostream>
#include <chrono>

namespace VermiLA
{
	void printHello();

	class timer
	{
	public:
		std::chrono::time_point<std::chrono::steady_clock> start, end;
		std::chrono::duration<float> dura;

		timer()
		{
			start = std::chrono::high_resolution_clock::now();
		}

		~timer()
		{
			end = std::chrono::high_resolution_clock::now();
			dura = end - start;
			std::cout << "Timer took : " << dura.count() * 1000.0f << "ms" << std::endl;
		}
	};

	template <typename TYPE>
	typename std::enable_if<!std::numeric_limits<TYPE>::is_integer, bool>::type isAlmostEqual(TYPE x, TYPE y, int ulp = 2)
	{
		return std::fabs(x - y) < std::numeric_limits<TYPE>::epsilon() * std::fabs(x + y) * ulp || std::fabs(x - y) < std::numeric_limits<TYPE>::min();
	};
}