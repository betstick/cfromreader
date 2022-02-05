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

			mseek(src,pos,SEEK_SET);
		}

		//printf("\t end pos:%x\n",i,mtell(src)-startOffset);
		return mat;
	};

	FLVER2::Bone* boneInit(MEM* src, int startOffset, int i)
	{
		//printf("bone[%i] pos:%x\n",i,mtell(src)-startOffset);
		FLVER2::Bone* ptr = (FLVER2::Bone*)((char*)mtellptr(src) + startOffset);
		mseek(src,sizeof(FLVER2::Bone),SEEK_CUR);
		return ptr;
	};

	FLVER2::Mesh* meshInit(MEM* src, int startOffset, FLVER2::Header* hdr)
	{
		printf("mesh[i] pos:%x\n",mtell(src)-startOffset);
		FLVER2::Mesh* mesh = (FLVER2::Mesh*)malloc(sizeof(FLVER2::Mesh));

		mesh->header = (FLVER2::Mesh::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::Mesh::Header),SEEK_CUR);

		int pos = mtell(src);

		if(mesh->header->boundingBoxOffset != 0)
		{
			mesh->boundingBoxData = (FLVER2::Mesh::BoundingBoxData*)mtellptr(src);
			mseek(src,sizeof(FLVER2::Mesh::BoundingBoxData),SEEK_CUR);

			if(hdr->version >= 0x2001A)
			{
				mesh->boundingBoxDataUnk = (FLVER2::Mesh::BoundingBoxDataUnk*)mtellptr(src);
				mseek(src,sizeof(FLVER2::Mesh::BoundingBoxDataUnk),SEEK_CUR);
			}
		}

		if(mesh->header->boneCount > 0)
		{
			mseek(src,sizeof(int32_t)*mesh->header->boneIndicesOffset+startOffset,SEEK_SET);
			mesh->boneIndices = (int32_t**)mtellptr(src);
		}

		mseek(src,mesh->header->faceSetIndicesOffset+startOffset,SEEK_SET);
		mesh->faceSetIndices = (int32_t**)mtellptr(src);

		mseek(src,mesh->header->vertexBufferIndicesOffset+startOffset,SEEK_SET);
		mesh->vertexBufferIndices = (int32_t**)mtellptr(src);

		mseek(src,pos,SEEK_SET);

		return mesh;
	};

	FLVER2::Member* memberInit(MEM* src, int startOffset)
	{
		return nullptr;
	};

	FLVER2::EdgeIndices* edgeIndicesInit(MEM* src, int startOffset)
	{
		FLVER2::EdgeIndices* indices = (FLVER2::EdgeIndices*)malloc(sizeof(FLVER2::EdgeIndices));
		indices->header = (FLVER2::EdgeIndices::Header*)mtellptr(src);

		indices->members = (FLVER2::Member*)malloc(sizeof(FLVER2::Member*)*indices->header->memberCount);

		return indices;
	};

	FLVER2::FaceSet* faceSetInit(MEM* src, int startOffset, FLVER2::Header* hdr)
	{
		FLVER2::FaceSet* set = (FLVER2::FaceSet*)malloc(sizeof(FLVER2::FaceSet));
		set->header = (FLVER2::FaceSet::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::FaceSet::Header),SEEK_CUR);

		//printf("faceset: count: %i\n",set->header->vertexIndexCount);
		//printf("faceset: offset: 0x%x\n",set->header->vertexIndicesOffset);

		if(hdr->version >= 0x20009)
		{
			set->vertInfo = (FLVER2::FaceSet::VertInfo*)mtellptr(src);
			mseek(src,sizeof(FLVER2::FaceSet::VertInfo),SEEK_CUR);
		}

		set->vertexSize = hdr->vertexIndexSize == 0 ? set->vertexSize : hdr->vertexIndexSize;

		long pos = mtell(src);
		mseek(src,set->header->vertexIndicesOffset+hdr->dataOffset+startOffset,SEEK_SET);

		if(set->vertexSize == 8)
		{
			//TODO: fix this mess later :/
			/*set->vertexIndicesEdge = (FLVER2::EdgeIndices**)malloc(sizeof(FLVER2::EdgeIndices)*set->header->vertexIndexCount);

			for(int i = 0; i < set->header->vertexIndexCount; i++)
			{
				set->vertexIndicesEdge[i] = edgeIndicesInit(src, startOffset);
			}*/
		}
		else if(set->vertexSize == 16)
		{
			set->vertexIndicesShort = (uint16_t**)mtellptr(src);
		}
		else if(set->vertexSize == 32)
		{
			set->vertexIndicesInt = (uint32_t**)mtellptr(src);
		}

		mseek(src,pos,SEEK_SET);

		return set;
	};

	FLVER2::VertexBuffer* vertexBufferInit(MEM* src, int startOffset, FLVER2::Header* hdr)
	{
		FLVER2::VertexBuffer* buff = (FLVER2::VertexBuffer*)malloc(sizeof(FLVER2::VertexBuffer));
		buff->header = (FLVER2::VertexBuffer::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::VertexBuffer::Header),SEEK_CUR);

		if(buff->header->unk10 == 0 && buff->header->unk14 == 0)
		{
			long pos = mtell(src);
			mseek(src,hdr->dataOffset+buff->header->bufferOffset+startOffset,SEEK_SET);

			buff->vertices = (char**)mtellptr(src);

			mseek(src,pos,SEEK_SET);
		}

		return buff;
	};

	FLVER2::BufferLayout* bufferLayoutInit(MEM* src, int startOffset)
	{
		FLVER2::BufferLayout* layout = (FLVER2::BufferLayout*)malloc(sizeof(FLVER2::BufferLayout));
		layout->header = (FLVER2::BufferLayout::Header*)mtellptr(src);
		mseek(src,sizeof(FLVER2::BufferLayout::Header),SEEK_CUR);

		long pos = mtell(src);

		//is this needed?
		layout->members = (FLVER2::LayoutMember**)malloc(sizeof(void*)*layout->header->memberCount);

		for(int i = 0; i < layout->header->memberCount; i++)
		{
			layout->members[i] = (FLVER2::LayoutMember*)mtellptr(src);
			mseek(src,sizeof(FLVER2::LayoutMember),SEEK_CUR);
		}

		mseek(src,pos,SEEK_SET);

		return layout;
	};

	FLVER2::Texture* textureInit(MEM* src)
	{
		FLVER2::Texture* tex = (FLVER2::Texture*)mtellptr(src);
		mseek(src,sizeof(FLVER2::Texture),SEEK_CUR);

		return tex;
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
			flver->bones[i] = *boneInit(src,startOffset,i);

		for(int32_t i = 0; i < flver->header->meshCount; i++)
			flver->meshes[i] = *meshInit(src,startOffset,flver->header);

		for(int32_t i = 0; i < flver->header->faceSetCount; i++)
			flver->faceSets[i] = *faceSetInit(src,startOffset,flver->header);

		for(int32_t i = 0; i < flver->header->vertexBufferCount; i++)
			flver->vertexBuffers[i] = *vertexBufferInit(src,startOffset,flver->header);

		for(int32_t i = 0; i < flver->header->bufferLayoutCount; i++)
			flver->bufferLayouts[i] = *bufferLayoutInit(src,startOffset);

		for(int32_t i = 0; i < flver->header->textureCount; i++)
			flver->textures[i] = *textureInit(src);

		return flver;
	};

	void exportFLVER2(FLVER2 flver, const char* filename)
	{
		FILE* out = fopen(filename,"w");

		char magicString[] = "magic: xxxxxx\n";
		memcpy(&magicString[7],flver.header->magic,5); //cut off the null char
		fwrite(magicString,14,1,out);

		fclose(out);
	};
};