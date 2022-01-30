#include "flver2.hpp"

//there has to be a more sane, but still memory saving, way to do this.
//this isn't compute or memory intensive... but just look at it! its spaghetti!

namespace cfr
{
	FLVER2::Header* headerInit(MEM* src, int startOffset)
	{
		//cast initial position as a header and return its address
		FLVER2::Header* ptr = (FLVER2::Header*)((char*)mtellptr(src));// + startOffset);
		
		mseek(src,sizeof(FLVER2::Header),SEEK_CUR);
		return ptr;
	};

	FLVER2::Dummy* dummyInit(MEM* src, int startOffset)
	{
		//printf("dum[] pos:%x\n",mtell(src)-startOffset);
		FLVER2::Dummy* ptr = (FLVER2::Dummy*)((char*)mtellptr(src) + startOffset);
		mseek(src,sizeof(FLVER2::Dummy),SEEK_CUR);
		return ptr;
	};

	FLVER2::Material::GxItem* gxItemInit(MEM* src, int startOffset)
	{
		FLVER2::Material::GxItem* ptr = (FLVER2::Material::GxItem*)((char*)mtellptr(src) + startOffset);
		mseek(src,ptr->length,SEEK_CUR); //length accounts for entire struct and data
		return ptr;
	};

	FLVER2::Material* materialInit(MEM* src, int startOffset, FLVER2::Header* hdr, int i)
	{
		//printf("mat[%i] pos:%x\n",i,mtell(src)-startOffset);
		FLVER2::Material* mat = (FLVER2::Material*)malloc(sizeof(FLVER2::Material));
		mat->header = (FLVER2::Material::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::Material::Header),SEEK_CUR);

		//printf("mat[%i]->header->gxOffset:%x\n",i,mat->header->gxOffset);
		//this is not confirmed to work!
		if(mat->header->gxOffset != 0)
		{
			//return here after the gxitems
			long pos = mtell(src);

			mseek(src,mat->header->gxOffset+startOffset,SEEK_CUR);

			int ii;
			int itemId;

			do {
				mat->gxItems[i][ii] = *gxItemInit(src, startOffset);
				itemId = mat->gxItems[i][ii].id;
				ii++;
			} while(hdr->version >= 0x20010 && itemId != 0x7FFFFFFF && itemId != 0xFFFFFFFF);

			mseek(src,0,pos);
		}

		return mat;
	};

	FLVER2::Bone* boneInit(MEM* src, int startOffset)
	{
		FLVER2::Bone* ptr = (FLVER2::Bone*)((char*)mtellptr(src) + startOffset);
		mseek(src,sizeof(FLVER2::Bone),SEEK_CUR);
		return ptr;
	};

	FLVER2::Mesh* meshInit(MEM* src, int startOffset, FLVER2::Header* hdr)
	{
		FLVER2::Mesh* mesh;

		mesh->header = (FLVER2::Mesh::Header*)((char*)mtellptr(src) + startOffset);
		mseek(src,sizeof(FLVER2::Mesh::Header),SEEK_CUR);

		if(mesh->header->boundingBoxOffset != 0)
		{
			mesh->boundingBoxData = (FLVER2::Mesh::BoundingBoxData*)((char*)mtellptr(src) + startOffset);
			mseek(src,sizeof(FLVER2::Mesh::BoundingBoxData),SEEK_CUR);
		}

		if(hdr->version >= 0x2001A)
		{
			mesh->boundingBoxDataUnk = (FLVER2::Mesh::BoundingBoxDataUnk*)((char*)mtellptr(src) + startOffset);
			mseek(src,sizeof(FLVER2::Mesh::BoundingBoxDataUnk),SEEK_CUR);
		}

		if(mesh->header->boneCount > 0)
		{
			mesh->boneIndexData->boneIndices = (int32_t*)((char*)mtellptr(src) + startOffset);
			mseek(src,sizeof(int32_t)*mesh->header->boneCount,SEEK_CUR);
		}

		//THIS MAY NOT BE FINISHED

		return mesh;
	};

	FLVER2::Member* memberInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::EdgeIndices* edgeIndicesInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::FaceSet* faceSetInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::VertexBuffer* vertexBufferInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::LayoutMember* layoutMemberInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::BufferLayout* bufferLayoutInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::Texture* textureInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::VertexBoneWeights* vertexBoneWeightsInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::VertexBoneIndices* vertexBoneIndicesInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::VertexColor* vertexColorInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	//Don't use this outside of testing!
	/*FLVER2::FLVER2(const char* path)
	{
		FILE* file = fopen(path,"rb");
		if(file == NULL)
			throw std::runtime_error("Failed to open FLVER2 file!\n");

		fseek(file,0,SEEK_END);
		long size = ftell(file);
		rewind(file);

		//there's no way to clear this :/ //TODO: make a way to free this
		char* memory = (char*)malloc(size*sizeof(char));
		fread(memory,size,1,file);

		FLVER2(mopen(memory,size));
	};*/

	//Cast a FLVER2 from a position in memory
	FLVER2* openFLVER2(MEM* src)
	{
		long startOffset = mtell(src);
		FLVER2* flver = (FLVER2*)malloc(sizeof(FLVER2));
		flver->header = (FLVER2::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::Header),SEEK_CUR);

		flver->dummies = (FLVER2::Dummy*)malloc(flver->header->dummyCount*sizeof(FLVER2::Dummy));
		flver->materials = (FLVER2::Material*)malloc(flver->header->materialCount*sizeof(FLVER2::Material));
		flver->bones = (FLVER2::Bone*)malloc(flver->header->boneCount*sizeof(FLVER2::Bone));
		flver->meshes = (FLVER2::Mesh*)malloc(flver->header->meshCount*sizeof(FLVER2::Mesh));
		flver->faceSets = (FLVER2::FaceSet*)malloc(flver->header->faceSetCount*sizeof(FLVER2::FaceSet));
		flver->vertexBuffers = (FLVER2::VertexBuffer*)malloc(flver->header->vertexBufferCount*sizeof(FLVER2::VertexBuffer));
		flver->bufferLayouts = (FLVER2::BufferLayout*)malloc(flver->header->bufferLayoutCount*sizeof(FLVER2::BufferLayout));
		flver->textures = (FLVER2::Texture*)malloc(flver->header->textureCount*sizeof(FLVER2::Texture));

		for(int32_t i = 0; i < flver->header->dummyCount; i++)
			flver->dummies[i] = *dummyInit(src,startOffset);

		for(int32_t i = 0; i < flver->header->materialCount; i++)
			flver->materials[i] = *materialInit(src,startOffset,flver->header,i);
		
		for(int32_t i = 0; i < flver->header->boneCount; i++)
			flver->bones[i] = *boneInit(src,startOffset);

		return flver;
	};


	/*void FLVER2::init(FILE* src, uint64_t offset)
	{
		

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
	};*/
};