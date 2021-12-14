#include "cfromreader.hpp"
using namespace cfr;

int main()
{
	//printf("_BND3_Heasder_ size: %i\n",sizeof(_BND3_Header_));
	Binder bnd = Binder("../c0000.anibnd");
	
	printf("File count: %i\n",bnd.fileHeaders.size());
	printf("Example file name: %.256s\n",bnd.fileHeaders[0].name);
	printf("Example offset hex: %x\n",bnd.fileHeaders[0].dataOffset);

	FLVER flver = FLVER("../c5370.flver");
	printf("bones: %i\n",flver.bones.size());

	return 0;
};