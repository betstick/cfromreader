#include "files.hpp"

namespace cfr
{
#ifdef _WIN32
	FILE* v_fopen(const char* filename, const char* modes)
#else
	FILE* v_fopen(const char* __restrict__ __filename, const char* __restrict__ __modes)
#endif
	{
		errno = 0;
		FILE* ptr = fopen(__filename,__modes);

		if(ptr == NULL || errno != 0)
		{
			perror("v_fopen() error: ");
			exit(1);
		}

		return ptr;
	};
};