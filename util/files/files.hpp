#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

namespace cfr
{
	//fopen() with extra validation and auto abort.
	FILE* v_fopen(const char* __restrict__ __filename, const char* __restrict__ __modes);
};