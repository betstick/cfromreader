#pragma once
#include "stdafx.hpp"

//angerly made because i can't figure out why fread() is doing weird things
//with my other data streams. so i'm switching to this. its a better FILE
//system than the native C one intended entirely for reading from memory.

//Not sure if I'm gonna keep this. Might be useful, but also might be too complex.

namespace cfr
{
	struct MEM
	{
		FILE** file;
		std::queue<int> stepStack[64];
		std::queue<int> markStack[64];
	};

	MEM* mOpen(void* stream, size_t len, const char* modes)
	{
		MEM* mem;
		*mem->file = fmemopen(stream,len,modes);
		return mem;
	};

	MEM* mOpen(const char* path, const char* modes)
	{
		MEM* mem;
		*mem->file = fopen(path,modes);
		return mem;
	};

	//Open file and copy to dest, returns size.
	int openFileToBuffer(const char* path, void* dest)
	{
		FILE* file = fopen(path,"rb");

		fseek(file,0,SEEK_END);
		long size = ftell(file);
		fseek(file,0,SEEK_SET);

		int ret = fread(dest,size,1,file);
		fclose(file);

		return ret;
	};

	size_t mRead(void* dest, size_t size, size_t n, MEM* mem)
	{
		return fread(dest,size,n,*mem->file);
	};

	int mSeek(MEM* mem, long off, int whence)
	{
		return fseek(*mem->file,off,whence);
	}

	void mStepIn(MEM* mem, long off)
	{
		mem->stepStack->push(off);
		mSeek(mem,off,SEEK_SET);
	};

	void mStepOut(MEM* mem)
	{
		mSeek(mem,mem->stepStack->front(),SEEK_SET);
		mem->stepStack->pop();
	};

	void mMarkPos(MEM* mem)
	{
		mem->markStack->push(ftell(*mem->file));
	};

	void mReturnToMark(MEM* mem)
	{
		mSeek(mem,mem->markStack->front(),SEEK_SET);
		mem->markStack->pop();
	};

	int mTell(MEM* mem)
	{
		return ftell(*mem->file);
	}

	int mClose(MEM* mem)
	{
		return fclose(*mem->file);
	};
}