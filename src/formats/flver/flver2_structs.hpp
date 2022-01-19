#pragma once
#include "../stdafx.hpp"

namespace cfr 
{
	class FLVER2_Header
	{
		public:
		char magic[6]; //this was working with size 4 somehow. it shouldn't
		char endian[2];
		uint32_t version;

		uint32_t dataOffset = 0;
		uint32_t dataLength;

		int32_t dummyCount;
		int32_t materialCount;
		int32_t boneCount;
		int32_t meshCount;
		int32_t vertexBufferCount;

		float boundingBoxMin_x;
		float boundingBoxMin_y;
		float boundingBoxMin_z;

		float boundingBoxMax_x;
		float boundingBoxMax_y;
		float boundingBoxMax_z;

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

		FLVER2_Header(){};

		FLVER2_Header(FILE* src,int startOffset)
		{
			fread(this->magic,sizeof(FLVER2_Header),1,src);
			printf("headerEndPos: 0x%x\n",ftell(src)-startOffset);
		};
	};

	class FLVER2_Dummy
	{
		public:
		float pos_x;
		float pos_y;
		float pos_z;

		uint8_t color[4];

		float forward_x;
		float forward_y;
		float forward_z;

		int16_t referenceID;
		int16_t dummyBoneIndex;

		float upward_x;
		float upward_y;
		float upward_z;

		int16_t attachBoneIndex;
		char unk2E;
		char useUpwardVector;

		int32_t unk30;
		int32_t unk34;
		int32_t unk38; //assert(0)
		int32_t unk3C; //assert(0)

		FLVER2_Dummy(){};

		FLVER2_Dummy(FILE* src, int startOffset)
		{
			fread(&pos_x,sizeof(FLVER2_Dummy),1,src);
		};
	};

	class FLVER2_GXItem
	{
		public:
		uint32_t id;
		int32_t unk04; //maybe assert(100)? if header.version < 0x20010
		int32_t length;
		int dataStart;

		FLVER2_GXItem(){};

		FLVER2_GXItem(FILE* src)
		{
			fread(&id,12,1,src); //id thru length
			fseek(src,4,SEEK_CUR);

			if(length > 0xC)
			{
				dataStart = ftell(src);

				fseek(src,length,SEEK_CUR);
			}
		};
	};

	class FLVER2_Material
	{
		public:
		uint32_t nameOffset = 0;
		uint32_t mtdOffset = 0;

		int32_t textureCount;
		int32_t textureIndex;

		uint32_t flags;
		uint32_t gxOffset;

		int32_t unk18;
		int32_t unk1C; //assert(0)

		FLVER2_GXItem* items;

		std::string name;
		std::string mtd;

		FLVER2_Material(){};

		FLVER2_Material(FILE* src, uint64_t offset, FLVER2_Header header, int startOffset)
		{
			fread(&nameOffset,4,1,src);
			int pos = ftell(src);
			fseek(src,nameOffset+startOffset,SEEK_SET);
			//file->readString(&name); //deal with this later :/
			fseek(src,pos,SEEK_SET);

			fread(&mtdOffset,4,1,src);
			pos = ftell(src);
			fseek(src,mtdOffset+startOffset,SEEK_SET);
			//file->readString(&mtd); //deal with this later :/
			fseek(src,pos,SEEK_SET);

			fread(&textureCount,24,1,src); //texCount thru unk1C

			if(gxOffset != 0)
			{
				pos = ftell(src);
				fseek(src,gxOffset+startOffset,SEEK_SET);

				uint32_t i = 0;
				bool valid = true;

				//TODO: fix this disgusting *thing*
				while(valid)
				{
					items[i] = FLVER2_GXItem(src);

					if(header.version >= 0x20010 && items[i].id != 0x7FFFFFFF && items[i].id != 0xFFFFFFFF)
						valid = true;
					else
						valid = false;

					i++;
				}

				fseek(src,pos,SEEK_SET);
			}
		}
	};

	//bonezone :^)
	class FLVER2_Bone
	{
		public:
		float translation_x;
		float translation_y;
		float translation_z;

		uint64_t nameOffset;

		float rot_x;
		float rot_y;
		float rot_z;

		int16_t parentIndex;
		int16_t childIndex;

		float scale_x;
		float scale_y;
		float scale_z;

		int16_t nextSiblingIndex;
		int16_t previousSiblingIndex;

		float boundingBoxMin_x;
		float boundingBoxMin_y;
		float boundingBoxMin_z;
		
		int32_t unk3C;

		float boundingBoxMax_x;
		float boundingBoxMax_y;
		float boundingBoxMax_z;
		
		char* emptyJunk[52]; //potentially needed for spacing :/

		FLVER2_Bone(){};

		FLVER2_Bone(FILE* src, int startOffset)
		{
			fread(&translation_x,12,1,src);
			fread(&nameOffset,4,1,src); //must be separate!
			fread(&rot_x,60+52,1,src);
		};
	};

