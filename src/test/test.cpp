#include "stdafx.hpp"

using namespace cfr;

int main()
{
	/*size_t size;
	char* bndArr = openDCX("../c5370.chrbnd.dcx",&size);

	if(bndArr == NULL)
		throw std::runtime_error("Failed to open file!\n");

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
	printf("flver.textures[63].scale.y:%8.f\n",flver->textures[63].scale_y);*/

	FILE* dvdbnd0 = fopen("../../DATA/dvdbnd0.bhd5","rb");
	if(dvdbnd0 == NULL)
		throw std::runtime_error("Failed to open BHD5 file!\n");

	BHD5_1* bhd5 = new BHD5_1(dvdbnd0);

	

	//accessing hashMap via a hash
	cfr::BHD5_1::FileHeader element = bhd5->hashMap.at(0x7C85E5FB);
	printf("filesize of 0x7C85E5FB: 0x%8.x\n",element.fileSize);
	printf("  offset of 0x7C85E5FB: 0x%8.lx\n",element.offset);

	std::unordered_map<std::string,int32_t> pathTable = createPathMap(getFileList(ds_ptde));
	int32_t hash = pathTable.at("/chr/c5370.anibnd.dcx");
	printf("hash of '/chr/c5370.anibnd.dcx': 0x%8.x\n",hash);

	return 0;
};