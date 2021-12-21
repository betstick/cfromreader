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

		FLVER_Header(BSReader* file)//, uint64_t offset)
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

		FLVER_Dummy(BSReader* file)//, uint64_t offset)
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

		FLVER_GXItem(BSReader* file)//, uint64_t offset)
		{
			//file->seek(offset);
			file->read(&id,12); //id thru length
			printf("length:%i\n",length);

			if(length > 0xC)
			{
				data = new char[length-0xC];

				for(int32_t i = 0; i < length-0xC; i++)
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

		FLVER_Material(BSReader* file, FLVER_Header header)
		{
			//file->read(&name.offset,4);
			//file->read(&mtd.offset,4);
			name = readOffsetString(file);
			mtd = readOffsetString(file);

			file->read(&textureCount,24); //texCount thru unk1C

			if(gxOffset != 0)
			{
				file->stepIn(gxOffset);
				printf("gxOffset:%i\treadPos:%lu\n",gxOffset,file->readPos);
				printf("textureCount:%i\textureIndex:%i\n",textureCount,textureIndex);

				uint32_t i = 0;
				bool valid = true;

				while(valid)
				{
					items[i] = FLVER_GXItem(file);//,offset);

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
				//printf("gxOffset: %i\n",gxOffset);
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
		char* emptyJunk[52]; //potentially needed for spacing :/

		FLVER_Bone(){};

		FLVER_Bone(BSReader* file)//, uint64_t offset)
		{
			//this has to be read in one element at a time.
			//the vectors cause segfaults if you try to lump them together.
			file->read(&translation.x,4);
			file->read(&translation.y,4);
			file->read(&translation.z,4);
			//file->read(&name.offset,4);
			name = readOffsetString(file);
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
			//boundingBoxMin = readVec3(file); //function test
			file->read(&unk3C,4);
			file->read(&boundingBoxMax.x,4);
			file->read(&boundingBoxMax.y,4);
			file->read(&boundingBoxMax.z,4);
			file->read(&emptyJunk,13*4);
		};
	};

	class FLVER_Mesh
	{
		public:
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

		FLVER_Mesh(){};

		FLVER_Mesh(BSReader* file, FLVER_Header header)
		{
			//file->read(&dynamic,48); //dynamic thru vertexBufferIndicesOffset
			file->read(&dynamic,1);

			file->read(&unk01,1);
			file->read(&unk02,1);
			file->read(&unk03,1);

			file->read(&materialIndex,4);

			file->read(&unk08,4);
			file->read(&unk0C,4);

			file->read(&defaultBoneIndex,4);
			file->read(&boneCount,4);
			file->read(&boundingBoxOffset,4);
			file->read(&boneIndicesOffset,4);

			file->read(&faceSetCount,4);
			file->read(&faceSetIndicesOffset,4);

			file->read(&vertexBufferCount,4);
			file->read(&vertexBufferIndicesOffset,4);

			uint64_t start = file->readPos;

			if(boundingBoxOffset != 0)
			{
				file->seek(boundingBoxOffset);
				
				file->read(&boundingBoxMin.x,4);
				file->read(&boundingBoxMin.y,4);
				file->read(&boundingBoxMin.z,4);
				file->read(&boundingBoxMax.x,4);
				file->read(&boundingBoxMax.y,4);
				file->read(&boundingBoxMax.z,4);

				if(header.version >= 0x2001A)
				{
					file->read(&boundingBoxUnk.x,4);
					file->read(&boundingBoxUnk.y,4);
					file->read(&boundingBoxUnk.z,4);
				}
			}

			if(boneCount > 0)
			{
				file->seek(boneIndicesOffset);
				boneIndices = new int32_t[boneCount];
				for(int32_t i = 0; i < boneCount; i++)
				{
					file->read(&boneIndices[i],4);
				}
			}

			file->seek(faceSetIndicesOffset);
			faceSetIndices = new int32_t[faceSetCount];
			for(int32_t i = 0; i < faceSetCount; i++)
			{
				file->read(&faceSetIndices[i],4);
			}
			
			file->seek(vertexBufferIndicesOffset);
			vertexBufferIndices = new int32_t[vertexBufferCount];
			for(int32_t i = 0; i < vertexBufferCount; i++)
			{
				file->read(&vertexBufferIndices[i],4);
			}

			file->seek(start);
		};
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

		FLVER_Member(){}; //default

		FLVER_Member(BSReader* file, uint64_t start)
		{
			file->read(&dataLength,sizeof(FLVER_Member)-1); //dataLength thru unk3C
			
			file->markPos();

			file->seek(start+dataOffset);
			data = new uint8_t[dataLength];

			file->returnToMark();
		};
	};

	class FLVER_EdgeIndices
	{
		public:
		int16_t memberCount;
		int16_t unk02;
		int32_t unk04;
		int32_t unk08; //assert(0)
		int32_t unk0C;
		FLVER_Member* members; //size of memberCount

		FLVER_EdgeIndices(){};

		FLVER_EdgeIndices(BSReader* file)//, uint64_t offset)
		{
			uint64_t start = file->readPos;

			file->read(&memberCount,16); //memberCount thru unk0C

			members = new FLVER_Member[memberCount];

			for(int16_t i = 0; i < memberCount; i++)
			{
				members[i] = FLVER_Member(file,start);
			}
		};
	};

	class FLVER_FaceSet
	{
		public:
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
		uint32_t* vertexIndicesArr; //size of vertexIndexCount

		FLVER_FaceSet(){};

		FLVER_FaceSet(BSReader* file, FLVER_Header header)
		{
			file->read(&flags,16); //flags thru vertexIndicesOffset

			if(header.version >= 0x20009)
			{
				file->read(&vertexIndicesLength,16); //thru unk1C
			}

			int32_t actualVertexIndexSize = header.vertexIndexSize == 0 ? vertexIndexSize : header.vertexIndexSize;

			file->markPos();

			file->seek(header.dataOffset + vertexIndicesOffset);

			//TODO: improve this mess
			if(actualVertexIndexSize == 8)
			{
				vertexIndices = FLVER_EdgeIndices();
			}
			else if(actualVertexIndexSize == 16)
			{
				vertexIndicesArr = new uint32_t[vertexIndexCount];
			}
			else if(actualVertexIndexSize == 32)
			{
				vertexIndicesArr = new uint32_t[vertexIndexCount];
			}

			file->returnToMark();
		};
	};

	class FLVER_Vertex
	{
		public:
		char* data; //size of size input

		FLVER_Vertex(){};

		FLVER_Vertex(BSReader* file, int32_t size)
		{
			data = new char[size];
			file->read(&data[0],size); //TODO: make this work, it should
		};
	};

	class FLVER_VertexBuffer
	{
		public:
		int32_t bufferIndex;
		int32_t layoutIndex;
		int32_t vertexSize;
		int32_t vertexCount;

		int32_t unk10; //assert(0)
		int32_t unk14; //assert(0)

		uint32_t verticesLength; //0 in version 20005, non 0 in 20008
		uint32_t bufferOffset;

		FLVER_Vertex* vertices; //size of vertexCount

		FLVER_VertexBuffer(){};

		FLVER_VertexBuffer(BSReader* file, FLVER_Header header)
		{
			file->read(&bufferIndex,32); //bufferIndex thru bufferOffset

			//printf("vertexCount:%i\n",vertexCount);

			if((unk10 == 0) && (unk14 == 0))
			{
				file->markPos();
				file->seek(header.dataOffset + bufferOffset);

				vertices = new FLVER_Vertex[vertexCount];
				for(int32_t i = 0; i < vertexCount; i++)
				{
					//TODO: verify this works and is mem safe
					vertices[i] = FLVER_Vertex(file,vertexSize);
				}

				file->returnToMark();
			}
		};
	};

	class FLVER_LayoutMember
	{
		public:
		int32_t unk00;
		int32_t structOffset;
		uint32_t type;
		uint32_t semantic;
		int32_t index;

		FLVER_LayoutMember(){};

		FLVER_LayoutMember(BSReader* file)
		{
			file->read(&unk00,20); //EVERYTHING
		};
	};

	class FLVER_BufferLayout
	{
		public:
		int32_t memberCount;

		int32_t unk04; //assert(0)
		int32_t unk08; //assert(0)

		uint32_t membersOffset;
		FLVER_LayoutMember* members; //size of memberCount

		FLVER_BufferLayout(){};

		FLVER_BufferLayout(BSReader *file)
		{
			file->read(&memberCount,16);
			file->stepIn(membersOffset);
			members = new FLVER_LayoutMember[memberCount];
			for(int32_t i = 0; i < memberCount; i++)
			{
				members[i] = FLVER_LayoutMember();
			}
			file->stepOut();
		};
	};

	class FLVER_Texture
	{
		public:
		OffsetString path;
		OffsetString type;

		Vector2 scale;

		char unk10;
		char unk11;
		int16_t unk12; //assert(0)

		_Float32 unk14;
		_Float32 unk18;
		_Float32 unk1C;

		FLVER_Texture(){};

		FLVER_Texture(BSReader* file)
		{
			//file->read(&path.offset,4);
			//file->read(&type.offset,4);
			path = readOffsetString(file);
			type = readOffsetString(file);

			file->read(&scale.x,4);
			file->read(&scale.y,4);

			file->read(&unk10,16);
		};
	};
};