#pragma once
#include "stdafx.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace cfr
{
	enum GameHashList
	{
		des,
		ds_ptde,
		ds_r,
		ds_2,
		ds_2_sotfs,
		ds_3,
		bb,
		sek,
		er
	};
	
	//opens file list for specific game and hashes on the fly for DVDBND support
	std::vector<std::string> getFileList(GameHashList list)
	{
		std::vector<std::string> paths;
		std::string path = "res/file_lists/";

		switch(list)
		{
			case ds_ptde:
				path.append("ds_ptde.text"); break;
			default:
			{
				//TODO: fix this so it tells you what you input
				char* listChar;
				//char* thing = itoa((int)list,listChar,10);
				throw std::invalid_argument("getHashList Unsupported argument: \n");
			}
		}

		//open file and read to memory for performance
		FILE* listFile = fopen(path.c_str(),"rb");

		if(listFile == NULL)
			throw std::runtime_error("Failed to open file list.\n");

		fseek(listFile,0,SEEK_END);
		int listFileSize = ftell(listFile);
		fseek(listFile,0,SEEK_SET);

		char* fileData;
		fileData = (char*)malloc(listFileSize);
		fread(fileData,listFileSize,1,listFile);
		fclose(listFile);

		FILE* listMem = fmemopen(fileData,listFileSize,"rb");
		
		if(listMem == NULL)
			throw std::runtime_error("Failed to map file list memory.\n");

		char* line;
		size_t lineSize = 128;
		size_t characters;

		line = (char*)malloc(lineSize * sizeof(char));

		while(feof(listMem) != 1 || ferror(listMem) != 0)
		{
			characters = getline(&line,&lineSize,listMem);

			std::string filePath = line;

			paths.push_back(filePath);
		}

		fclose(listMem);

		return paths;
	}


	uint32_t hashFilePath(std::string path)
	{
		uint32_t hash = 0;

		//minus 1 to account for newline char? null char won't affect hash
		for(int i = 0; i < path.size(); i++)
		{
			switch(path[i])
			{
				case '\n': break;
				//case '\0': break;
				default:
				{
					hash *= (uint32_t)37;
					hash += (uint32_t)path[i];
					//printf("h:%12.u 0x%12.x\n",hash,hash);
				}
			}
		}

		return hash;
	};
};