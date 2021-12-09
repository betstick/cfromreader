/*#ifndef CFR_FILE_READER
#define CFR_FILE_READER*/

#pragma once
#include "stdafx.hpp"

namespace cfr {
	class FileReader
	{
		uint64_t position; //64 bit for the FUTURE!
		uint64_t offset; //buffer start relative to file
		FILE* file;
		void* buffer;
		uint64_t bufferSize;

		public:

		FileReader(FILE* filePtr, void* buffer, size_t size)
		{
			file = filePtr;
			position = 0;
			bufferSize = size;
			fread(buffer,size,1,filePtr);
		};

		void cfrread(void* target, size_t size)
		{
			uint64_t end = position + size;

			if(end < bufferSize + offset)
			{
				position += size;
				memcpy();
			}
			else if(position )
			{

			}
		};

		private:
	};

	//this function is *very* stupid. its simple and works though
	//this may or may not need to be reversed :^)
	int32_t byteToBinary(int8_t byte)
	{
		int32_t output = 0;
		output += (bool)((byte >> 0) & 0x01) ?        1 : 0;
		output += (bool)((byte >> 1) & 0x01) ?       10 : 0;
		output += (bool)((byte >> 2) & 0x01) ?      100 : 0;
		output += (bool)((byte >> 3) & 0x01) ?     1000 : 0;
		output += (bool)((byte >> 4) & 0x01) ?    10000 : 0;
		output += (bool)((byte >> 5) & 0x01) ?   100000 : 0;
		output += (bool)((byte >> 6) & 0x01) ?  1000000 : 0;
		output += (bool)((byte >> 7) & 0x01) ? 10000000 : 0;

		return output;
	};
};

//#endif