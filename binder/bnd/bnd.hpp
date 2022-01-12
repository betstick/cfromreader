#pragma once
#include "bnd3.hpp"
#include "bnd4.hpp"

namespace cfr
{
    enum BNDVER
    {
        BNDv3,
        BNDv4
    };

    struct BND_File
    {
        uint64_t position; //location in mem relative to bnd start
        uint32_t id;
        char name[256]; //supports up to win32 path limit
        uint64_t compressedSize;
        uint64_t uncompressedSize;
		//might need to create a file type enum
    };

	class BND
    {
        private:
        _BND3_Header_ bnd3header;
        //_BND4_Header_ bnd4header;
        BNDVER version;

        public:
        std::vector<BND_File> files;

        BND(){};

        BND(BSReader* file)
        {
            char magic[4];
            file->read(&magic,4);
            file->seek(0); //back to start

            switch(magic[3])
            {
                default:
                    throw std::runtime_error("Unkown BND format!\n");
                case '3':
                    initBND3(file); break;
                case '4':
                    initBND4(file); break;
            }
        };

        private:
        void initBND3(BSReader* file)
        {
            BND3* bnd3 = new BND3(file,0);

            for(int32_t i = 0; i < bnd3->fileHeaders.size(); i++)
            {
				BND_File bndFile;
				bndFile.position = bnd3->fileHeaders[i].dataOffset;
				memcpy(&bndFile.name[0],&bnd3->fileHeaders[i].name[0],256);
				bndFile.id = bnd3->fileHeaders[i].id;
				//bndFile.compressedSize   = __builtin_bswap32(bnd3->fileHeaders[i].compressedSize  );
				//bndFile.uncompressedSize = __builtin_bswap32(bnd3->fileHeaders[i].uncompressedSize);
				bndFile.compressedSize   = (bnd3->fileHeaders[i].compressedSize  );
				bndFile.uncompressedSize = (bnd3->fileHeaders[i].uncompressedSize);
				files.push_back(bndFile);
            }
        };

        void initBND4(BSReader* file)
        {
			//TOOD: write the bnd4 classes and structs
			//BND4 bnd4 = new BND4(file);
        };
    };
};