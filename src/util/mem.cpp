#include "mem.hpp"

namespace cfr
{

	MEM* mopen(void* src, size_t size)
	{
		MEM* mem;

		mem->data = (char*)src;
		mem->size = size;
		mem->position = 0;

		return mem;
	};

	void mread(void* dest, size_t size, size_t n, MEM* source)
	{
		memcpy(dest,&source->data[source->position],size*n);
		source->position += size*n;

		if(source->position > source->size)
			throw std::runtime_error("Reading undefined memory!\n");
	};

	void mseek(MEM* mem, long offset, int whence)
	{
		switch(whence)
		{
			case 0: mem->position = offset; break;
			case 1: mem->position += offset; break;
			case 2: mem->position = mem->size + offset; break;
			default: mem->position = 0;
		}

		if(mem->position > mem->size)
			throw std::runtime_error("Reading undefined memory!\n");
	};

	long mtell(MEM* mem)
	{
		return mem->position;
	};

	void mclose(MEM* mem)
	{
		//idk
	};
};