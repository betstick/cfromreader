#include "cfromreader.hpp"
#include <ctime> //needed only for testing!
using namespace cfr;

void compareFlverHeader(FLVER2_Header* header1, FLVER2_Header* header2)
{
	printf("magic:\t%s\t%s\n",header1->magic,header2->magic);
	printf("endn :\t%s\t%s\n",header1->endian,header2->endian);
	printf("versi:\t%8.i | %8.i\n",header1->version,header2->version);
	printf("datOf:\t%8.i | %8.i\n",header1->dataOffset,header2->dataOffset);
	printf("datLn:\t%8.i | %8.i\n",header1->dataLength,header2->dataLength);
	printf("dumCt:\t%8.i | %8.i\n",header1->dummyCount,header2->dummyCount);
	printf("matCt:\t%8.i | %8.i\n",header1->materialCount,header2->materialCount);
	printf("boneC:\t%8.i | %8.i\n",header1->boneCount,header2->boneCount);
	printf("meshC:\t%8.i | %8.i\n",header1->meshCount,header2->meshCount);
	printf("VerBC:\t%8.i | %8.i\n",header1->vertexBufferCount,header2->vertexBufferCount);
	printf("TruFC:\t%8.i | %8.i\n",header1->trueFaceCount,header2->trueFaceCount);
	printf("TotFC:\t%8.i | %8.i\n",header1->totalFaceCount,header2->totalFaceCount);
	printf("VIS  :\t%8.i | %8.i\n",header1->vertexIndexSize,header2->vertexIndexSize);
	printf("unicd:\t%8.i | %8.i\n",header1->unicode,header2->unicode);
	printf("unk4A:\t%8.i | %8.i\n",header1->unk4A,header2->unk4A);
	printf("unk4B:\t%8.i | %8.i\n",header1->unk4B,header2->unk4B);
	printf("PRC  :\t%8.i | %8.i\n",header1->primitiveRestartConstant,header2->primitiveRestartConstant);
	printf("FSC  :\t%8.i | %8.i\n",header1->faceSetCount,header2->faceSetCount);
	printf("BFLC :\t%8.i | %8.i\n",header1->bufferLayoutCount,header2->bufferLayoutCount);
	printf("txCnt:\t%8.i | %8.i\n",header1->textureCount,header2->textureCount);
	printf("unk5C:\t%8.i | %8.i\n",header1->unk5C,header2->unk5C);
	printf("unk5D:\t%8.i | %8.i\n",header1->unk5D,header2->unk5D);
	printf("unk5E:\t%8.i | %8.i\n",header1->unk5E,header2->unk5E);
	printf("unk5F:\t%8.i | %8.i\n",header1->unk5F,header2->unk5F);
	printf("unk60:\t%8.i | %8.i\n",header1->unk60,header2->unk60);
	printf("unk64:\t%8.i | %8.i\n",header1->unk64,header2->unk64);
	printf("unk68:\t%8.i | %8.i\n",header1->unk68,header2->unk68);
	printf("unk6C:\t%8.i | %8.i\n",header1->unk6C,header2->unk6C);
	printf("unk70:\t%8.i | %8.i\n",header1->unk70,header2->unk70);
	printf("unk74:\t%8.i | %8.i\n",header1->unk74,header2->unk74);
	printf("unk78:\t%8.i | %8.i\n",header1->unk78,header2->unk78);
	printf("unk7C:\t%8.i | %8.i\n",header1->unk7C,header2->unk7C);
}

int main()
{
	BSReader* reader = new BSReader("../c5370.chrbnd.dcx",4096);
	DCX* dcx = new DCX(reader);
	dcx->load(reader);

	std::vector<char> out = std::vector<char>(dcx->header.uncompressedSize);
	FILE* outFile = fmemopen(out.data(),out.size(),"w");

	printf("dcx offset: %x\n",dcx->header.offset);
	reader->seek(dcx->header.offset);

	//int ret = inflate_zlib(reader,&out[0],dcx->header.uncompressedSize);
	int ret = inflate_zlib(reader,outFile);
	fclose(outFile);
	printf("ret:%i\n",ret);

	printf("data:%c%c%c%c%c\n",out[0],out[1],out[2],out[3],out[4]);
	printf("dcx size: %i\n",dcx->header.uncompressedSize);
	printf("out size: %i\n",out.size());

	reader->open(&out[0],out.size());
	BND3* bnd = new BND3(reader,0);
	//printf("bnd file count:%i\n",bnd->fileHeaders.size());

	BSReader* bnd3file = new BSReader("../c5370.chrbnd",4096);
	BND3* bnd3 = new BND3(bnd3file,0);

	reader->markPos();
	bnd3file->markPos();

	printf("bndX.size:%i\n",reader->fileSize);
	printf("bnd3.size:%i\n",bnd3file->fileSize);

	for(uint32_t i = 0; i < reader->fileSize; i++)
	{
		char output1; 
		char output2; 
		int ret1 = reader->read(&output1,1);
		int ret2 = bnd3file->read(&output2,1);
		if(output1 != output2 || ret1 != 1 || ret2 != 1 || i < 64)
		{
			printf("row[%2.i]ret1:[%i]ret2[%i]:%8.x|%8.x\n",i,ret1,ret2,output1,output2);
		}
	}

	FILE* flverFile = fopen("../c5370.flver","rb");

	reader->returnToMark();
	bnd3file->returnToMark();

	reader->seek(bnd->fileHeaders[0].dataOffset);
	printf("generating via memory\n");
	FLVER2* flverM = new FLVER2(reader);

	printf("generating via file\n");
	FLVER2* flverF = new FLVER2("../c5370.flver");
	
	compareFlverHeader(flverM->header,flverF->header);
	
	return 0;
};