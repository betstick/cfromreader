#pragma once
#include "../stdafx.hpp"
#include "flver2_structs.hpp"
//welcome to FLVER-town

//standard fromsoft model format from DSPTDE onwards
//as usual, built using SoulsTemplates! :)
//to help shorten this for now, it won't support non souls games

namespace cfr {
	class FLVER2
	{
		public:
		FLVER2_Header* header;

		FLVER2_Dummy* dummies;
		FLVER2_Material* materials;
		FLVER2_Bone* bones;
		FLVER2_Mesh* meshes;
		FLVER2_FaceSet* faceSets;
		FLVER2_VertexBuffer* vertexBuffers;
		FLVER2_BufferLayout* bufferLayouts;
		FLVER2_Texture* textures;

		int startOffset = 0;

		FLVER2(FILE* src) //read from already opened file
		{
			startOffset = ftell(src);
			init(src,startOffset);
		};

		FLVER2(const char* path) //open file and create the reader
		{
			FILE* file = fopen(path,"r");
			init(file, 0); //this one is differet, its okay
		};

		private:
		void init(FILE* src, uint64_t offset)
		{
			//printf("offset is: 0x%x\n",offset);
			header = new FLVER2_Header(src,startOffset);
			//fseek(src,+startOffset,SEEK_SET);

			//just init them all at once onto the heap in order
			dummies = new FLVER2_Dummy[header->dummyCount];
			materials = new FLVER2_Material[header->materialCount];
			bones = new FLVER2_Bone[header->boneCount];
			meshes = new FLVER2_Mesh[header->meshCount];
			faceSets = new FLVER2_FaceSet[header->faceSetCount];
			vertexBuffers = new FLVER2_VertexBuffer[header->vertexBufferCount];
			bufferLayouts = new FLVER2_BufferLayout[header->bufferLayoutCount];
			textures = new FLVER2_Texture[header->textureCount];

			//printf("reading flver data at: %x\n",file->readPos);

			for(int32_t i = 0; i < header->dummyCount; i++)
				dummies[i] = FLVER2_Dummy(src,startOffset);

			for(int32_t i = 0; i < header->materialCount; i++)
				materials[i] = FLVER2_Material(src,offset,*header,startOffset);

			for(int32_t i = 0; i < header->boneCount; i++)
				bones[i] = FLVER2_Bone(src,startOffset);

			for(int32_t i = 0; i < header->meshCount; i++)
				meshes[i] = FLVER2_Mesh(src,*header,startOffset);

			for(int32_t i = 0; i < header->faceSetCount; i++)
				faceSets[i] = FLVER2_FaceSet(src,*header);

			for(int32_t i = 0; i < header->vertexBufferCount; i++)
				vertexBuffers[i] = FLVER2_VertexBuffer(src,*header,startOffset);

			for(int32_t i = 0; i < header->bufferLayoutCount; i++)
				bufferLayouts[i] = FLVER2_BufferLayout(src,startOffset);

			for(int32_t i = 0; i < header->textureCount; i++)
				textures[i] = FLVER2_Texture(src);
		};

		public:
		//returns formatted list on how the vertex data is layed out
		std::string getVertexStructure();	
	};
};