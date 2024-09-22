#pragma once
#include <iostream>
#include <chrono>

namespace VermiLA
{
	void printHello();

	class Timer
	{
	public:
		std::chrono::time_point<std::chrono::steady_clock> start, end;
		std::chrono::duration<float> dura;

		Timer()
		{
			start = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			end = std::chrono::high_resolution_clock::now();
			dura = end - start;
			std::cout << "Timer took : " << dura.count() * 1000.0f << "ms" << std::endl;
		}
	};
}