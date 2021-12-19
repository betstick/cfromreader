#pragma once
#include "stdafx.hpp"

//standard fromsoft model format from DSPTDE onwards
//this is ONLY flver2 type files. flver0 is old and grody
//as usual, built using SoulsTemplates! :)
//to help shorten this for now, it won't support non souls games

namespace cfr {

	struct _FLVER_Header_
	{
		char magic[4];
		char endian[2];
		uint32_t version;

		uint32_t dataOffset;
		uint32_t dataLength;

		int32_t dummyCount;
		int32_t materialCount;
		int32_t boneCount;
		int32_t meshCount;
		int32_t vertexBufferCount;

		Vector3 boundingBoxMin;
		Vector3 boundingBoxMax;

		int32_t trueFaceCount; // Does not include shadow meshes or degenerate faces
		int32_t totalFaceCount;

		char vertexIndexSize;
		char unicode;

		char unk4A;
		char unk4B;

		int32_t primitiveRestartConstant; //guess, needs confirmation
		int32_t faceSetCount;
		int32_t bufferLayoutCount;
		int32_t textureCount;

		char unk5C;
		char unk5D;
		char unk5E; //assert(0)
		char unk5F; //assert(0)

		int32_t unk60; //assert(0)
		int32_t unk64; //assert(0)
		int32_t unk68;
		int32_t unk6C; //assert(0)
		int32_t unk70; //assert(0)
		int32_t unk74; //assert(0)
		int32_t unk78; //assert(0)
		int32_t unk7C; //assert(0)
	};

	class FLVER
	{
		private:
		_FLVER_Header_ header;
		char* data;
		
		public:
		uint64_t size;

		public:
		FLVER(BSReader* file, uint64_t offset)
		{
			//init header
			file->read(&header,sizeof(_FLVER_Header_));
			uint64_t flverStart = file->position;
			//generate filesize
			size = header.dataLength + header.dataOffset;

			data = new char[size]; //init the array on the heap
			file->seek(flverStart,0); //reset to start to prevent oversize
			file->read(&data,size); //load in the entire file
		};

		//see how long this takes in real time
		/*Vector3 getVertices()
		{

		};*/
	};
};