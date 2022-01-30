#pragma once
#include "../stdafx.hpp"

namespace cfr
{
	struct BND3
	{
		struct Header
		{
			char magic[4];
			char version[8];
			uint8_t rawFarmat;
			uint8_t bigEndian;
			uint8_t bitBigEndian;
			char unk0F;
			int32_t fileCount;
			int32_t fileHeadersEnd;
			int32_t unk18;
			int32_t unk1C; //assert 0
		};

		struct File
		{
			//this part is always filled
			struct Header
			{
				char rawFlags;
				char unk01;
				char unk02;
				char unk03;

				int32_t compressedSize;
				uint32_t dataOffset;
			};

			Header* header;

			//circumstantially fill these
			int32_t* id = NULL;
			int32_t* nameOffset = NULL;
			int32_t* uncompressedSize = NULL;

			char** data;
		};

		Header* header;
		File* files;
	};

	//Init array of BND3 from current pos in file, count is set to array size
	BND3* openBnd3(FILE* src, int* count);

	//Init BND3 via current pos in MEM
	BND3* openBnd3(MEM* src);

	//Returns array of formats for a BND3
	//CfrFileType* getBnd3FileTypes(BND3* bnd);
};