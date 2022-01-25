#ifndef CFR_MEM__
#define CFR_MEM__
#pragma once
#include "stdafx.hpp"

namespace cfr
{
	struct MEM
	{
		char* data;
		size_t size;
		long position;
	};

	MEM* mopen(void* src, size_t size);

	void mread(void* dest, size_t size, size_t n, MEM* source);

	void mseek(MEM* mem, long offset, int whence);

	long mtell(MEM* mem);

	void mclose(MEM* mem);
};

#endif