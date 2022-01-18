#pragma once
#include "../stdafx.hpp"

namespace cfr
{
	struct _BHF3_Header_
	{
		char magic[4]; //BHF3
		char version[8];
		uint8_t rawFormat;
		int8_t bigEndian; //bool
		int8_t unk0E; //0 or 1
		int8_t unk0F; //0

		//TODO: implement emergency big endian support

		int32_t fileCount;

		//all zero
		int32_t unk14;
		int32_t unk18;
		int32_t unk1C;
	};

	struct _BHF3_File_
	{
		uint8_t rawFlags;
		int8_t unk01;
		int8_t unk02;
		int8_t unk03;

		int32_t compressedSize;
		uint32_t dataOffset;

		//heavily conditional
		int32_t id;
		int32_t nameOffset;
		int32_t uncompressedSize;

		std::string name;
	};

	class _BHF3_
	{
		public:
		_BHF3_Header_ header;
		std::vector<_BHF3_File_> files;

		_BHF3_(BSReader* file)
		{
			uint64_t startPos = file->readPos;
			file->read(&this->header.magic[0],sizeof(_BHF3_Header_)); //magic thru unk1C

			for(int32_t i = 0; i < this->header.fileCount; i++)
			{
				_BHF3_File_ tempFile;
				file->read(&tempFile.rawFlags,12); //rawFlags thru dataOffset

				if(this->header.rawFormat & 0b01000000)
					file->read(&tempFile.id,4);

				if(this->header.rawFormat & 0b00100000)
				{
					file->read(&tempFile.nameOffset,4);

					file->stepIn(tempFile.nameOffset+startPos);

					file->readString(&tempFile.name);

					file->stepOut();
				}

				if(this->header.rawFormat & 0b00000100)
					file->read(&tempFile.uncompressedSize,4);

				this->files.push_back(tempFile);
			}
		};
	};
}