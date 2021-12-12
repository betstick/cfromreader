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
		char name[256]; //only if nameOffset exists per its conditions, located at the offset
		char* bytes; //supposed to be an array the size of compressedSize, may not be needed
	};

	class BND3 
	{
		public:
		_BND3_Header_ header;
		std::vector<_BND3_File_> fileHeaders; //array of files, size set on class init
		uint64_t offset = 0; //inherited from parent file (if there is one)

		//this initializer is only for use by other classes.
		BND3(BSReader* file, uint64_t priorOffset)
		{
			//prior offset should be gotten from the file this file came from
			//so that we get the net offset regarding the larger scope of files
			//this comment probably makes no sense.
			file->seek(offset,0);
			offset += priorOffset;
			init(file, offset);
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
		void init(BSReader* file, uint64_t offset)
		{
			initBinderHeader(file, 0);

			for(uint64_t i = 0; i < header.fileCount; i++)
			{
				initFileHeaders(file, offset);
			}
#ifdef DEBUG
			printf("If you can see this message, all BND3 checks passed!\n");
#endif
			initSubFiles(file, offset);
		};

		void initBinderHeader(BSReader* file, uint64_t offset)
		{
			//can't just map ALL the bytes from the file
			//because i'm not packing my structs!
			//longer to load into mem, but faster to use
			file->read(&header.magic,4); //can't remove idk why
			file->read(&header.version,8);
			file->read(&header.rawFormat,1);
			file->read(&header.bigEndian,1);
			file->read(&header.bitBigEndian,1);
			file->read(&header.unk0F,1);
			file->read(&header.fileCount,4);
			file->read(&header.fileHeadersEnd,4);
			file->read(&header.unk18,4);
			file->read(&header.unk1C,4);

#ifdef DEBUG
			validateBinderHeader();
#endif	
		};

		void initFileHeaders(BSReader* file, uint64_t offset)
		{
			_BND3_File_ bndFile;
			file->read(&bndFile.rawFlags,1);
			file->read(&bndFile.unk01,1);
			file->read(&bndFile.unk02,1);
			file->read(&bndFile.unk03,1);
			file->read(&bndFile.compressedSize,4);
			file->read(&bndFile.dataOffset,4);
			//printf("headerFormat: %08i\n", byteToBinary(header.rawFormat));
			
			//all of these binary comparisons had to have the numbers inverted >:(
			//better to invert them this way than to make to CPU do it manually
			if(header.rawFormat & 0b01000000)
				file->read(&bndFile.id,4);
				
			if(header.rawFormat & 0b00100000 | header.rawFormat & 0b00010000)
				file->read(&bndFile.nameOffset,4);

			if(header.rawFormat & 0b00000100)
				file->read(&bndFile.uncompressedSize,4);

			if(bndFile.nameOffset > 0)
			{
				/*fpos64_t position;
				fgetpos64(file, &position);*/
				int64_t pos = file->position;

				file->seek(bndFile.nameOffset,0);

				int32_t i = 0;
				//this while condition is gross but it works. idk why
				while(bndFile.name[i-1] != 0 || i == 0)
				{
					file->read(&bndFile.name[i],1);
					//printf("%c",bndFile.name[i]);
					i++;
				}
				//printf("\n");

				/*fsetpos64(file, &position);*/
				file->seek(pos,0);
			}
#ifdef DEBUG
			validateFileHeader(bndFile);
#endif
			fileHeaders.push_back(bndFile);
		};
	
		void initSubFiles(BSReader* file, uint64_t offset)
		{

		};
	};
};

//#endif