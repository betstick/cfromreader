#include "cfromreader.hpp"
using namespace cfr;

int main()
{
	//printf("_BND3_Heasder_ size: %i\n",sizeof(_BND3_Header_));
	/*Binder bnd = Binder("../c0000.anibnd");
	
	printf("File count: %i\n",bnd.fileHeaders.size());
	printf("Example file name: %.256s\n",bnd.fileHeaders[0].name);
	printf("Example offset hex: %x\n",bnd.fileHeaders[0].dataOffset);*/

	BSReader* reader = new BSReader("../c5370.flver",4096);

	FLVER* flver = new FLVER(reader);
	printf("headerstuff:%i\n",flver->dummies[1].referenceID);
	printf("materials:%i\n",flver->materials[1].textureCount);
	printf("rotation:%f\n",flver->bones->rotation.x);
	printf("bonecount:%i\n",flver->meshes->boneCount);

	/*FLVER flver = FLVER("../c5370.flver");
	printf("bones: %i\n",flver.bones.size());*/

	return 0;
};