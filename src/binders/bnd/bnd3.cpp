#include "bnd3.hpp"

namespace cfr
{
	BND3File* openBnd3(FILE* src, int* count)
	{
		unsigned int startPos = ftell(src);

		char magic[4];
		fread(magic,4,1,src);
		if(memcmp(magic,"BND3",4) != 0){return NULL;}

		fseek(src,8,SEEK_CUR); //skip version, not needed

		uint8_t rawFormat;
		fread(&rawFormat,1,1,src);

		fseek(src,3,SEEK_CUR);

		int32_t fileCount;
		fread(&fileCount,4,1,src);
		*count = fileCount; //copy to count pointer

		fseek(src,12,SEEK_CUR); //skip to end of BND3 header

		BND3File* fileArr = (BND3File*)malloc(fileCount*sizeof(BND3File));

		//iterate thru and generate all the file structs for arr
		for(int i = 0; i < fileCount; i++)
		{
			//file headers are 24 bytes total
			uint8_t rawFlags;
			fread(&rawFlags,1,1,src);

			fseek(src,3,SEEK_CUR);

			int32_t compressedSize;
			fread(&compressedSize,4,1,src);

			uint32_t dataOffset;
			fread(&dataOffset,4,1,src);

			int32_t id = -1;
			if(rawFormat & 0b01000000)
				fread(&id,4,1,src);

			//check if nameOffset needs to be skipped
			if((rawFormat & 0b00100000) | (rawFormat & 0b00010000))
				fseek(src,4,SEEK_CUR);

			int32_t uncompressedSize = 0;
			if(rawFormat & 0b00000100)
				fread(&uncompressedSize,4,1,src);

			//step into the file to figure out its type
			int pos = ftell(src);
			fseek(src,dataOffset,SEEK_SET);

			char magic[8];
			fread(magic,8,1,src);

			fseek(src,pos,SEEK_SET);

			//get whichever is larger, shouldn't matter *too* much.
			int totalSize = uncompressedSize > compressedSize ? uncompressedSize : compressedSize;
			fileArr[i] = {src,dataOffset,totalSize,id,determineFormat(magic)};
		}

		fseek(src,startPos,SEEK_SET); //return to start for cleanliness
		return fileArr;
	};

	BND3File* openBnd3(const char* path, int* count)
	{
		FILE* src = fopen(path,"r");
		return openBnd3(src,count);
	};
};