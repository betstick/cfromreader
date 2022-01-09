#include "../../common/stdafx.hpp"

//not really needed anymore, replaced by upgraded bsreader
//leaving it for now. padding my loc.

namespace cfr
{
	//abstraction of memory and files to use one interace
	class Walker
	{
		public:
		bool isFile; //false means its mapped to memory

		private:
		BSReader file;
		std::vector<char> memory;
		uint64_t memPos; //position in memory

		public:
		Walker(){};

		Walker(BSReader* file)
		{

		};

		Walker(std::vector<char> data)
		{
			this->memory = data;
		};

		void read(void* dest, uint64_t readSize)
		{
			if(this->isFile)
			{
				this->file.read(dest,readSize);
			}
			else
			{
				memcpy(dest,&this->memory[memPos],readSize);
				memPos += readSize;
			}
		};
	};
}