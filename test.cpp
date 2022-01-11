#include "cfromreader.hpp"
#include <ctime> //needed only for testing!
using namespace cfr;

struct testResults 
{
	int64_t a;
	int64_t b;
	int64_t c;
	int64_t d;
	int64_t e;
	int64_t f;
};

int main()
{
	BSReader* reader = new BSReader("../c5370.flver",4096);

	FLVER2* flver = new FLVER2(reader);
	testResults results;
	printf("referenceId:%i\n",flver->dummies[1].referenceID);
	printf("textureCount:%i\n",flver->materials[1].textureCount);
	printf("rotation_x:%f\n",flver->bones->rotation.x);
	printf("bonecount:%i\n",flver->meshes->boneCount);
	printf("vertexIndexCount:%i\n",flver->faceSets->vertexIndexCount);
	printf("vertexCount:%i\n",flver->vertexBuffers->vertexCount);
	results.a = (int64_t)flver->dummies->referenceID;
	results.b = (int64_t)flver->materials[0].textureCount;
	results.c = (int64_t)flver->bones->rotation.x;
	results.d = (int64_t)flver->meshes->boneCount;
	results.e = (int64_t)flver->faceSets->vertexIndexCount;
	results.f = (int64_t)flver->vertexBuffers->vertexCount;

	uint64_t result = results.a + results.b + results.c + results.d + results.e + results.f;
	printf("final:%lu\n",result);

	/*FLVER flver = FLVER("../c5370.flver");*/
	printf("tex path offset:%u\n",flver->textures->path.offset);

	printf("bufferLayerCount:%i\n",flver->header->bufferLayoutCount);

	for(int32_t i = 0; i < flver->header->bufferLayoutCount; i++)
	{
		printf("bufferLayout #%i\n",i);
		printf("bufferLayoutOffset:%x\n",flver->bufferLayouts[i].membersOffset);
		printf("memberCount:%i\n",flver->bufferLayouts[i].memberCount);

		for(int32_t l = 0; l < flver->bufferLayouts[i].memberCount; l++)
		{
			//printf("semantic:%u\n",flver->bufferLayouts[i].members[l].semantic);
			printf("\tsemantic: %16s\tbytes: %2i\n",
				semanticToType(flver->bufferLayouts[i].members[l].semantic).c_str(),
				flver->bufferLayouts[i].members[l].size
			);
		}
	}

	printf("vertexSize:%i\n",flver->vertexBuffers[1].vertexSize);
	printf("vertexCount:%i\n",flver->vertexBuffers[1].vertexCount);
	printf("vertBufferSize:%i\n",flver->vertexBuffers[1].verticesLength);
	printf("layoutIndex:%i\n",flver->vertexBuffers[1].layoutIndex);

	printf("dataoffset:%x\n",flver->header->dataOffset);
	printf("indexSize:%i\n",flver->header->vertexIndexSize);
	printf("vicount:%i\n",flver->faceSets[0].vertexIndexCount);

	printf("FSoffset:%x\n",flver->faceSets[0].vertexIndicesOffset);
	printf("isTriStrip:%d\n",flver->faceSets[0].triangleStrip);
	//printf("faceSets:%u\n",flver->faceSets[0].vertexIndices16[54]);

	//printf("vertexCount");

	//uint32_t index = 0;

	//YOU MUST SCALE THE VECTOR FIRST OR IT WILL SEGFAULT
	//std::vector<char> indices;// = std::vector<char>(flver->faceSets[0].trueSize);
	//indices.reserve(flver->faceSets[0].trueSize*flver->faceSets[0].vertexIndexCount);
	//flver->faceSets[0].copyFaceSet(reader,indices.data());

	//memcpy(&index,indices.data()+2,2);
	//printf("index:%i\n",index);

	std::vector<int16_t> fastIndices = std::vector<int16_t>(flver->faceSets[0].vertexIndexCount);

	memcpy(fastIndices.data(),flver->faceSets[0].getFaceSet(reader),
		flver->faceSets[0].vertexIndexCount * (flver->faceSets[0].vertexIndexSize/8)
	);

	printf("vic:%u\n",flver->faceSets[0].vertexIndexCount);
	printf("vis:%u\n",flver->faceSets[0].vertexIndexSize/8);

	reader->open("../c5370.chrbnd.dcx",4096);
	DCX* dcx = new DCX(reader);
	printf("dcx:\tuncompressedSize:\t %u bytes\n",dcx->header.uncompressedSize);
	printf("dcx:\t  compressedSize:\t %u bytes\n",dcx->header.compressedSize  );

	reader->seek(0x4C); //beginning of the actual archive

	printf("dcxSize:%i\tdcxCap:%i\n",dcx->data.size(),dcx->data.capacity());

	dcx->mem_read_zlib(
		reader,
		&dcx->data[0],
		dcx->header.compressedSize,
		dcx->header.uncompressedSize);
	
	//FILE* dcxFile = fopen("../c5370.chrbnd.dcx","rb");
	//fseek(dcxFile,0x4C,SEEK_SET);
	//FILE* dcxData = fmemopen(&dcx->data[0],dcx->data.capacity(),"rwb");
	//int ret = dcx->inflate_zlib(dcxFile,dcxData);
	//printf("ret:%i\n",ret);

	printf("dcxdata:%i\n",dcx->data[0]);

	printf("output[4]:%c\n",&dcx->data[1]);
	printf("dcxSize:%x\n",dcx->data.capacity());

	reader->open(&dcx->data[0],dcx->data.capacity());
	BND* bnd = new BND(reader);
	printf("bnd->files.size(): %i\n",bnd->files.size());

	for(uint32_t i = 0; i < bnd->files.size(); i++)
	{
		printf("FileName: %s\t",bnd->files[i].name);
		printf("Position: 0x%8x\t",bnd->files[i].position);
		printf("Size: 0x%8x\n",bnd->files[i].uncompressedSize);
	}

	reader->seek(bnd->files[0].position);
	FLVER2* flver2 = new FLVER2(reader);
	printf("memFlverVertIndCount:%i\n",flver2->faceSets[0].vertexIndexCount);
	printf("memFlverSize:%i\n",bnd->files[0].uncompressedSize);
	printf("memFlverSize:%i\n",bnd->files[0].compressedSize);

	reader->seek(bnd->files[3].position);
	_BHF3_* bhf3 = new _BHF3_(reader);

	printf("uncompressedSize:%u\n",bnd->files[1].uncompressedSize);
	printf("  compressedSize:%u\n",bnd->files[1].compressedSize);

	for(uint64_t i = 0; i < dcx->data.size(); i++)
	{
		char tempchar;
		reader->read(&tempchar,1);
		if(tempchar == 'B')
		{
			reader->markPos();
			char temp2;
			char temp3;
			char temp4;
			reader->read(&temp2,1);
			reader->read(&temp3,1);
			reader->read(&temp4,1);
			reader->returnToMark();
			if(temp2 == 'D')
				printf("%c%c%c%c\n",tempchar,temp2,temp3,temp4);
		}
	}

	printf("File 0 name: %c\n",bhf3->header.magic[0]);
	printf("File 0 name: %i\n",bhf3->header.fileCount);

	return 0;
};