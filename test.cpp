#include "cfromreader.hpp"
using namespace cfr;

int main()
{
	//printf("_BND3_Heasder_ size: %i\n",sizeof(_BND3_Header_));
	Binder bnd3 = Binder("../c0000.anibnd");
	printf("Magic: %.4s\n",bnd3.header.magic); //have to manually specify the length :fatcat:
	printf("Version: %.8s\n",bnd3.header.version);
	printf("fileCount: %i\n",bnd3.header.fileCount);
	printf("unk1C: %i\n",bnd3.header.unk1C);
	//printf("file array size: %i\n",bnd3.files.size());
	printf("file name: %.256s\n",bnd3.fileHeaders[50].name);

	//01110100
	//_1110100 //padded
	return 0;
};