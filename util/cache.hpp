#pragma once
#include "../common/stdafx.hpp"
#ifndef CFR_CACHE
#define CFR_CACHE

namespace cfr
{
	struct Asset
	{
		char name[32];
		char type[8];
		char format[8];
		int64_t offset; //offset within the binder file
	};

	struct Binder
	{
		Asset assetList[128]; //this can be any number in theory
		uint64_t assetCount; //64 bit to keep it byte aligned for 64 bit systems
		
		char path[1024]; //path to the binder file in filesystem
		char name[32];
		char format[8];
		char compressionFormat[8];
	};

	struct AssetInfo //minimum data needed to open a specific file within a binder
	{
		//do NOT want to initialize a 512 word array if i don't need to.
		char binderPath[1024];
		char binderName[32];
		char binderFormat[8];
		char binderCompressionFormat[8];

		Asset asset;
	};

	class BinderAssetCache
	{
		//this database requires just under 16MB, it's word aligned! :)
		Binder binderList[2048]; //seems like a lot, it's not!
		uint64_t assetCount; 
		uint64_t binderCount;

		//binders cannot be removed. once they're in, they're in!
		void addBinder()
		{

		};

		//returns number of assets in the binder
		uint64_t indexBinder(uint64_t binderIndex)
		{
			return binderIndex;
		};

		/*AssetInfo findAsset(char binderName[32], int64_t assetIndex)
		{
			AssetInfo info;
			for(uint64_t b = 0; b < binderCount; b++)
			{
				if(strncmp(binderName, binderList[b].name, sizeof(binderName)))
				{
					//this is the fastest and WORST way to do this.
					//touching ANYTHING will break this. if it even works...
					//copies the memory in larger blockes using static values

					//the below will won't need to calc at runtime
					uint64_t offset = sizeof(Asset) + sizeof(uint64_t);
					uint16_t memsize = sizeof(AssetInfo) - sizeof(Asset);

					//gets the pointer to the binder and lets me apply offsets to the pointer
					unsigned char* bytePtr = reinterpret_cast<unsigned char*>(&binderList[b]);
					bytePtr += offset;
					memcpy(&info, bytePtr, memsize);

					offset = sizeof(AssetInfo) - sizeof(Asset);
					memsize = sizeof(Asset);

					bytePtr = reinterpret_cast<unsigned char*>(&info);
					bytePtr += offset;
					memcpy(bytePtr, &binderList[b].assetList[assetIndex], memsize);

					return info;
				}
			}
			throw std::runtime_error("Invalid file specified. Crashing!");
		};*/
	};

	std::string getMagicBytes(){};
};

#endif