#pragma once
#include "stdafx.hpp"

namespace cfr 
{
	class FLVER_Header
	{
		public:
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

		FLVER_Header(){};

		FLVER_Header(BSReader* file, uint64_t offset)
		{
			file->read(&magic[0],sizeof(FLVER_Header)); //i refuse to read one at a time!
		};
	};

	class FLVER_Dummy
	{
		public:
		Vector3 position;
		uint8_t color[4];
		Vector3 forward;
		int16_t referenceID;
		int16_t dummyBoneIndex;
		Vector3 upward;
		int16_t attachBoneIndex;
		char unk2E;
		char useUpwardVector;

		int32_t unk30;
		int32_t unk34;
		int32_t unk38; //assert(0)
		int32_t unk3C; //assert(0)

		FLVER_Dummy(){};

		FLVER_Dummy(BSReader* file, uint64_t offset)
		{
			//file->seek(offset);
			file->read(&position, sizeof(FLVER_Dummy));
		};
	};

	class FLVER_GXItem
	{
		public:
		uint32_t id;
		int32_t unk04; //maybe assert(100)? if header.version < 0x20010
		int32_t length;
		char* data; //size of (length - 0xC)

		FLVER_GXItem(){};

		FLVER_GXItem(BSReader* file, uint64_t offset)
		{
			//file->seek(offset);
			file->read(&id,12); //id thru length
			printf("length:%i\n",length);

			if(length > 0xC)
			{
				data = new char[length-0xC];

				for(uint32_t i = 0; i < length-0xC; i++)
				{
					file->read(&data+i,1);
				}
			}
		};
	};

	class FLVER_Material
	{
		public:
		OffsetString name; //offsets to the actual values?
		OffsetString mtd; 

		int32_t textureCount;
		int32_t textureIndex;

		uint32_t flags;
		uint32_t gxOffset;

		int32_t unk18;
		int32_t unk1C; //assert(0)

		FLVER_GXItem* items;

		FLVER_Material(){};

		FLVER_Material(BSReader* file, uint64_t offset, FLVER_Header header)
		{
			file->read(&name.offset,4);
			file->read(&mtd.offset,4);
			file->read(&textureCount,24); //texCount thru unk1C

			if(gxOffset != 0)
			{
				file->stepIn(gxOffset);
				printf("gxOffset:%i\treadPos:%i\n",gxOffset,file->readPos);
				printf("textureCount:%i\textureIndex:%i\n",textureCount,textureIndex);

				uint32_t i = 0;
				bool valid = true;

				while(valid)
				{
					items[i] = FLVER_GXItem(file,offset);

					if(header.version >= 0x20010 && items[i].id != 0x7FFFFFFF && items[i].id != 0xFFFFFFFF)
						valid = true;
					else
						valid = false;

					i++;
				}

				file->stepOut();
			}
			else
			{
				printf("gxOffset: %i\n",gxOffset);
			}
		}
	};

	//bonezone :^)
	class FLVER_Bone
	{
		public:
		Vector3 translation;
		OffsetString name;
		Vector3 rotation;
		int16_t parentIndex;
		int16_t childIndex;
		Vector3 scale;
		int16_t nextSiblingIndex;
		int16_t previousSiblingIndex;
		Vector3 boundingBoxMin;
		int32_t unk3C;
		Vector3 boundingBoxMax;

		FLVER_Bone(){};

		FLVER_Bone(BSReader* file, uint64_t offset)
		{
			file->read(&translation.x,4);
			file->read(&translation.y,4);
			file->read(&translation.z,4);
			file->read(&name.offset,4);
			//file->read(&name.string,getStringSize(file)); //TODO: validate
			file->read(&rotation.x,4);
			file->read(&rotation.y,4);
			file->read(&rotation.z,4);
			file->read(&parentIndex,2);
			file->read(&childIndex,2);
			file->read(&scale.x,4);
			file->read(&scale.y,4);
			file->read(&scale.z,4);
			file->read(&nextSiblingIndex,2);
			file->read(&previousSiblingIndex,2);
			file->read(&boundingBoxMin.x,4);
			file->read(&boundingBoxMin.y,4);
			file->read(&boundingBoxMin.z,4);
			file->read(&unk3C,4);
			file->read(&boundingBoxMax.x,4);
			file->read(&boundingBoxMax.y,4);
			file->read(&boundingBoxMax.z,4);
			//i have reason to believe that this is the end
		};
	};

