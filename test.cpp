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
	BSReader* reader = new BSReader("../c5370.chrbnd.dcx",4096);
	reader->getSize();
	DCX* dcx = new DCX(reader);
	dcx->load(reader);

	//std::vector<char> in  = std::vector<char>(dcx->header.compressedSize  );
	std::vector<char> out = std::vector<char>(dcx->header.uncompressedSize);

	reader->seek(0x4C);

	int ret = inflate_zlib(reader,&out[0],dcx->header.uncompressedSize);
	printf("ret:%i\n",ret);

	printf("data:%c%c%c%c%c\n",out[0],out[1],out[2],out[3],out[4]);
	printf("dcx size: %i\n",dcx->header.uncompressedSize);
	printf("out size: %i\n",out.size());

	reader->open(&out[0],out.size());
	BND* bnd = new BND(reader);
	printf("bnd file count:%i\n",bnd->files.size());

	for(int32_t i = 0; i < bnd->files.size(); i++)
	{
		printf("fileName:%s\n",bnd->files[i].name);
	}

	reader->seek(bnd->files[0].position);
	FLVER2* flver2 = new FLVER2(reader);
	printf("flverMeshes:%i\n",flver2->header->meshCount);

	reader->seek(bnd->files[3].position);
	_BHF3_* bhf3 = new _BHF3_(reader);
	printf("bhf3 file count:%i\n",bhf3->files.size());

	for(int32_t i = 0; i < bhf3->header.fileCount; i++)
	{
		printf("bhf3 fileName:%s\n",bhf3->files[i].name);
	}

	printf("halfway char: %i %c\n",out[663872],out[663872]); //correct
	printf("3/4 char: %i %c\n",out[945056] ,out[945056] ); //correct!
	printf("4/5 char: %i %c\n",out[1123920],out[1123920]); //correct!!
	printf("5/6 char: %i %c\n",out[1232488],out[1232488]); //correct!!!
	printf("6/7 char: %i %c\n",out[1283008],out[1283008]); //correct!!!
	printf("7/8 char: %i %c\n",out[1324896],out[1324896]); //correct!!!!
	printf("8/9 char: %i %c\n",out[1326119],out[1326119]); //correct!!!!!
	printf("9/9 char: %i %c\n",out[1326420],out[1326420]); //correct!!!!!!
};