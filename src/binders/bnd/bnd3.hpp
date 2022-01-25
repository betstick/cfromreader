#pragma once
#include "../stdafx.hpp"

namespace cfr
{
	struct BND3File
	{
		FILE* parent;
		unsigned int offset;
		int size;
		int id;
		CfrFileType type;
	};

	BND3File* openBnd3(FILE* src, int* count);

	BND3File* openBnd3(const char* path, int* count);
};