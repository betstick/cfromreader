#pragma once
#include "../stdafx.hpp"
#include "flver2_structs.hpp"

namespace cfr
{
	//all this data needs to be collected anyways,
	//so just get it out of the way once.
	struct FLVER2_Info
	{
		FILE* parent;

		uint version; //is uint legal?

		int dummyCount;
		int dummyStartOffset;

		int materialCount;
		int materialStartOffset;

		int boneCount;
		int boneStartOffset;

		int meshCount;
		int meshStartOffset;

		int faceSetCount;
		int faceSetStartOffset;

		int vertexBufferCount;
		int vertexBufferStartOffset;

		int bufferLayoutCount;
		int bufferLayoutStartOffset;

		int textureCount;
		int textureStartOffset;
	};

	FLVER2_Info openFLVER2(FILE* src)
	{
		int startPos = ftell(src);

		char magic[6];
		fread(magic,6,1,src);
		assert(memcmp(magic,"FLVER\0",6) != 0); //can't return null

		fseek(src,2,SEEK_CUR); //skip endian, lol

		uint32_t version;
		fread(&version,4,1,src);

		uint32_t dataOffset;
		fread(&dataOffset,4,1,src);

		uint32_t dataLength;
		fread(&dataLength,4,1,src);

		int32_t dummyCount;
		int32_t materialCount;
		int32_t boneCount;
		int32_t meshCount;
		int32_t vertexBufferCount;
	};
};