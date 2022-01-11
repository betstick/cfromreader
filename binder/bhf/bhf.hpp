#pragma once
#include "bhf3.hpp"
#include "bhf4.hpp"

namespace cfr
{
	struct _BHF_File_
	{
		uint32_t dataOffset;
		int32_t id;
		int32_t nameOffset;
		char name[256];

		int32_t compressedSize;
		int32_t uncompressedSize;
	};

	class BHF
	{
		public:
		std::vector<_BHF_File_> files;

		BHF(BSReader* file)
		{
			char magic[4];
			file->markPos();
			file->read(&magic[0],4);
			file->returnToMark();

			if(magic[4] == '3')
			{
				_BHF3_* temp = new _BHF3_(file);

				for(int32_t i = 0; i < temp->header.fileCount; i++)
				{
					_BHF_File_ bhfFile;
					bhfFile.id = temp->files[i].id;
					bhfFile.dataOffset = temp->files[i].dataOffset;
					bhfFile.compressedSize = temp->files[i].compressedSize;
					bhfFile.uncompressedSize = temp->files[i].uncompressedSize;
					bhfFile.nameOffset = temp->files[i].nameOffset;
					//TODO: name
					this->files.push_back(bhfFile);
				}
			}
			else if(magic[4] == '4')
			{
				//TODO: finish this part
			}
		};
	};
};