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
		void load()
		{
			//call correct decompression util and copy output into data member
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

		int inflate_zlib(FILE *source, FILE *dest)
		{
			#define CHUNK 16384
			int ret;
			unsigned have;
			z_stream strm;
			unsigned char in[CHUNK];
			unsigned char out[CHUNK];

			/* allocate inflate state */
			strm.zalloc = Z_NULL;
			strm.zfree = Z_NULL;
			strm.opaque = Z_NULL;
			strm.avail_in = 0;
			strm.next_in = Z_NULL;
			ret = inflateInit(&strm);
			if (ret != Z_OK)
				return ret;

			/* decompress until deflate stream ends or end of file */
			do {
				strm.avail_in = fread(in, 1, CHUNK, source);
				if (ferror(source)) {
					(void)inflateEnd(&strm);
					return Z_ERRNO;
				}
				if (strm.avail_in == 0)
					break;
				strm.next_in = in;

				/* run inflate() on input until output buffer not full */
				do {
					strm.avail_out = CHUNK;
					strm.next_out = out;
					ret = inflate(&strm, Z_NO_FLUSH);
					assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
					switch (ret) {
					case Z_NEED_DICT:
						ret = Z_DATA_ERROR;     /* and fall through */
					case Z_DATA_ERROR:
					case Z_MEM_ERROR:
						(void)inflateEnd(&strm);
						return ret;
					}
					have = CHUNK - strm.avail_out;
					if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
						(void)inflateEnd(&strm);
						return Z_ERRNO;
					}
				} while (strm.avail_out == 0);

				/* done when inflate() says it's done */
			} while (ret != Z_STREAM_END);

			/* clean up and return */
			(void)inflateEnd(&strm);
			return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
		}

		int mem_read_zlib(BSReader* file, char* dest, size_t sizeIn, size_t uncompressedSize)
		{
			int32_t chunkSize = 16384; //figure out good value for this?
			//std::vector<Bytef>  inBuffer = std::vector<Bytef>(chunkSize);
			//std::vector<Bytef> outBuffer = std::vector<Bytef>(chunkSize);
			unsigned char inBuffer[chunkSize];
			unsigned char outBuffer[chunkSize];


			unsigned have;

			z_stream* stream = new z_stream;
			stream->zalloc   = Z_NULL;
			stream->zfree    = Z_NULL;
			stream->opaque   = Z_NULL;
			stream->avail_in = 0;
			stream->next_in  = Z_NULL;

			FILE* destMem = fmemopen(dest,uncompressedSize,"rwb");

			if(destMem == NULL)
				throw std::runtime_error("failed to open mem\n");

			int ret = 0;
			ret = inflateInit(stream);
			if(ret != Z_OK)
				throw std::runtime_error("failed inflate init\n");

			do
			{
				stream->avail_in = file->read(inBuffer,chunkSize);

				if (stream->avail_in == 0)
            		break;

				stream->next_in = inBuffer;
				do
				{
					stream->avail_out = chunkSize;
					stream->next_out  = outBuffer;

					ret = inflate(stream, Z_NO_FLUSH);
					assert(ret != Z_STREAM_ERROR);
					if(ret != Z_OK)
					{
						printf("retcode:%i\n",ret);
						throw std::runtime_error("failed to inflate\n");
					}

					switch (ret) {
						case Z_NEED_DICT:
							ret = Z_DATA_ERROR;     /* and fall through */
						case Z_DATA_ERROR:
						case Z_MEM_ERROR:
							(void)inflateEnd(stream);
							return ret;
					}

					have = chunkSize - stream->avail_out;
					if(fwrite(outBuffer,1,have,destMem) != have){
						//throw std::runtime_error("failed memory write");
						(void)inflateEnd(stream);
                		return Z_ERRNO;}
				} while(stream->avail_out == 0);
			} while(ret != Z_STREAM_END);

			return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
		};

		int read_zlib(BSReader* file, char* dest, size_t sizeIn, size_t uncompressedSize)
		{
			int32_t chunkSize = 16384; //figure out good value for this?
			uint64_t copied = 0; //number of chunks copied
			std::vector<Bytef>  inBuffer = std::vector<Bytef>(chunkSize);
			std::vector<Bytef> outBuffer = std::vector<Bytef>(chunkSize);

			unsigned have;

			z_stream* stream = new z_stream;
			stream->zalloc   = Z_NULL;
			stream->zfree    = Z_NULL;
			stream->opaque   = Z_NULL;
			stream->avail_in = 0;
			stream->next_in  = Z_NULL;

			int ret = 0;
			ret = inflateInit(stream);
			if(ret!=Z_OK)
				throw std::runtime_error("failed inflate init");

			//while(copied + chunkSize < sizeIn)
			do
			{
				stream->avail_in = file->read(&inBuffer[0],chunkSize);

				if (stream->avail_in == 0)
            		break;

				stream->next_in = &inBuffer[0];
				do
				{
					stream->avail_out = chunkSize;
					stream->next_out  = &outBuffer[0];

					ret = inflate(stream, Z_NO_FLUSH);
					assert(ret != Z_STREAM_ERROR);
					if(ret != Z_OK)
					{
						printf("retcode:%i\n",ret);
						throw std::runtime_error("failed to inflate\n");
					}

					have = chunkSize - stream->avail_out;
					memcpy(&dest[0]+(copied*chunkSize),&outBuffer[0],have);
				} while(stream->avail_out == 0);
				copied ++;
			//} while(copied * chunkSize < uncompressedSize);
			} while(ret != Z_STREAM_END);

			printf("data copied: %i\n",copied*chunkSize);

			return 0;
		};
	};
};