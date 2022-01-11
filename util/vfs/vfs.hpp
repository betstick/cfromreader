#pragma once
#include "../stdafx.hpp"

//virtual file system for data loading

namespace cfr
{
	struct vnode
	{
		void* parent; //if null, its a physical file
		void* child; //if null, its a file with data
		uint64_t fileSize;
		char name[256];
	};
}