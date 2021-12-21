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

	std::chrono::_V2::system_clock::time_point startTime;
	std::chrono::_V2::system_clock::time_point endTime;
};

/*bool validateResults(testResults* results, uint64_t count)
{

};

testResults benchmarkFlver(BSReader* reader)
{
	testResults results;
	results.startTime = std::chrono::system_clock::now();

	FLVER* flver = new FLVER(reader);
	results.a = (int64_t)flver->dummies->referenceID;
	results.b = (int64_t)flver->materials[0].textureCount;
	results.c = (int64_t)flver->bones->rotation.x;
	results.d = (int64_t)flver->meshes->boneCount;
	results.e = (int64_t)flver->faceSets->vertexIndexCount;
	results.f = (int64_t)flver->vertexBuffers->vertexCount;

	results.endTime = std::chrono::system_clock::now();
	return results;
};*/

int main()
{
	auto startTime = std::chrono::system_clock::now();

	//printf("_BND3_Heasder_ size: %i\n",sizeof(_BND3_Header_));
	/*Binder bnd = Binder("../c0000.anibnd");
	
	printf("File count: %i\n",bnd.fileHeaders.size());
	printf("Example file name: %.256s\n",bnd.fileHeaders[0].name);
	printf("Example offset hex: %x\n",bnd.fileHeaders[0].dataOffset);*/

	BSReader* reader = new BSReader("../c5370.flver",4096);

	/*uint64_t testCount = 8;
	testResults* resultArr = new testResults[testCount];

	for(uint64_t i = 0; i < testCount; i++)
	{
		resultArr[i] = benchmarkFlver(reader);
	}*/

	FLVER* flver = new FLVER(reader);
	testResults results;
	/*printf("headerstuff:%i\n",flver->dummies[1].referenceID);
	printf("materials:%i\n",flver->materials[1].textureCount);
	printf("rotation:%f\n",flver->bones->rotation.x);
	printf("bonecount:%i\n",flver->meshes->boneCount);
	printf("faceSets:%i\n",flver->faceSets->vertexIndexCount);
	printf("vertexBuffer:%i\n",flver->vertexBuffers->vertexCount);*/
	results.a = (int64_t)flver->dummies->referenceID;
	results.b = (int64_t)flver->materials[0].textureCount;
	results.c = (int64_t)flver->bones->rotation.x;
	results.d = (int64_t)flver->meshes->boneCount;
	results.e = (int64_t)flver->faceSets->vertexIndexCount;
	results.f = (int64_t)flver->vertexBuffers->vertexCount;

	uint64_t result = results.a + results.b + results.c + results.d + results.e + results.f;
	printf("final:%i\n",result);

	/*FLVER flver = FLVER("../c5370.flver");
	printf("bones: %i\n",flver.bones.size());*/

	//auto endTime = std::chrono::system_clock::now();
	//std::chrono::duration<double> elapsed = endTime - startTime;
	//std::cout << "Elapsed time: " << elapsed.count();
	//printf("Elapsed time: %f\n",elapsed.count());

	return 0;
};