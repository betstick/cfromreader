#include "bnd3.hpp"

namespace cfr
{
	/*BND3File* openBnd3(MEM* src, int* count)
	{
		unsigned int startPos = mtell(src);

		char magic[4];
		mread(magic,4,1,src);
		if(memcmp(magic,"BND3",4) != 0){return NULL;}

		mseek(src,8,SEEK_CUR); //skip version, not needed

		uint8_t rawFormat;
		mread(&rawFormat,1,1,src);

		mseek(src,3,SEEK_CUR);

		int32_t fileCount;
		mread(&fileCount,4,1,src);
		*count = fileCount; //copy to count pointer

		mseek(src,12,SEEK_CUR); //skip to end of BND3 header

		BND3File* fileArr = (BND3File*)malloc(fileCount*sizeof(BND3File));

		//iterate thru and generate all the file structs for arr
		for(int i = 0; i < fileCount; i++)
		{
			//file headers are 24 bytes total
			uint8_t rawFlags;
			mread(&rawFlags,1,1,src);

			mseek(src,3,SEEK_CUR);

			int32_t compressedSize;
			mread(&compressedSize,4,1,src);

			uint32_t dataOffset;
			mread(&dataOffset,4,1,src);

			int32_t id = -1;
			if(rawFormat & 0b01000000)
				mread(&id,4,1,src);

			//check if nameOffset needs to be skipped
			if((rawFormat & 0b00100000) | (rawFormat & 0b00010000))
				mseek(src,4,SEEK_CUR);

			int32_t uncompressedSize = 0;
			if(rawFormat & 0b00000100)
				mread(&uncompressedSize,4,1,src);

			//step into the file to figure out its type
			int pos = mtell(src);
			mseek(src,dataOffset,SEEK_SET);

			char magic[8];
			mread(magic,8,1,src);

			mseek(src,pos,SEEK_SET);

			//get whichever is larger, shouldn't matter *too* much.
			int totalSize = uncompressedSize > compressedSize ? uncompressedSize : compressedSize;
			fileArr[i] = {src,dataOffset,totalSize,id,determineFormat(magic)};
		}

		mseek(src,startPos,SEEK_SET); //return to start for cleanliness
		return fileArr;
	};

	BND3File* openBnd3(const char* path, int* count)
	{
		FILE* src = fopen(path,"rb");
		
		fclose(src);
		return openBnd3(src,count);
	};*/

	BND3* openBnd3(MEM* src)
	{
		BND3* bnd = (BND3*)malloc(sizeof(BND3));

		bnd->header = (BND3::Header*)mtellptr(src);
		mseek(src,sizeof(BND3::Header),SEEK_CUR);

		bnd->files = (BND3::File*)malloc(sizeof(BND3::File)*bnd->header->fileCount);

		for(int i = 0; i < bnd->header->fileCount; i++)
		{
			bnd->files[i].header = (BND3::File::Header*)mtellptr(src);
			mseek(src,sizeof(BND3::File::Header),SEEK_CUR);
			
			if(bnd->header->rawFarmat & 0b01000000)
			{
				bnd->files[i].id = (int32_t*)mtellptr(src);
				mseek(src,4,SEEK_CUR);
			}

			if((bnd->header->rawFarmat & 0b00100000) | (bnd->header->rawFarmat & 0b00010000))
			{
				bnd->files[i].nameOffset = (int32_t*)mtellptr(src) + mtell(src);
				mseek(src,4,SEEK_CUR);
			}

			if(bnd->header->rawFarmat & 0b00000100)
			{
				bnd->files[i].uncompressedSize = (int32_t*)mtellptr(src);
				mseek(src,4,SEEK_CUR);
			}

			//bnd->files[i].data = (char**)mtellptr(src) + mtell(src);
			bnd->files[i].data = (char**)(bnd->files[i].header->dataOffset+src->src);

			//don't seek the memory isn't here!
			//mseek(src,*bnd->files[i].uncompressedSize,SEEK_CUR);
		}

		return bnd;
	};

};