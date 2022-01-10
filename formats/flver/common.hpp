#pragma once
#include "../stdafx.hpp"

//TODO: determine if any more of the FLVERx structs can get merged

namespace cfr
{
	struct _FLVER_Dummy_
	{
		_Float32 position_x;
		_Float32 position_y;
		_Float32 position_z;

		uint8_t color[4];

		_Float32 forward_x;
		_Float32 forward_y;
		_Float32 forward_z;

		int16_t referenceID;
		int16_t dummyBoneIndex;

		_Float32 upward_x;
		_Float32 upward_y;
		_Float32 upward_z;
		
		int16_t attachBoneIndex;
		char unk2E;
		char useUpwardVector;

		int32_t unk30;
		int32_t unk34;
		int32_t unk38; //assert(0)
		int32_t unk3C; //assert(0)
	};
}