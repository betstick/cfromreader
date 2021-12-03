#pragma once
#include "stdafx.hpp"

#ifndef DCX_FORMAT
#define DCX_FORMAT

namespace cfr {
	class DCX 
	{
		std::string name; //name of the DCX
		//filedata
		char magic[4];
		int32_t unk04; //asset 0x10000 or 0x11000
		int32_t dcsOffset; //assert 0x18
		int32_t dcpOffset; //assert 0x24
		int32_t unk10; //assert 0x24 or 0x44
		int32_t unk14; //unkown
		char dcs[4]; //assert DCS\0
		uint32_t uncompressedSize;
		uint32_t compressedSize;
		char dcp[4]; //assert DCX\0
		char format[4]; //asssert DFLT, EDGE, or KRAK
		int32_t unk2C; //asset 0x20
		char unk30; //assert 6, 8, or 9
		char unk31; //assert 0
		char unk32; //assert 0
		char unk33; //assert 0
		int32_t unk34; //assert 0
		int32_t unk38; //assert 0
		int32_t unk3C; //assert 0
		int32_t unk40; //unkown
		char dca[4]; //assert DCA\0
		int32_t dcaSize; //from before "DCA" to dca end //not sure what this means?

		void read(std::string assetName, ){};
	};
};

#endif