#include "flver.hpp"

namespace cfr
{
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