#pragma once
#include "../stdafx.hpp"

namespace cfr
{
	struct _BDF3_Header_
	{
		char magic[4]; //assert BDF3
		char version[8];
		int32_t unk0C; //assert 0
	};

	//need BHF3 file to function
	class _BDF3_
	{
		public:
		_BDF3_Header_ header;

		_BDF3_(){};

		_BDF3_(BSReader* file)
		{
			file->read(&this->header.magic[0],16); //magic thru unk0C
		};
	};
}