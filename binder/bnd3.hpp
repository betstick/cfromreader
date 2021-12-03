#pragma once
#include "stdafx.hpp"

#ifndef BND3_FORMAT
#define BND3_FORMAT

//the structs aren't intended to be used outside of this file, hence their ugly names

namespace cfr {
	struct _BND3_Header_
	{
		//from file
		char magic[4]; //asert BND3
		char version[8];
		uint8_t rawFormat; //raw byte
		int8_t bigEndian; //if 1, error out
		int8_t bitBigEndian; //also error out on 1?
		int8_t unk0F; //assert 0
		int32_t fileCount;
		int32_t fileHeadersEnd; //does not include padding before data starts
		int32_t unk18;
		int32_t unk1C; //assert 0

		//generated
		//uint8_t format; //not needed as we're always little endian
	};

	struct _BND3_File_
	{
		//from file
		uint8_t rawFlags;
		int8_t unk01; //assert 0
		int8_t unk02; //assert 0
		int8_t unk03; //assert 0
		int32_t compressedSize;
		uint32_t dataOffset;
		uint32_t id; //if(header.format & 0b00000010)//???
		uint32_t nameOffset; //if(header.format & 0b00000100 | header.format & 0b00001000)
		uint32_t uncompressedSize; //if(header.format & 0b00100000)
		uint64_t name; //only if nameOffset exists per its conditions, will be located at the offset
		char* bytes; //supposed to be an array the size of compressedSize

		//generated
		//uint8_t flags; not needed as we're always little endian
	};

	class BND3 
	{
		public:
		_BND3_Header_ header;
		std::unique_ptr<_BND3_File_[]> files; //array of files, size set on class init


		BND3(FILE* file, uint64_t offset)
		{
			init(file, offset);
		};

		BND3(std::string path)
		{
			FILE* ptr = fopen(path.c_str(),"rb");
			init(ptr, 0);
		};

		bool validateHeader()
		{
			assert(strncmp("BND3",header.magic,4));

			return true;
		};

		private:
		void init(FILE* file, uint64_t offset)
		{
			FileReader fr = FileReader(file,offset);

			fr.readDouble(&header.magic[0]);
			fr.readQuad(&header.version[0]);
			//fr.readByte();
		};
	};
};

#endif