	class FLVER2_Mesh
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
		float boundingBoxMin_x;
		float boundingBoxMin_y;
		float boundingBoxMin_z;
		float boundingBoxMax_x;
		float boundingBoxMax_y;
		float boundingBoxMax_z;
		float boundingBoxUnk_x; //only if header.version >= 0x2001A
		float boundingBoxUnk_y; //only if header.version >= 0x2001A
		float boundingBoxUnk_z; //only if header.version >= 0x2001A

		//only if boneCount > 0
		int32_t* boneIndices; //size of boneCount
		int32_t* faceSetIndices; //size of faceSetCount
		int32_t* vertexBufferIndices; //size of vertexBufferCount

		FLVER2_Mesh(){};

		FLVER2_Mesh(FILE* src, FLVER2_Header header, int startOffset)
		{
			fread(&dynamic,48,1,src); //dynamic thru vertexBufferIndicesOffset

			int pos = ftell(src);

			if(boundingBoxOffset != 0)
			{
				fseek(src,boundingBoxOffset+startOffset,SEEK_SET);
				
				fread(&boundingBoxMin_x,12,1,src);

				if(header.version >= 0x2001A)
					fread(&boundingBoxUnk_x,12,1,src);
			}

			if(boneCount > 0)
			{
				fseek(src,boneIndicesOffset+startOffset,SEEK_SET);
				boneIndices = new int32_t[boneCount];

				for(int32_t i = 0; i < boneCount; i++)
					fread(&boneIndices[i],4,1,src);
			}

			fseek(src,faceSetIndicesOffset+startOffset,SEEK_SET);
			faceSetIndices = new int32_t[faceSetCount];

			for(int32_t i = 0; i < faceSetCount; i++)
				fread(&faceSetIndices[i],4,1,src);
			
			fseek(src,vertexBufferIndicesOffset+startOffset,SEEK_SET);
			vertexBufferIndices = new int32_t[vertexBufferCount];

			for(int32_t i = 0; i < vertexBufferCount; i++)
				fread(&vertexBufferIndices[i],4,1,src);

			fseek(src,pos,SEEK_SET);
		};
	};

	//quad start??? position i think, maybe use offset?
	class FLVER2_Member
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

		FLVER2_Member(){}; //default

		FLVER2_Member(FILE* src, uint64_t start, int startOffset)
		{
			fread(&dataLength,sizeof(FLVER2_Member)-1,1,src); //dataLength thru unk3C
			
			int pos = ftell(src);

			fseek(src,start+dataOffset+startOffset,SEEK_SET);
			data = new uint8_t[dataLength];

			fseek(src,pos,SEEK_SET);
		};
	};

	class FLVER2_EdgeIndices
	{
		public:
		int16_t memberCount;
		int16_t unk02;
		int32_t unk04;
		int32_t unk08; //assert(0)
		int32_t unk0C;
		FLVER2_Member* members; //size of memberCount

		FLVER2_EdgeIndices(){};

		FLVER2_EdgeIndices(FILE* src, int startOffset)
		{
			uint64_t start = ftell(src);

			fread(&memberCount,16,1,src); //memberCount thru unk0C

			members = new FLVER2_Member[memberCount];

			for(int16_t i = 0; i < memberCount; i++)
			{
				members[i] = FLVER2_Member(src,start,startOffset);
			}
		};
	};

	class FLVER2_FaceSet
	{
		public:
		uint32_t flags;
		bool triangleStrip;
		bool cullBackFaces;
		char unk06;
		char unk07;
		int32_t vertexIndexCount; //number of indices
		uint32_t vertexIndicesOffset;

		//only if header.version >= 0x20009
		int32_t vertexIndicesLength; //i don't know
		int32_t unk14; //assert(0)
		int32_t vertexIndexSize; //byte size of indices
		int32_t unk1C; //assert(0)

		uint32_t location;

		FLVER2_FaceSet(){};

		FLVER2_FaceSet(FILE* src, FLVER2_Header header)
		{
			fread(&flags,16,1,src); //flags thru vertexIndicesOffset

			if(header.version >= 0x20009)
				fread(&vertexIndicesLength,16,1,src); //thru unk1C

			//just make vertexIndexSize correct :/
			if(header.vertexIndexSize != 0)
				vertexIndexSize = header.vertexIndexSize;

			location = header.dataOffset + vertexIndicesOffset;
		};
	};

	class FLVER2_VertexBuffer
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

		uint64_t start; //position where the data starts
		uint64_t trueSize; //how big it is
		uint32_t location;

		FLVER2_VertexBuffer(){};

		FLVER2_VertexBuffer(FILE* src, FLVER2_Header header, int startOffset)
		{
			fread(&bufferIndex,32,1,src);

			if((unk10 == 0) && (unk14 == 0)) //bufferIndex thru bufferOffset
			{
				int pos = ftell(src);
				fseek(src,header.dataOffset + bufferOffset + startOffset,SEEK_SET);

				//size by doing the math manually
				uint32_t calcSize = vertexSize * vertexCount;
				//size the flver claims the buffer is
				uint32_t claimedSize = verticesLength;

				//go with the bigger size cause bigger is better
				trueSize = calcSize > claimedSize ? calcSize : claimedSize;

				location = ftell(src);
				start = ftell(src);

				fseek(src,pos,SEEK_SET);
			}
		};
	};

	class FLVER2_LayoutMember
	{
		public:
		int32_t unk00; //0, 1, or 2
		int32_t structOffset; //unused?
		uint32_t type;
		uint32_t semantic;
		int32_t index;
		int32_t size; //calc'd via type

		FLVER2_LayoutMember(){};

		//finish this!
		FLVER2_LayoutMember(FILE* src) //, int32_t offset)
		{
			fread(&unk00,20,1,src); //unk00 thru index

			switch(type)
			{
				case 0xF0:
					size = 1; break;

				case 0x10:
				case 0x11:
				case 0x12:
				case 0x13:
				case 0x15:
				case 0x2F:
					size = 4; break;

				case 0x01:
				case 0x16:
				case 0x18:
				case 0x1A:
				case 0x2E:
					size = 8; break;

				case 0x02:
					size = 12; break;
				
				case 0x03:
					size = 16; break;
			}
		};
	};

	class FLVER2_BufferLayout
	{
		public:
		int32_t memberCount;

		int32_t unk04; //assert(0)
		int32_t unk08; //assert(0)

		uint32_t membersOffset;
		FLVER2_LayoutMember* members; //size of memberCount

		FLVER2_BufferLayout(){};

		FLVER2_BufferLayout(FILE *src, int startOffset)
		{
			fread(&memberCount,16,1,src); //memberCount thru memberOffset

			int pos = ftell(src);
			fseek(src,membersOffset+startOffset,SEEK_SET);

			members = new FLVER2_LayoutMember[memberCount];
			for(int32_t i = 0; i < memberCount; i++)
			{
				members[i] = FLVER2_LayoutMember(src);
			}
			
			fseek(src,pos,SEEK_SET);
		};
	};

	class FLVER2_Texture
	{
		public:
		uint64_t pathOffset;
		uint64_t typeOffset;

		float scale_x;
		float scale_y;

		char unk10;
		char unk11;
		int16_t unk12; //assert(0)

		float unk14;
		float unk18;
		float unk1C;

		FLVER2_Texture(){};

		FLVER2_Texture(FILE* src)
		{
			fread(&pathOffset,4,1,src);
			fread(&typeOffset,4,1,src);

			fread(&scale_x,24,1,src);
		};
	};

	//weight of the four potential bones, 0 means no bone
	//format for this class is for what type of data to read
	class FLVER2_VertexBoneWeights
	{
		public:
		float a,b,c,d;
		int32_t length; //always 4

		FLVER2_VertexBoneWeights(){};

		FLVER2_VertexBoneWeights(FILE* src, uint32_t format, float divisor)
		{
			a = getValue(src,format,divisor);
			b = getValue(src,format,divisor);
			c = getValue(src,format,divisor);
			d = getValue(src,format,divisor);

			length = 4; //always 4
		};

		private:
		float getValue(FILE* src, uint32_t format, float divisor)
		{
			float value = 0;
			//char temp[2];

			//this is ugly but should be "safe"
			int8_t   tempS08;
			uint8_t  tempU08;
			int16_t  tempS16;
			uint16_t tempU16;

			//cast to the appropriate type and calculate the final value
			switch(format)
			{
				case 0: fread(&tempS08,1,1,src); value = tempS08 / divisor;
				case 1: fread(&tempU08,1,1,src); value = tempU08 / divisor;
				case 2: fread(&tempS16,2,1,src); value = tempS16 / divisor;
				case 3: fread(&tempU16,2,1,src); value = tempU16 / divisor;
			}

			return value;
		};
	};

	//index of which bone (if any) a vert is bound to
	//0 means no bone :(
	class FLVER2_VertexBoneIndices
	{
		public:
		int32_t a,b,c,d;
		int32_t length; //always 4

		FLVER2_VertexBoneIndices(){};

		FLVER2_VertexBoneIndices(FILE* src, uint32_t format)
		{
			a = getValue(src,format);
			b = getValue(src,format);
			c = getValue(src,format);
			d = getValue(src,format);

			length = 4; //always 4
		};

		private:
		int32_t getValue(FILE* src, uint32_t format)
		{
			int32_t value = 0;
			//char temp[2];

			//this is ugly but should be "safe"
			int8_t   tempS08;
			uint8_t  tempU08;
			int16_t  tempS16;
			uint16_t tempU16;

			//cast to the appropriate type and calculate the final value
			switch(format)
			{
				case 0: fread(&tempS08,1,1,src); value = tempS08;
				case 1: fread(&tempU08,1,1,src); value = tempU08;
				case 2: fread(&tempS16,2,1,src); value = tempS16;
				case 3: fread(&tempU16,2,1,src); value = tempU16;
			}

			return value;
		};
	};

	class FLVER2_VertexColor
	{
		public:
		float a,r,g,b;

		FLVER2_VertexColor(FILE* src)
		{
			fread(&a,16,1,src); //a thru b
		};
	};
};