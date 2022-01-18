#pragma once
#include "stdafx.hpp"

//angerly made because i can't figure out why fread() is doing weird things
//with my other data streams. so i'm switching to this.

namespace cfr2
{
	struct MEM
	{
		FILE* file;
		std::queue<int> stepStack[64];
		std::queue<int> markStack[64];
	};

	MEM* mOpen(void* stream, size_t len, const char* modes)
	{
		MEM mem;
		mem.file = fmemopen(stream,len,modes);
		return &mem;
	};

	MEM* mOpenFile(const char* path)
	{
		MEM mem;
		mem.file = fopen(path,"rb");
		return &mem;
	};

	size_t mRead(void* dest, size_t size, size_t n, MEM* mem)
	{
		return fread(dest,size,n,mem->file);
	};

	int mSeek(MEM* mem, long off, int whence)
	{
		return fseek(mem->file,off,whence);
	}
}