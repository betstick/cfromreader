#include "cfromreader.hpp"

using namespace cfr;

int main()
{
	size_t size;
	char* bndArr = openDCX("../c5370.chrbnd.dcx",&size);

	FILE* bndPtr = fmemopen(bndArr,size,"r");

	int bnd3count;
	BND3File* files = openBnd3(bndPtr,&bnd3count);

	for(int i = 0; i < bnd3count; i++)
	{
		printf("bndFile[%i] offset: 0x%x\n",i,files[i].offset);
		printf("bndFile[%i]   type: %i\n",i,files[i].type  );
	}

	fseek(bndPtr,files[0].offset,SEEK_SET);

	FLVER2* flver = new FLVER2(bndPtr);
	
	printf("flver.header.magic:%s\n",flver->header->magic);
	printf("flver.meshes:%i\n",flver->header->meshCount);
	printf("flver.textures[63].scale.x:%8.f\n",flver->textures[63].scale_x);
	printf("flver.textures[63].scale.y:%8.f\n",flver->textures[63].scale_y);

	return 0;
};