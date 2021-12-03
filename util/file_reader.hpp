#pragma once
#include "stdafx.hpp"

#ifndef CFR_FILE_READER
#define CFR_FILE_READER

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
			memcpy(target,file+offset,4);
			offset += 4;
		};

		//eight bytes
		void readQuad(void* target)
		{
			memcpy(target,file+offset,4);
			offset += 8;
		};

		//void assertPattern(){};

		private:
	};
};

#endif