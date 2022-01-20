#pragma once
#include "stdafx.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

namespace cfr
{
	duration<double, std::milli> cfrTimeFunction(int count, void (*function)())
	{
		auto t1 = high_resolution_clock::now();

		//slightly optimized for loop :)
		for(int i = count; i > 0; i--)
			function();

		auto t2 = high_resolution_clock::now();
		
		duration<double, std::milli> ms_double = t2 - t1;

		return ms_double;
	}; 
};