	class FLVER_Mesh
	{
		bool dynamic; 
		
		char unk01; //assert(0)
		char unk02; //assert(0)
		char unk03; //assert(0)
		
		int32_t materialIndex;
		
		int32_t unk08; //assert(0)
		int32_t unk0C; //assert(0)
		
		int32_t defaultBoneIndex;
		int32_t boneCount;
		uint32_t boundingBoxOffset;
		uint32_t boneIndicesOffset;
		
		int32_t faceSetCount;
		uint32_t faceSetIndicesOffset;

		int32_t vertexBufferCount;
		uint32_t vertexBufferIndicesOffset;

		//only if boundingBoxOffset != 0
		Vector3 boundingBoxMin;
		Vector3 boundingBoxMax;
		Vector3 boundingBoxUnk; //only if header.version >= 0x2001A

		//only if boneCount > 0
		int32_t* boneIndices; //size of boneCount
		int32_t* faceSetIndices; //size of faceSetCount
		int32_t* vertexBufferIndices; //size of vertexBufferCount
	};

	//quad start??? position i think, maybe use offset?
	class FLVER_Member
	{
		public:
		int32_t dataLength;
		int32_t dataOffset;

		int32_t unk08; //assert(0)
		int32_t unk0C; //assert(0)
		int16_t unk10;
		int16_t unk12;

		int16_t baseIndex;

		int16_t unk16;
		int32_t unk18;
		int32_t unk1C;
		int32_t unk20; //assert(0)
		int32_t unk24; //assert(0)
		int32_t unk28; //assert(0)
		int32_t unk2C; //assert(0)

		//edgeGeomSpuConfigInfo? not sure what this means
		int16_t unk30;
		int16_t unk32;
		int16_t unk34;
		int16_t unk36;
		int16_t unk38;

		int16_t indexCount;

		int32_t unk3C; //assert(-1)

		uint8_t* data; //size of dataLength
	};

	class FLVER_EdgeIndices
	{
		int16_t memberCount;
		int16_t unk02;
		int32_t unk04;
		int32_t unk08; //assert(0)
		int32_t unk0C;
		FLVER_Member* members; //size of memberCount
	};

	class FLVER_FaceSet
	{
		uint32_t flags;
		char triangleStrip;
		char cullBackFaces;
		char unk06;
		char unk07;
		int32_t vertexIndexCount;
		uint32_t vertexIndicesOffset;

		//only if header.version >= 0x20009
		int32_t vertexIndicesLength;
		int32_t unk14; //assert(0)
		int32_t vertexIndexSize;
		int32_t unk1C; //assert(0)

		//these have very specific conditions to be filled
		FLVER_EdgeIndices vertexIndices;
		uint32_t* vertexIndicesVec; //size of vertexIndexCount
	};

	class FLVER_Vertex
	{
		char* data; //size of size input
	};

	class FLVER_VertexBuffer
	{
		int32_t bufferIndex;
		int32_t layoutIndex;
		int32_t vertexSize;
		int32_t vertexCount;

		int32_t unk10; //assert(0)
		int32_t unk14; //assert(0)

		uint32_t verticesLength; //0 in version 20005, non 0 in 20008
		uint32_t bufferOffset;

		FLVER_Vertex* vertices; //size of vertexCount
	};

	class FLVER_LayoutMember
	{
		int32_t unk00;
		int32_t structOffset;
		uint32_t type;
		uint32_t semantic;
		int32_t index;
	};

	class FLVER_BufferLayout
	{
		int32_t memberCount;

		int32_t unk04; //assert(0)
		int32_t unk08; //assert(0)

		uint32_t membersOffset;
		FLVER_LayoutMember* members; //size of memberCount
	};

	class FLVER_Texture
	{
		OffsetString path;
		OffsetString type;

		Vector2 scale;

		char unk10;
		char unk11;
		int16_t unk12; //assert(0)

		float unk14;
		float unk18;
		float unk1C;
	};
};