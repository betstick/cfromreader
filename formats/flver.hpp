#pragma once
#include "stdafx.hpp"

//standard fromsoft model format from DSPTDE onwards
//this is ONLY flver2 type files. flver0 is old and grody
//as usual, built using SoulsTemplates! :)
//to help shorten this for now, it won't support non souls games

namespace cfr {
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
			file->seek(offset,0);
			file->read(&magic[0],38); //magic thru vertexBufferCount
			file->read(&boundingBoxMin, 24); //both boundingBox vars
			file->read(&trueFaceCount, 64); //trueFactCount thru unk7C
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
			file->seek(offset,0);
			file->read(&position, sizeof(FLVER_Dummy));
		};
	};

	class FLVER_GXItem
	{
		public:
		uint32_t id;
		int32_t unk04; //maybe assert(100)? if header.version < 0x20010
		int32_t length;
		std::vector<char> data; //size of (length - 0xC)

		FLVER_GXItem(){};

		//TODO: validate this mess
		FLVER_GXItem(BSReader* file, uint64_t offset)
		{
			file->seek(offset,0);
			file->read(&id,12);

			if(length > 0xC)
			{
				data.resize(length-0xC,0);
				file->read(&data,length-0xC);
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

		std::vector<FLVER_GXItem> items;

		FLVER_Material(){};

		FLVER_Material(BSReader* file, uint64_t offset, uint32_t version)
		{
			file->read(&name.offset,4);
			//file->read(&name.string,getStringSize(file)); //TODO: validate
			file->read(&mtd.offset,4);
			//file->read(&mtd.string,getStringSize(file)); //TODO: VALIDATE
			file->read(&textureCount,24); //texCount thru unk1C

			if(gxOffset != 0)
			{
				uint64_t initialPos = file->position;
				
				file->seek(gxOffset,0);
				bool valid = true;

				//TODO: this loop sucks, clean it up somehow
				while(valid)
				{
					FLVER_GXItem gxItem = FLVER_GXItem(file,0);

					if(version >= 0x20010 && gxItem.id != 0x7FFFFFFF && gxItem.id != 0xFFFFFFFF)
						valid = true;
					else
						valid = false;

					items.push_back(gxItem);
				}

				file->seek(initialPos,0);
			}
		};
	};

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
		//next 13 or so bytes should be 0 i think?

		FLVER_Bone(){};

		FLVER_Bone(BSReader* file, uint64_t offset)
		{
			file->read(&translation,12);
			file->read(&name.offset,4);
			//file->read(&name.string,getStringSize(file)); //TODO: validate
			file->read(&rotation,60); //TODO: confirm
			file->read(nullptr,13); //TODO: confirm
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
		std::vector<int32_t> boneIndices; //size of boneCount

		std::vector<int32_t> faceSetIndices; //size of faceSetCount
		std::vector<int32_t> vertexBufferIndices; //size of vertexBufferCount

		FLVER_Mesh(){};

		FLVER_Mesh(BSReader* file, uint64_t offset, uint32_t version)
		{
			file->read(&dynamic,48);

			uint64_t initialPos = file->position;

			if(boundingBoxOffset != 0)
			{
				file->seek(boundingBoxOffset,0);
				file->read(&boundingBoxMin,24);
				if(version >= 0x2001A)
					file->read(&boundingBoxUnk,12);
			}

			if(boneCount > 0)
			{
				file->seek(boneIndicesOffset,0);
				boneIndices.resize(boneCount,0);
				file->read(&boneIndices,boneCount*4);
			}
			
			file->seek(faceSetIndicesOffset,0);
			faceSetIndices.resize(faceSetCount,0);
			file->read(&faceSetIndices,faceSetCount*4);

			file->seek(vertexBufferIndicesOffset,0);
			vertexBufferIndices.resize(vertexBufferCount,0);
			file->read(&vertexBufferIndices,vertexBufferCount*4);

			file->seek(initialPos,0);
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

		std::vector<uint8_t> data; //size of dataLength

		FLVER_Member(){}; //needed for FLVER_EdgeIndices constructor

		FLVER_Member(BSReader* file, uint64_t offset, uint64_t start)
		{
			file->read(&dataLength,62);
			uint64_t initialPosition = file->position;

			file->seek(start+dataOffset,0);
			data.resize(dataLength,0);
			file->read(&data,dataLength);

			file->seek(initialPosition,0);
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
		std::vector<FLVER_Member> members; //size of memberCount

		FLVER_EdgeIndices(){}; //needed for FLVER_FaceSet constructor

		FLVER_EdgeIndices(BSReader* file, uint64_t offset)
		{
			uint64_t initPos = file->position;
			file->read(&memberCount,16);
			members.resize(memberCount);
			for(uint32_t i = 0; i < memberCount; i++)
			{
				members.push_back(FLVER_Member(file,0,initPos));
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
		std::vector<uint32_t> vertexIndicesVec; //size of vertexIndexCount

		FLVER_FaceSet(){};

		FLVER_FaceSet(BSReader* file, uint64_t offset, FLVER_Header header)
		{
			file->read(&flags,16);
			
			if(header.version > 0x20009)
				file->read(&vertexIndicesLength,16);
			
			uint32_t actualVertexIndexSize = header.vertexIndexSize;

			if(actualVertexIndexSize == 0)
				actualVertexIndexSize = vertexIndexSize;

			uint64_t initPos = file->position;
			file->seek(header.dataOffset+vertexIndicesOffset,0);

			if(actualVertexIndexSize == 8)
			{
				vertexIndices = FLVER_EdgeIndices(file,0);
			}
			else
			{
				vertexIndicesVec.resize(vertexIndexCount,0);
				file->read(&vertexIndicesVec,vertexIndexCount*4);
			}

			file->seek(initPos,0);
		};
	};

	class FLVER_Vertex
	{
		public:
		std::vector<char> data; //size of size input

		FLVER_Vertex(){}; //keep this just in case its needed

		FLVER_Vertex(BSReader* file, uint64_t offset, int32_t size)
		{
			data.resize(size,0);
			file->read(&data,size);
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

		std::vector<FLVER_Vertex> vertices; //size of vertexCount

		FLVER_VertexBuffer(){};

		FLVER_VertexBuffer(BSReader* file, uint64_t offset, FLVER_Header header)
		{
			file->read(&bufferIndex,32); //bufferIndex thru bufferOffset
			vertices.resize(vertexCount);

			if(unk10 == 0 && unk14 == 0)
			{
				uint64_t initPos = file->position;
				file->seek(header.dataOffset+bufferOffset,0);
				//this should be faster than looping and dynamically initing the vertexs
				//should still be correct :^)
				//TODO: verify that this is correct
				file->read(&vertices,vertexCount*vertexSize);
				file->seek(initPos,0);
			}
			//TODO: implement a backup using a loop to generate the vertex objects
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

		FLVER_LayoutMember(BSReader* file, uint64_t offset)
		{
			file->read(&unk00,20);
		};
	};

	class FLVER_BufferLayout
	{
		public:
		int32_t memberCount;

		int32_t unk04; //assert(0)
		int32_t unk08; //assert(0)

		uint32_t membersOffset;
		std::vector<FLVER_LayoutMember> members; //size of memberCount

		FLVER_BufferLayout(){};

		FLVER_BufferLayout(BSReader* file, uint64_t offset)
		{
			file->read(&memberCount,32);
			members.resize(memberCount);
			uint64_t initPos = file->position;
			file->seek(membersOffset,0);
			file->read(&members,memberCount*sizeof(FLVER_LayoutMember));
			file->seek(initPos,0);
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

		float unk14;
		float unk18;
		float unk1C;

		FLVER_Texture(){};

		FLVER_Texture(BSReader* file, uint64_t offset)
		{
			file->read(&path,40);
		};
	};

	class FLVER
	{
		public:
		FLVER_Header header;

		std::vector<FLVER_Dummy> dummies;
		std::vector<FLVER_Material> materials;
		std::vector<FLVER_Bone> bones;
		std::vector<FLVER_Mesh> meshes;
		std::vector<FLVER_FaceSet> facesets;
		std::vector<FLVER_VertexBuffer> vertexbuffers;
		std::vector<FLVER_BufferLayout> bufferlayouts;
		std::vector<FLVER_Texture> textures;

		//the offset should actually be used for this one :^)
		FLVER(BSReader* file, uint64_t offset)
		{
			init(file,offset);
		};

		FLVER(std::string path)
		{
			printf("this buffer is WAY too big! 131072KB (128MB) FIX IT\n");
			char buffer[131072];
			BSReader file = BSReader(path,buffer,131072);
			init(&file,0); //this one is differet, its okay
		};

		private:
		void init(BSReader* file, uint64_t offset)
		{
			header = FLVER_Header(file,offset);
			
			dummies.resize(header.dummyCount);
			for(uint32_t i = 0; i < header.dummyCount; i++)
				dummies.push_back(FLVER_Dummy(file,offset));

			materials.resize(header.materialCount);
			for(uint32_t i = 0; i < header.materialCount; i++)
				materials.push_back(FLVER_Material(file,offset,header.version));

			bones.resize(header.boneCount);
			for(uint32_t i = 0; i < header.boneCount; i++)
				bones.push_back(FLVER_Bone(file,offset));
					
			meshes.resize(header.meshCount);
			for(uint32_t i = 0; i < header.meshCount; i++)
				meshes.push_back(FLVER_Mesh(file,offset,header.version));
					
			facesets.resize(header.faceSetCount);
			for(uint32_t i = 0; i < header.faceSetCount; i++)
				facesets.push_back(FLVER_FaceSet(file,offset,header));
						
			vertexbuffers.resize(header.vertexBufferCount);
			for(uint32_t i = 0; i < header.vertexBufferCount; i++)
				vertexbuffers.push_back(FLVER_VertexBuffer(file,offset,header));
			
			bufferlayouts.resize(header.bufferLayoutCount);
			for(uint32_t i = 0; i < header.bufferLayoutCount; i++)
				bufferlayouts.push_back(FLVER_BufferLayout(file,offset));
			
			textures.resize(header.textureCount);
			for(uint32_t i = 0; i < header.textureCount; i++)
				textures.push_back(FLVER_Texture(file,offset));
		};
	};
};