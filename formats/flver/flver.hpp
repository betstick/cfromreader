#pragma once
#include "stdafx.hpp"
//welcome to FLVER-town

//unified FLVER0/FLVER2 class for easier data access.

namespace cfr {
	struct FLVER_Mesh
	{
		//need easy to use way to get all relevant data
		//maybe with LODs?
	};

	class FLVER
	{
		public:
		//vertex buffers
		//vertex layouts
		//textures
		//face sets

		FLVER(){};

		FLVER(BSReader* file)
		{

		};

		//done without copying any data
		FLVER(BSReader* file, uint64_t startLocation)
		{
			file->seek(startLocation);
			//figure out some way to determine 0 vs 2
			
		};
	};
};