/*#ifndef BND3_FORMAT
#define BND3_FORMAT*/

#pragma once
#include "stdafx.hpp"

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
		int32_t id; //if(header.format & 0b00000010)//???
		int32_t nameOffset; //if(header.format & 0b00000100 | header.format & 0b00001000)
		int32_t uncompressedSize; //if(header.format & 0b00100000)
		char name[256]; //only if nameOffset exists per its conditions, will be located at the offset
		char* bytes; //supposed to be an array the size of compressedSize, may not be needed
	};

	class BND3 
	{
		public:
		_BND3_Header_ header;
		std::vector<_BND3_File_> files; //array of files, size set on class init

		BND3(FILE* file, uint64_t offset)
		{
			fseek(file,offset,0);
			initBinderHeader(file, offset);
		};

		BND3(std::string path)
		{
			FILE* ptr = fopen(path.c_str(),"rb");
			/*if(ptr == NULL)
				throw std::runtime_error("E");*/
			initBinderHeader(ptr, 0);
		};

		bool validateBinderHeader()
		{
			assert(strncmp("BND3",header.magic,5)); //5th char is NUL
			assert(header.unk0F == 0);
			assert(header.unk1C == 0);
			assert(header.bigEndian == 0);
			assert(header.bitBigEndian == 0);

			//printf("bigEndian: %i\n",byteToBinary(header.bigEndian));
			//printf("bitBigEndian: %i\n",byteToBinary(header.bitBigEndian));
			return true;
		};

		bool validateFileHeader(_BND3_File_ file)
		{
			assert(file.unk01 == 0);
			assert(file.unk02 == 0);
			assert(file.unk03 == 0);
			//printf("File header passed validation.\n");
			return true;
		};

		private:
		void initBinderHeader(FILE* file, uint64_t offset)
		{
			//can't just map ALL the bytes from the file
			//because i'm not packing my structs!
			//longer to load into mem, but faster to use
			fread(&header.magic,4,1,file);
			fread(&header.version,8,1,file);
			fread(&header.rawFormat,1,1,file);
			fread(&header.bigEndian,1,1,file);
			fread(&header.bitBigEndian,1,1,file);
			fread(&header.unk0F,1,1,file);
			fread(&header.fileCount,4,1,file);
			fread(&header.fileHeadersEnd,4,1,file);
			fread(&header.unk18,4,1,file);
			fread(&header.unk1C,4,1,file);

#ifdef DEBUG
			validateBinderHeader();
#endif

			for(uint64_t i = 0; i < header.fileCount; i++)
			{
				initFileHeaders(file, offset);
			}
#ifdef DEBUG
			printf("If you can see this message, all BND3 checks passed!\n");
#endif			
		};

		void initFileHeaders(FILE* file, uint64_t offset)
		{
			_BND3_File_ bndFile;
			fread(&bndFile.rawFlags,1,1,file);
			fread(&bndFile.unk01,1,1,file);
			fread(&bndFile.unk02,1,1,file);
			fread(&bndFile.unk03,1,1,file);
			fread(&bndFile.compressedSize,4,1,file);
			fread(&bndFile.dataOffset,4,1,file);
			//printf("headerFormat: %08i\n", byteToBinary(header.rawFormat));
			
			//all of these binary comparisons had to have the numbers inverted >:(
			if(header.rawFormat & 0b01000000)
				fread(&bndFile.id,4,1,file);
				
			if(header.rawFormat & 0b00100000 | header.rawFormat & 0b00010000)
				fread(&bndFile.nameOffset,4,1,file);


			if(header.rawFormat & 0b00000100)
				fread(&bndFile.uncompressedSize,4,1,file);

			if(bndFile.nameOffset > 0)
			{
				fpos_t position;
				fgetpos(file, &position);

				fseek(file, bndFile.nameOffset, SEEK_SET);

				int32_t i = 0;
				//this while condition is gross but it works. idk why
				while(bndFile.name[i-1] != 0 || i == 0)
				{
					fread(&bndFile.name[i],1,1,file);
					//printf("%c",bndFile.name[i]);
					i++;
				}
				//printf("\n");

				fsetpos(file,&position);
			}
#ifdef DEBUG
			validateFileHeader(bndFile);
#endif
			files.push_back(bndFile);
		};
	};
};

//#endif