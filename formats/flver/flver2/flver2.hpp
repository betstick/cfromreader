#pragma once
#include "../stdafx.hpp"
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

		FLVER2(BSReader* file) //read from already opened file
		{
			init(file);
		};

		FLVER2(std::string path) //open file and create the reader
		{
			BSReader file = BSReader(path,4096);
			init(&file); //this one is differet, its okay
		};

		private:
		void init(BSReader* file);	

		public:
		//returns formatted list on how the vertex data is layed out
		std::string getVertexStructure();	
	};

	//moved from the cpp file cause i don't feel like fixing compile issue right now
	void FLVER2::init(BSReader* file)
	{
		header = new FLVER2_Header(file);

		//just init them all at once onto the heap in order
		dummies = new FLVER2_Dummy[header->dummyCount];
		materials = new FLVER2_Material[header->materialCount];
		bones = new FLVER2_Bone[header->boneCount];
		meshes = new FLVER2_Mesh[header->meshCount];
		faceSets = new FLVER2_FaceSet[header->faceSetCount];
		vertexBuffers = new FLVER2_VertexBuffer[header->vertexBufferCount];
		bufferLayouts = new FLVER2_BufferLayout[header->bufferLayoutCount];
		textures = new FLVER2_Texture[header->textureCount];

		for(int32_t i = 0; i < header->dummyCount; i++)
		{
			dummies[i] = FLVER2_Dummy(file);
		}

		for(int32_t i = 0; i < header->materialCount; i++)
		{
			materials[i] = FLVER2_Material(file,*header);
		}

		for(int32_t i = 0; i < header->boneCount; i++)
		{
			bones[i] = FLVER2_Bone(file);
		}

		for(int32_t i = 0; i < header->meshCount; i++)
		{
			meshes[i] = FLVER2_Mesh(file,*header);
		}

		for(int32_t i = 0; i < header->faceSetCount; i++)
		{
			faceSets[i] = FLVER2_FaceSet(file,*header);
		}

		for(int32_t i = 0; i < header->vertexBufferCount; i++)
		{
			vertexBuffers[i] = FLVER2_VertexBuffer(file,*header);
		}

		for(int32_t i = 0; i < header->bufferLayoutCount; i++)
		{
			bufferLayouts[i] = FLVER2_BufferLayout(file);
		}

		for(int32_t i = 0; i < header->textureCount; i++)
		{
			textures[i] = FLVER2_Texture(file);
		}
	};
};