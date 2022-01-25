#pragma once
#include "../stdafx.hpp"
//#include "flver2_structs.hpp"
//welcome to FLVER-town

//standard fromsoft model format from DSPTDE onwards
//as usual, built using SoulsTemplates! :)
//to help shorten this for now, it won't support non souls games

namespace cfr {
	class FLVER2
	{
		public:
		struct Header
		{
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
		};

		struct Dummy
		{
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
		};

		struct Material //unclear on how safe this is '_>', specifically gxOffset
		{
			struct Header
			{
				uint32_t nameOffset = 0;
				uint32_t mtdOffset = 0;

				int32_t textureCount;
				int32_t textureIndex;

				uint32_t flags;
				uint32_t gxOffset;

				int32_t unk18;
				int32_t unk1C; //assert(0)
			};
			
			struct GxItem //length INCLUDES this struct!
			{
				uint32_t id;
				int32_t unk04; //maybe assert(100)? if header.version < 0x20010
				int32_t length;

				char data[];
			};

			Header* header;
			GxItem** gxItems;
		};

		struct Bone
		{
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
		};

		struct Mesh
		{
			struct Header
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
			};

			//only if boundingBoxOffset != 0
			struct BoundingBoxData
			{
				float boundingBoxMin_x;
				float boundingBoxMin_y;
				float boundingBoxMin_z;
				float boundingBoxMax_x;
				float boundingBoxMax_y;
				float boundingBoxMax_z;
			};

			//only if header.version >= 0x2001A
			struct BoundingBoxDataUnk
			{
				float boundingBoxUnk_x; 
				float boundingBoxUnk_y;
				float boundingBoxUnk_z;
			};
			
			//only if boneCount > 0
			struct BoneIndexData
			{
				int32_t* boneIndices; //size of boneCount
			};

			struct MeshFooter
			{
				int32_t* faceSetIndices; //size of faceSetCount
				int32_t* vertexBufferIndices; //size of vertexBufferCount
			};
			
			Header* header = NULL;
			BoundingBoxData* boundingBoxData = NULL;
			BoundingBoxDataUnk* boundingBoxDataUnk = NULL;
			BoneIndexData* boneIndexData = NULL;
			MeshFooter* meshFooter = NULL;
		};

		struct Member
		{
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

		struct EdgeIndices
		{
			int16_t memberCount;
			int16_t unk02;
			int32_t unk04;
			int32_t unk08; //assert(0)
			int32_t unk0C;
			Member* members; //size of memberCount
		};

		struct FaceSet
		{
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
		};

		struct VertexBuffer
		{
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
		};

		struct LayoutMember
		{
			int32_t unk00; //0, 1, or 2
			int32_t structOffset; //unused?
			uint32_t type;
			uint32_t semantic;
			int32_t index;
			int32_t size; //calc'd via type
		};

		struct BufferLayout
		{
			int32_t memberCount;

			int32_t unk04; //assert(0)
			int32_t unk08; //assert(0)

			uint32_t membersOffset;
			LayoutMember* members; //size of memberCount
		};

		struct Texture
		{
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
		};

		struct VertexBoneWeights
		{
			float a,b,c,d;
			int32_t length; //always 4
		};

		struct VertexBoneIndices
		{
			int32_t a,b,c,d;
			int32_t length; //always 4
		};

		struct VertexColor
		{
			float a,r,g,b;
		};

		private:
		Header* headerInit(MEM* src, int startOffset);

		Dummy* dummyInit(MEM* src, int startOffset);

		Material* materialInit(MEM* src, int startOffset, int i);

		Bone* boneInit(MEM* src, int startOffset);

		Mesh* meshInit(MEM* src, int startOffset);

		Member* memberInit(MEM* src, int startOffset);

		EdgeIndices* edgeIndicesInit(MEM* src, int startOffset);

		FaceSet* faceSetInit(MEM* src, int startOffset);

		VertexBuffer* vertexBufferInit(MEM* src, int startOffset);

		LayoutMember* layoutMemberInit(MEM* src, int startOffset);

		BufferLayout* bufferLayoutInit(MEM* src, int startOffset);

		Texture* textureInit(MEM* src, int startOffset);

		VertexBoneWeights* vertexBoneWeightsInit(MEM* src, int startOffset);

		VertexBoneIndices* vertexBoneIndicesInit(MEM* src, int startOffset);

		VertexColor* vertexColorInit(MEM* src, int startOffset);

		public:
		Header* header;
		Dummy* dummies;
		Material* materials;
		Bone* bones;
		Mesh* meshes;
		FaceSet* faceSets;
		VertexBuffer* vertexBuffers;
		BufferLayout* bufferLayouts;
		Texture* textures;

		int startOffset = 0;
		MEM** src;

		//read from already opened file
		FLVER2(MEM* src);

		//open file and read
		FLVER2(const char* path);
	};
};