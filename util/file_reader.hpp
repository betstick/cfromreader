/*#ifndef CFR_FILE_READER
#define CFR_FILE_READER*/

#pragma once
#include "stdafx.hpp"

namespace cfr {
	class FileReader
	{
		uint64_t offset; //64 bit for the FUTURE!
		//need a pointer to the file
		FILE* file;

		public:

		FileReader(FILE* filePtr, uint64_t offsetInput)
		{
			file = filePtr;
			offset = offsetInput;
		};

		/*glm::vec3 readVector3()
		{
			glm::vec3 value;
			offset += 12;
			return value;
		};*/

		//one byte
		void readByte(void* target)
		{
			memcpy(target,file+offset,1);
			offset += 1;
		};

		//two bytes
		void readWord(void* target)
		{
			memcpy(target,file+offset,2);
			offset += 2;
		};

		//four bytes
		void readDouble(void* target)
		{
			//memcpy(target,file+offset,4);
			fread(target,4,1,file);
			offset += 4;
		};

		//eight bytes
		void readQuad(void* target)
		{
			//memcpy(target,file+offset,4);
			fread(target,8,1,file);
			offset += 8;
		};

		//void assertPattern(){};

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