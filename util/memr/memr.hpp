#ifndef CFR_MEMR_H__
#define CFR_MEMR_H__

//this is insecure on windows :^)
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h> //free
#include <string.h> //memcpy
#include <stdbool.h> //bools

//needs ftell, fread, ferror, fwrite
namespace cfr
{
	struct MEM
	{
		char* src;
		size_t size;
		long position;
		bool err;
		bool eod; //end of data
	};

	//Open arbitrary data for RW operations, not thread safe :^)
	MEM* mopen(char* src, size_t size);

	//Read data like with fread()
	size_t mread(void* dest, size_t size, size_t n, MEM* source);

	size_t mwrite(char* source, size_t size, size_t count, MEM* dest);

	//Set position like with fseek()
	void mseek(MEM* mem, long offset, int whence);

	//Returns position in data
	long mtell(MEM* mem);

	//Close the data or something
	void mclose(MEM* mem);

	//If end of data has been reached
	int meod(MEM* mem);

	//If an error has occurred
	int merror(MEM* mem);

	//Return pointer to position in memory
	char* mtellptr(MEM* mem);
};

#endif