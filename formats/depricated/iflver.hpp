#pragma once
#include "stdafx.hpp"

namespace cfr {
	struct iUV
	{
		_Float32 pos_x,pos_y,pos_z;
	};

	struct iTan
	{
		_Float32 w,x,y,z;
	};

	struct iVertex
	{
		_Float32 pos_x,pos_y,pos_z;
		_Float32 nor_x,nor_y,nor_z;

		_Float32 boneWeight_w,boneWeight_x,boneWeight_y,boneWeight_z;
		_Float32 boneIndice_w,boneIndice_x,boneIndice_y,boneIndice_z;

		_Float32 bitan_w,bitan_x,bitan_y,bitan_z;

		int8_t r,g,b,a;
	};

	struct iMesh
	{
		iVertex* vertices;
	};

	struct iBone
	{

	};

	//intermediary flver format, temporary
	class iflver
	{
		
	};
};