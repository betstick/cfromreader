#pragma once
#include "stdafx.hpp"
//welcome to FLVER-town

//standard fromsoft model format from DSPTDE onwards
//this is ONLY flver2 type files. flver0 is old and grody
//as usual, built using SoulsTemplates! :)
//to help shorten this for now, it won't support non souls games

namespace cfr {
	class FLVER
	{
		public:
		FLVER_Header* header;

		FLVER_Dummy* dummies;
		FLVER_Material* materials;
		FLVER_Bone* bones;
		FLVER_Mesh* meshes;
		FLVER_FaceSet* faceSets;
		FLVER_VertexBuffer* vertexBuffers;
		FLVER_BufferLayout* bufferLayouts;
		FLVER_Texture* textures;

		FLVER(BSReader* file) //read from already opened file
		{
			init(file);
		};

		FLVER(std::string path) //open file and create the reader
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
	void FLVER::init(BSReader* file)
	{
		header = new FLVER_Header(file);

		//just init them all at once onto the heap in order
		dummies = new FLVER_Dummy[header->dummyCount];
		materials = new FLVER_Material[header->materialCount];
		bones = new FLVER_Bone[header->boneCount];
		meshes = new FLVER_Mesh[header->meshCount];
		faceSets = new FLVER_FaceSet[header->faceSetCount];
		vertexBuffers = new FLVER_VertexBuffer[header->vertexBufferCount];
		bufferLayouts = new FLVER_BufferLayout[header->bufferLayoutCount];
		textures = new FLVER_Texture[header->textureCount];

		for(int32_t i = 0; i < header->dummyCount; i++)
		{
			dummies[i] = FLVER_Dummy(file);
		}

		for(int32_t i = 0; i < header->materialCount; i++)
		{
			materials[i] = FLVER_Material(file,*header);
		}

		for(int32_t i = 0; i < header->boneCount; i++)
		{
			bones[i] = FLVER_Bone(file);
		}

		for(int32_t i = 0; i < header->meshCount; i++)
		{
			meshes[i] = FLVER_Mesh(file,*header);
		}

		for(int32_t i = 0; i < header->faceSetCount; i++)
		{
			faceSets[i] = FLVER_FaceSet(file,*header);
		}

		for(int32_t i = 0; i < header->vertexBufferCount; i++)
		{
			vertexBuffers[i] = FLVER_VertexBuffer(file,*header);
		}

		for(int32_t i = 0; i < header->bufferLayoutCount; i++)
		{
			bufferLayouts[i] = FLVER_BufferLayout(file);
		}

		for(int32_t i = 0; i < header->textureCount; i++)
		{
			textures[i] = FLVER_Texture(file);
		}
	};
};