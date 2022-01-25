#include "flver2.hpp"

//there has to be a more sane, but still memory saving, way to do this.
//this isn't compute or memory intensive... but just look at it! its spaghetti!

namespace cfr
{
	FLVER2::Header* headerInit(MEM* src, int startOffset)
	{
		//cast initial position as a header and return its address
		FLVER2::Header* ptr = (FLVER2::Header*)&src->data[src->position+startOffset];
		mseek(src,sizeof(FLVER2::Header),SEEK_CUR);
		return ptr;
	};

	FLVER2::Dummy* dummyInit(MEM* src, int startOffset)
	{
		FLVER2::Dummy* ptr = (FLVER2::Dummy*)&src->data[src->position+startOffset];
		mseek(src,sizeof(FLVER2::Dummy),SEEK_CUR);
		return ptr;
	};

	FLVER2::Material::GxItem* gxItemInit(MEM* src, int startOffset)
	{
		FLVER2::Material::GxItem* ptr = (FLVER2::Material::GxItem*)&src->data[src->position+startOffset];
		mseek(src,ptr->length,SEEK_CUR); //length accounts for entire struct and data
		return ptr;
	};

	FLVER2::Material* materialInit(MEM* src, int startOffset, FLVER2::Header* hdr, int i)
	{
		FLVER2::Material* ptr = (FLVER2::Material*)&src->data[src->position+startOffset];
		mseek(src,sizeof(FLVER2::Material),SEEK_CUR);

		//return here after the gxitems
		long pos = mtell(src);

		mseek(src,ptr->header->gxOffset+startOffset,SEEK_SET);

		int ii;
		int itemId;

		do {
			ptr->gxItems[i][ii] = *gxItemInit(src, startOffset);
			itemId = ptr->gxItems[i][ii].id;
			ii++;
		} while(hdr->version >= 0x20010 && itemId != 0x7FFFFFFF && itemId != 0xFFFFFFFF);

		mseek(src,0,pos);

		return ptr;
	};

	FLVER2::Bone* boneInit(MEM* src, int startOffset)
	{
		FLVER2::Bone* ptr = (FLVER2::Bone*)&src->data[src->position+startOffset];
		mseek(src,sizeof(FLVER2::Bone),SEEK_CUR);
		return ptr;
	};

	FLVER2::Mesh* meshInit(MEM* src, int startOffset, FLVER2::Header* hdr)
	{
		FLVER2::Mesh* mesh;

		mesh->header = (FLVER2::Mesh::Header*)&src->data[src->position+startOffset];
		mseek(src,sizeof(FLVER2::Mesh::Header),SEEK_CUR);

		if(mesh->header->boundingBoxOffset != 0)
		{
			mesh->boundingBoxData = (FLVER2::Mesh::BoundingBoxData*)&src->data[src->position+startOffset];
			mseek(src,sizeof(FLVER2::Mesh::BoundingBoxData),SEEK_CUR);
		}

		if(hdr->version >= 0x2001A)
		{
			mesh->boundingBoxDataUnk = (FLVER2::Mesh::BoundingBoxDataUnk*)&src->data[src->position+startOffset];
			mseek(src,sizeof(FLVER2::Mesh::BoundingBoxDataUnk),SEEK_CUR);
		}

		if(mesh->header->boneCount > 0)
		{
			mesh->boneIndexData->boneIndices = (int32_t*)&src->data[src->position+startOffset];
			mseek(src,sizeof(int32_t)*mesh->header->boneCount,SEEK_CUR);
		}
	};

	FLVER2::Member* memberInit(MEM* src, int startOffset)
	{
		
	};

	FLVER2::EdgeIndices* edgeIndicesInit(MEM* src, int startOffset)
	{
		
	};

	FLVER2::FaceSet* faceSetInit(FILE* src, int startOffset)
	{
		
	};

	FLVER2::VertexBuffer* vertexBufferInit(FILE* src, int startOffset)
	{
		
	};

	FLVER2::LayoutMember* layoutMemberInit(FILE* src, int startOffset)
	{
		
	};

	FLVER2::BufferLayout* bufferLayoutInit(FILE* src, int startOffset)
	{
		
	};

	FLVER2::Texture* textureInit(FILE* src, int startOffset)
	{
		
	};

	FLVER2::VertexBoneWeights* vertexBoneWeightsInit(FILE* src, int startOffset)
	{
		
	};

	FLVER2::VertexBoneIndices* vertexBoneIndicesInit(FILE* src, int startOffset)
	{
		
	};

	FLVER2::VertexColor* vertexColorInit(FILE* src, int startOffset)
	{
		
	};

	//Don't use this outside of testing!
	FLVER2::FLVER2(const char* path)
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
	};

	//Cast a FLVER2 from a position in memory
	FLVER2::FLVER2(MEM* src)
	{
		this->startOffset = mtell(src);
		this->header = headerInit(src,this->startOffset);

		this->dummies = (Dummy*)malloc(this->header->dummyCount*sizeof(Dummy));
		this->materials = (Material*)malloc(this->header->materialCount*sizeof(Material));
		this->bones = (Bone*)malloc(this->header->boneCount*sizeof(Bone));
		this->meshes = (Mesh*)malloc(this->header->meshCount*sizeof(Mesh));
		this->faceSets = (FaceSet*)malloc(this->header->faceSetCount*sizeof(FaceSet));
		this->vertexBuffers = (VertexBuffer*)malloc(this->header->vertexBufferCount*sizeof(VertexBuffer));
		this->bufferLayouts = (BufferLayout*)malloc(this->header->bufferLayoutCount*sizeof(BufferLayout));
		this->textures = (Texture*)malloc(this->header->textureCount*sizeof(Texture));

		for(int32_t i = 0; i < header->dummyCount; i++)
			this->dummies[i] = *dummyInit(src,this->startOffset);

		for(int32_t i = 0; i < header->materialCount; i++)
			this->materials[i] = *materialInit(src,this->startOffset,i);
	};


	/*void FLVER2::init(FILE* src, uint64_t offset)
	{

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
	};*/
};