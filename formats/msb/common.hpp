#pragma once
#include "stdafx.hpp"

namespace cfr
{
	struct _MSB_Header_
	{
		char magic[4]; // MSB\0
		int32_t unk04; // 1
		int32_t headerSize; // 0x10
		int8_t isBigEndian; // 0
		int8_t isBitBigEndian; // 0
		int8_t textEncoding; // 1
		int8_t is64BitOffset; // -1
	};

	struct _MSB_Param_
	{
		int32_t version;

		//////////////////////////////////////////////////////////////////////
		int32_t count; //this before nameoffset if VARINT_LONG otherwise flip
		uint64_t nameOffset; //if VARINT_LONG is true, 32 bit, otherwise 64
		//////////////////////////////////////////////////////////////////////

		
	};
};