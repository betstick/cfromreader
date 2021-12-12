#pragma once
#include "stdafx.hpp"

namespace cfr {
	class BinderFileHeader
	{
		public:
		char name[256];//assets must fit in win32 path size limit, saves mem.
		uint64_t dataOffset; //relative to the binder?, support 64 bit
		uint64_t sizeUncompressed;
		//probably don't need ID?

		BinderFileHeader(char* nameIn, uint64_t sizeIn, uint64_t offsetIn)
		{
			dataOffset = offsetIn;
			memcpy(&name,nameIn,256);
			sizeUncompressed = sizeIn;
		};
	};

	class Binder
	{
		private:
		uint64_t offset;
		char magicBytes[4];

		public:
		std::string format; //technically dupe of magicBytes. easier to use.
		uint64_t fileCount;
		std::vector<BinderFileHeader> fileHeaders;

		Binder(BSReader* file, uint64_t priorOffset)
		{
			file->seek(offset,0);
			offset += priorOffset;
			init(file, offset);
		};

		Binder(std::string path)
		{
			char buffer[4096];
			BSReader file = BSReader(path,buffer,4096);
			init(&file, 0); //this one is differet, its okay
		};

		private:
		void init(BSReader* file, uint64_t offset)
		{
			file->read(&magicBytes,4);

			if(magicBytes[3] == '3')
			{
				format = "BND3";
				//init bnd3
				BND3 tempBinder = BND3(file,offset);

				for(uint32_t i = 0; i < tempBinder.fileHeaders.size(); i++)
				{
					//printf("%.256s\n",tempBinder.fileHeaders[i].name);
					BinderFileHeader tempFileHeader = BinderFileHeader(tempBinder.fileHeaders[i].name, tempBinder.fileHeaders[i].uncompressedSize, tempBinder.fileHeaders[i].dataOffset);
					fileHeaders.push_back(tempFileHeader);
				}
			} 
			else if(magicBytes[3] == '4')
			{
				format = "BND4";
				//TODO: set up bnd4 classes, *later*
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