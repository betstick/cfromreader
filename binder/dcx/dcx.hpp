#pragma once
#include "../stdafx.hpp"

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

		DCX(){};

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
		};

		//loads the entire file into memory :^)
		void load()
		{
			data = std::vector<char>(this->header.uncompressedSize);
		};

		//clear the data to save memory
		void clear()
		{
			//what is this even supposed to do???
		};

		void open(){};

		int deflate_zlib(BSReader* file, char* dest, size_t sizeIn, size_t uncompressedSize)
		{
			uint32_t copied = 0; //amount of data copied
			std::vector<Bytef>  inBuffer = std::vector<Bytef>(16384);
			std::vector<Bytef> outBuffer = std::vector<Bytef>(16384);

			z_stream* stream = new z_stream;
			stream->zalloc   = Z_NULL;
			stream->zfree    = Z_NULL;
			stream->opaque   = Z_NULL;
			stream->avail_in = 0;
			stream->next_in  = Z_NULL;

			inflateInit(stream);

			int ret = 0;
			while(copied + 16384 < sizeIn)
			{
				stream->avail_in  = file->read(&inBuffer[0],16384);
				stream->next_in   = &inBuffer[0];
				do
				{
					stream->avail_out = 16384;
					stream->next_out  = &outBuffer[0];

					ret = inflate(stream, Z_NO_FLUSH);
					if(ret != Z_OK)
					{
						printf("retcode:%i\n",ret);
						throw std::runtime_error("failed to inflate\n");
					}

					memcpy(&dest[copied],&outBuffer[0],16384);
					copied += 16384;
				} while(stream->avail_out == 0);
			}

			if(copied < sizeIn)
			{
				uint32_t buffer = sizeIn - copied;

				stream->avail_in  = file->read(&inBuffer[0],buffer);
				stream->next_in   = &inBuffer[0];
				stream->avail_out = buffer;
				stream->next_out  = &outBuffer[0];

				int ret = inflate(stream, Z_NO_FLUSH);
				if(ret != Z_OK)
				{
					printf("retcode:%i\n",ret);
					throw std::runtime_error("failed to inflate\n");
				}

				memcpy(&dest[copied+buffer],&outBuffer[0],buffer);
			}

			return 0;
		};
	};
};