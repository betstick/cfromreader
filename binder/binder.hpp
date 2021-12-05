#pragma once
#include "stdafx.hpp"

namespace cfr {
	struct File
	{
		
	};

	class Binder
	{
		private:
		uint64_t offset;
		char magicBytes[4];

		public:
		std::string format;


		Binder(FILE* file, uint64_t priorOffset)
		{
			fseek(file,offset,0);
			offset += priorOffset;
			init(file, offset);
		};

		Binder(std::string path)
		{
			FILE* ptr = fopen(path.c_str(),"rb");
			init(ptr, 0);
		};

		private:
		void init(FILE* file, uint64_t offset)
		{
			fread(&magicBytes,4,1,file);

			if(magicBytes[3] == '3')
			{
				format = "BND3";
				//init bnd3
			} 
			else if(magicBytes[3] == '4')
			{
				format = "BND4";
				//TODO set up bnd4 classes
			}
			else
			{
				//this print statement is hacky and stupid
				printf(
					"Binder format detected: %c%c%c%c\n",
					magicBytes[0],
					magicBytes[1],
					magicBytes[2],
					magicBytes[3]
				);
				throw std::runtime_error("Invalid binder format!\n");
			}
		};
	};
};