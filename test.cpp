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

	FLVER* flver = new FLVER(reader);
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
			printf("semantic: %s\tbytes: %i\n",
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

	//memcpy(&index,fastIndices.data()+4,4);
	for(int32_t i = 0; i < 24; i++)
	{
		printf("index:%i\n",fastIndices[i]);
	}

	return 0;
};