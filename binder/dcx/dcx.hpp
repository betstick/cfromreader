#pragma once
#include "../stdafx.hpp"

//only the children of DCX files are needed in memory?

namespace cfr
{
	//i'm 99% sure this code is somewhere else as well. can't find it.
	enum _DCX_TYPE_
	{
		unkown,
		none,
		zlib,
		dcp_edge,
		dcp_dflt,
		dcx_edge,
		dcx_dflt_10000_24_9,
		dcx_dflt_10000_44_9,
		dcx_dflt_11000_44_8,
		dcx_dflt_11000_44_9,
		dcx_krak
	};

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

		uint64_t offset; //where the archive begins, my own addition
	};

	class DCX
	{
		public:
		_DCX_HEADER_ header;
		_DCX_TYPE_ type;
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
			uint64_t startPos = file->readPos;

			file->read(this->header.magic,76); //magic to dcaSize
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

			uint64_t endPos = file->readPos;

			this->header.offset = endPos - startPos;
		};

		//loads the entire file into memory :^)
		void load(BSReader* file)
		{
			//call correct decompression util and copy output into data member
			file->read(this->data.data(),this->header.compressedSize);

			if(strcmp(this->header.magic,"DCP") == 0)
			{
				printf("DCP\n");
				//fill this out later when i get around to needing it
			}
			else if(strcmp(this->header.magic,"DCX") == 0)
			{
				printf("DCX\n");
				if(strncmp(this->header.format,"EDGE",4) == 0)
				{
					if(this->header.unk10 == 0x24)
					{
						this->type = dcx_dflt_10000_24_9;
					}
					else if(this->header.unk10 == 0x44)
					{
						if(this->header.unk04 == 0x10000)
						{
							this->type = dcx_dflt_10000_44_9;
						}
						else if(this->header.unk04 == 0x11000)
						{
							if(this->header.unk30 == 0x8000000)
							{
								this->type = dcx_dflt_11000_44_8;
							}
							else if(this->header.unk30 == 0x9000000)
							{
								this->type = dcx_dflt_11000_44_9;
							}
						}
					}
					decompress_dcx_edge(file,this->type);
				}
				else if(strncmp(this->header.format,"DFLT",4) == 0)
				{
					decompress_dcx_dflt(file);
				}
				else if(strncmp(this->header.format,"KRAK",4) == 0)
				{
					decompress_dcx_krak(file);
				}
			}
			else
			{
				decompress_zlib(file);
			}
		};

		//clear the data to save memory
		void clear()
		{
			//delete whatever is in the data vector.
		};

		void open(){};

		int decompress_zlib(BSReader* file)
		{
			//call inflate_zlib
			return 0;
		};

		int decompress_dcp_edge(BSReader* file)
		{
			return 0;
		};

		int decompress_dcp_dflt(BSReader* file)
		{
			//call inflate_zlib
			return 0;
		};

		int decompress_dcx_edge(BSReader* file, _DCX_TYPE_ type)
		{
			//similar to inflate_zlib.. but not quite.
			return 0;
		};

		int decompress_dcx_dflt(BSReader* file)
		{
			//call inflate_zlib
			return 0;
		};

		int decompress_dcx_krak(BSReader* file)
		{
			return 0;
		};
	};
};