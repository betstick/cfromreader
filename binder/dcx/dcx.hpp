#pragma once
#include "../stdafx.hpp"
//#include <bits/stdc++.h> //bit flipping

namespace cfr
{
	struct _DCX_BLOCK_
	{
		int32_t unk00; //assert 0
		int32_t dataOffset;
		int32_t dataLength;
		int32_t unk0C;
	};

	struct _DCX_HEADER_
	{
		char magic[4]; //assert "DCX\0"
		int32_t unk04; //assert 0x10000 or 0x11000
		int32_t dcsOffset; //assert 0x18
		int32_t dcpOffset; //assert 0x24
		int32_t unk10; //assert 0x24 or 0x44
		int32_t unk14;
		char dcs[4]; //assert "DCS\0"
		uint32_t uncompressedSize;
    	uint32_t compressedSize;
		char dcp[4]; //assert "DCP\0"
		char format[4]; //assert "DFLT" "EDGE" or "KRAK"
		int32_t unk2C; //assert 0x20
		int8_t unk30; //assert 6, 8, 9
		int8_t unk31; //assert 0
		int8_t unk32; //assert 0
		int8_t unk33; //assert 0
		int32_t unk34; //assert 0, 0x10000
		int32_t unk38; //assert 0
		int32_t unk3C; //assert 0
		int32_t unk40; //unk
		char dca[4]; //assert "DCA\0"
		int32_t dcaSize; //from DCA to dca end

		//only if format is EDGE
		char egdt[4]; //assert "EgdT"
		int32_t unk50; //assert 0x10100
		int32_t unk54; //assert 0x24
		int32_t unk58; //assert 0x10
		int32_t unk5C; //assert 0x10000
		int32_t lastBlockUncompressedSize;
		int32_t egdtSize; //from before "EgdT" to dca end
		int32_t blockCount;
		int32_t unk6C; //assert 0x100000
		std::vector<_DCX_BLOCK_> blocks; //size of blockCount
	};

	class DCX
	{
		public:
		_DCX_HEADER_ header;
		std::vector<char> data; //bytes of uncompressed data
		std::vector<void*> child; //pointer to whatever is in the DCX (most likely a bnd)

		DCX(){};

		//DO NOT USE THIS OUTSIDE OF TESTING
		DCX(const char* filePath)
		{
			BSReader* file = new BSReader(filePath,4096);
			init(file);
		};

		DCX(BSReader* file)
		{
			init(file);
		};

		//abstracted for multiple instantiation methods
		void init(BSReader* file)
		{
			file->read(this->header.magic,72); //magic to dcaSize
			if(this->header.format[0] == 'E') //EDGE
			{
				file->read(this->header.egdt,36);
				this->header.blocks.resize(this->header.blockCount);
				for(int32_t i = 0; i < this->header.blockCount; i++)
				{
					_DCX_BLOCK_ block;
					file->read(&block,16); //entire block
					this->header.blocks.push_back(block);
				}
			}

			//this only works in GCC i think :^), fixed endian issues
			this->header.compressedSize   = __builtin_bswap32(this->header.compressedSize  );
			this->header.uncompressedSize = __builtin_bswap32(this->header.uncompressedSize);

			if(this->header.compressedSize >= this->header.uncompressedSize)
				this->data.resize(this->header.compressedSize  );
			else
				this->data.resize(this->header.uncompressedSize);
		};

		//loads the entire file into memory :^)
		void load(BSReader* file)
		{
			//call correct decompression util and copy output into data member
			file->read(this->data.data(),this->header.compressedSize);
		};

		//clear the data to save memory
		void clear()
		{
			//what is this even supposed to do???
		};

		void open(){};

		int decompress_dcp_edge(BSReader* file)
		{
			return 0;
		};

		int decompress_dcp_dflt(BSReader* file)
		{
			//call read_zlib
			return 0;
		};

		int decompress_dcx_edge(BSReader* file)
		{
			//similar to readzlib.. but not quite.
			return 0;
		};

		int decompress_dcx_dflt(BSReader* file)
		{
			//call read_zlib
			return 0;
		};

		int decompress_dcx_krak(BSReader* file)
		{
			return 0;
		};
	};
};