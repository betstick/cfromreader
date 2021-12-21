#pragma once
#include "stdafx.hpp"

namespace cfr {
	//LONG
	enum hkToolset {
		HKUNKVER, 
		HK500, 
		HK510,
		HK550,
		HK600,
		HK610,
		HK650,
		HK660,
		HK700,
		HK710,
		HK2010_1,
		HK2010_2,
		HK2011_1,
		HK2011_2,
		HK2011_3,
		HK2012_1,
		HK2012_2,
		HK2013,
		HK2014,
		HK2015,
		HK2016,
		HK2017
	};

	/*class hkxHeaderLayout
	{
		public:
		uint8_t bytesInPointer; //num of bytes in a pointer
		uint8_t littleEndian;
		uint8_t reusePaddingOptimization;
		uint8_t emptyBaseClassOptimization;

		hkxHeaderLayout(FILE* file, uint64_t offset)
		{
			fread(&bytesInPointer,1,1,file);
			fread(&littleEndian,1,1,file);
			fread(&reusePaddingOptimization,1,1,file);
			fread(&emptyBaseClassOptimization,1,1,file);
		};
	};*/

	class hkxHeader
	{
		public:
		uint32_t magic1 = 0; //this is hacky
		uint32_t magic2 = 0; //but using ints works
		uint32_t userTag = 0;
		uint32_t version = 0;

		//hkxHeaderLayout layout = {0,0,0,0};
		uint8_t bytesInPointer = 0; //num of bytes in a pointer
		uint8_t littleEndian = 0;
		uint8_t reusePaddingOptimization = 0;
		uint8_t emptyBaseClassOptimization = 0;

		int32_t numSections = 0;
		int32_t contentsSectionIndex = 0;
		int32_t contentsSectionOffset = 0; //may have been misssing this
		int32_t contentsClassNameSectionIndex = 0;
		int32_t contentsClassNameSectionOffset = 0;

		char contentsVersion[16] = {'0','0','0','0','0','0','0','0',
									'0','0','0','0','0','0','0','0'};
		uint32_t flags = 0;
		int16_t maxPredicate = 0;
		int16_t predicateArraySizePlusPadding = 0; //section offset

		uint32_t unk40 = 0;
		uint32_t unk44 = 0;
		uint32_t unk48 = 0;
		uint32_t unk4C = 0;

		//this is the ONLY class that needs a default header????
		hkxHeader(){}; //this just shouldn't be needed :/ but it is. //TODO: get rid of this!
		hkxHeader(FILE* file, hkToolset version)
		{
			fread(&magic1,4,1,file);
			fread(&magic2,4,1,file);

#ifdef DEBUG
			if(magic1 != 0x57e0e057)
				throw std::runtime_error("invalid HKX magic1");
			if(magic2 != 0x10c0c010)
				throw std::runtime_error("invalid HKX magic2");
#endif

			fread(&userTag,4,1,file);
			fread(&version,4,1,file);

			//layout = hkxHeaderLayout(file, 0);
			fread(&bytesInPointer,1,1,file);
			fread(&littleEndian,1,1,file);
			fread(&reusePaddingOptimization,1,1,file);
			fread(&emptyBaseClassOptimization,1,1,file);

			fread(&numSections,4,1,file); //usually 3 for souls games
			fread(&contentsSectionIndex,4,1,file);
			fread(&contentsSectionOffset,4,1,file);
			fread(&contentsClassNameSectionIndex,4,1,file);
			fread(&contentsClassNameSectionOffset,4,1,file);
			fread(&contentsVersion,16,1,file);

			fread(&flags,4,1,file);

			//TODO: translate this to use toolset version
			if(version >= 0x0B) //newer version of hkx
			{
				fread(&maxPredicate,2,1,file);
				if(maxPredicate == -1)
					fread(&predicateArraySizePlusPadding,2,1,file); //unk3c
				
				fread(&predicateArraySizePlusPadding,2,1,file); //section offset
				fread(&unk40,2,1,file);
				fread(&unk44,2,1,file);
				fread(&unk48,2,1,file);
				fread(&unk4C,2,1,file);

				fseek(file, predicateArraySizePlusPadding, SEEK_SET);
			}
			else //older version of hkx
			{
				fread(nullptr,4,1,file); //skips four bytes ahead :P
			}
		};
	};

	class hkxLocalFixup
	{
		public:
		uint32_t pointer;
		uint32_t destination;

		hkxLocalFixup(FILE* file)
		{
			fread(&pointer,4,1,file);
			fread(&destination,4,1,file);
		};
	};

	class hkxGlobalFixup
	{
		public:
		uint32_t pointer;
		uint32_t sectionIndex;
		uint32_t destination;

		hkxGlobalFixup(FILE* file, uint64_t offset)
		{
			fread(&pointer,4,1,file);
			fread(&sectionIndex,4,1,file);
			fread(&destination,4,1,file);
		};
	};

	class hkxVirtualFixup
	{	
		public:
		uint32_t pointer;
		uint32_t sectionIndex;
		uint32_t classNameOffset;

		hkxVirtualFixup(FILE* file)
		{
			fread(&pointer,4,1,file);
			fread(&sectionIndex,4,1,file);
			fread(&classNameOffset,4,1,file);
		};
	};

	class hkxObject
	{
		public:
		uint32_t sectionOffset; //offset within section where object is stored
		uint32_t dataSize; //size of serialized object
	};

	//may only be used for writing
	/*class hkxLocalReference
	{
		public:
		hkxSection section;
		
		uint32_t sourceLocalOffset;
		uint32_t destLocalOffset;
		
		hkxObject sourceObject;
		hkxObject destObject;
	};

	//may only be used for writing
	class hkxGlobalReference
	{
		public:
		hkxSection sourceSection;
		hkxSection destSection;
		
		uint32_t sourceLocalOffset;
		uint32_t destLocalOffset;
		
		hkxObject sourceObject;
		hkxObject destObject;
	};*/



	class hkxClassName
	{
		public:
		uint32_t signature;
		char className[32]; //i sure hope its not more than 32 :^)
		uint32_t sectionOffset;

		hkxClassName(FILE* file, hkToolset version)
		{
			fread(&signature,4,1,file);
			if(fgetc(file) != 0x09)
				throw std::runtime_error("hkxclassname didn't have a 9 in it");
			sectionOffset = ftell(file);
			
			uint64_t i = 0;
			//like in bnd3.hpp, i still don't fully get how this works
			while(className[i-1] != 0 || i == 0)
			{
				fread(&className[i],1,1,file);
				i++;
			}
			//maybe i'm just dumb. 
			//TODO: make it a function later i suppose.
		};
	};

	class hkxClassNames : public hkxObject
	{
		public:
		//offset[n] refers to the offset of classNames[n]
		//original code uses dictionary, i don't have those
		std::vector<hkxClassName> classNames;
		std::vector<uint64_t> offsets;

		hkxClassNames(FILE* file, uint64_t offset, hkToolset version)
		{
			uint16_t temp;
			//this line is ugly and may not work.
			while((fread(&temp,2,1,file) == sizeof(uint16_t))&&(temp != 0xFFFF))
			{
				fseek(file,-2,SEEK_CUR);
				offsets.push_back(ftell(file) + 5);
				classNames.push_back(hkxClassName(file,offset,version));
			}
		};
	};

	class hkxVirtualReference
	{
		public:
		hkxObject* sourceObject; //
		void* destSection;  //hkxSection, would cause dep loop
		hkxClassName className; //
	};

	class hkxSection 
	{
		public:
		int32_t sectionID; //unused???
		char sectionTag[20]; //last char should be null

		uint32_t dataStartOffset;
		uint32_t localFixupsOffset;
		uint32_t globalFixupsOffset;
		uint32_t virtualFixupsOffset;
		
		uint32_t exportsOffset;
		uint32_t importsOffset;
		uint32_t endOffset;

		char* data; //should be size localFixupsOffset

		std::vector<hkxLocalFixup> localFixups;
		std::vector<hkxGlobalFixup> globalFixups;
		std::vector<hkxVirtualFixup> virtualFixups;

		//these may not be needed for read only mode
		/*std::vector<hkxLocalReference> localReferences;
		std::vector<hkxGlobalReference> hkxGlobalReferences;
		std::vector<hkxVirtualReference> hkxVirtualReferences;*/

		std::vector<hkxObject> objects;

		hkxSection(FILE* file, uint64_t offset, hkToolset version)
		{
			//fread(&sectionID,4,1,file); //TODO: determine if this is needed
			fread(&sectionTag,20,1,file);

			fread(&dataStartOffset,4,1,file);
			fread(&localFixupsOffset,4,1,file);
			fread(&globalFixupsOffset,4,1,file);
			fread(&virtualFixupsOffset,4,1,file);

			fread(&exportsOffset,4,1,file);
			fread(&importsOffset,4,1,file);
			fread(&endOffset,4,1,file);

			fpos64_t pos;
			fgetpos64(file, &pos);

			fseek(file, localFixupsOffset, SEEK_SET);
			fread(data,localFixupsOffset,1,file); //not sure if this is valid?
			
			fsetpos64(file, &pos);

			//these three blocks should be made into a function
			//TODO: compare to havok libs and restructure the code
			fgetpos64(file, &pos);
			fseek(file, dataStartOffset + localFixupsOffset, SEEK_SET);
			for(uint32_t i = 0; i < (dataStartOffset - localFixupsOffset); i++)
			{
				uint32_t temp;
				fread(&temp,4,1,file);
				if(temp != 0xFFFFFFFF)
				{
					fseek(file,-4,SEEK_CUR); //go back four bytes
					localFixups.push_back(hkxLocalFixup(file, 0));
				}
			}
			fsetpos64(file, &pos);

			fgetpos64(file, &pos);
			fseek(file, dataStartOffset + globalFixupsOffset, SEEK_SET);
			for(uint32_t i = 0; i < (dataStartOffset - globalFixupsOffset); i++)
			{
				uint32_t temp;
				fread(&temp,4,1,file);
				if(temp != 0xFFFFFFFF)
				{
					fseek(file,-4,SEEK_CUR); //go back four bytes
					globalFixups.push_back(hkxGlobalFixup(file, 0));
				}
			}
			fsetpos64(file, &pos);

			fgetpos64(file, &pos);
			fseek(file, dataStartOffset + globalFixupsOffset, SEEK_SET);
			for(uint32_t i = 0; i < (dataStartOffset - virtualFixupsOffset); i++)
			{
				uint32_t temp;
				fread(&temp,4,1,file);
				if(temp != 0xFFFFFFFF)
				{
					fseek(file,-4,SEEK_CUR); //go back four bytes
					virtualFixups.push_back(hkxVirtualFixup(file, 0));
				}
			}
			fsetpos64(file, &pos);

			//bloodborne is version 2014 and so is DS3
			if(version == HK2014)
			{
				uint32_t temp;

				for(uint32_t i = 0; i < 4; i++)
				{
					fread(&temp,4,4,file);
					if(temp != 0xFFFFFFFF)
						throw std::runtime_error("no buffer at end of hkx section header :(");
				}
			}

			//these likely aren't needed
			//localRefrence vector
			//globalReference vector
			//virtualReference vector
		};
		void readClassNames(FILE* file, uint64_t offset, hkToolset version, int32_t sectionDataInput)
		{
			//goto the section data itself, its a location in the file!
			fseek(file,sectionDataInput,SEEK_SET);
			//TODO: confirm this works
			objects.push_back(hkxClassNames(file,offset,version));
		};

		/*void readDataObjects(FILE* file, uint64_t offset, hkToolset version, int32_t sectionDataInput)
		{
			for(uint64_t i = 0; i < virtualFixups.size(); i++)
			{

			}
		};*/
	};

	//hktoolset sctruct/var is needed??!?!?!

	class hkx
	{
		public:
		hkToolset version; //set it to something good as a default!
		private:
		hkxHeader headerData;
		std::vector<hkxSection> sections;

		//hkToolset toolset; //shouldn't need this

		public:
		hkx(FILE* file, uint64_t offset)
		{
			//read headers
			headerData = hkxHeader(file, offset, version);

			//find out what version the file is
			determineVersion(); //sets the version variable.

			//initialize the sections (usually 3)
			for(uint32_t s = 0; s > headerData.numSections; s++)
			{
				hkxSection tempSection = hkxSection(file, 0, version);
				sections.push_back(tempSection);
			};

			//set up the classnames?
			sections[0].readClassNames(file, offset, version, headerData.predicateArraySizePlusPadding);
			//deserialize the objects
			sections[1].readDataObjects(file, offset, version, headerData.predicateArraySizePlusPadding);
		};

		private:	
		void determineVersion()
		{
			uint32_t versions[3] = {};
			char* cc = headerData.contentsVersion;
			size_t cVer = 0;

			while (cc && *cc && cVer < 3) {
				if (isdigit(*cc)) {
					char *endPtr = nullptr;
					versions[cVer++] = std::strtol(cc, &endPtr, 10);
					cc = endPtr;
				} else {
					cc++;
				}
			}

			auto convert = [&]() {
				switch (versions[0]) 
				{
					case 5: {
						switch (versions[1]) {
							case 0 : return HK500;
							case 1 : return HK510;
							case 5 : return HK550;
						}
						return HKUNKVER;
					}

					case 6: {
						switch (versions[1]) {
							case 0 : return HK600;
							case 1 : return HK610;
							case 5 : return HK650;
							case 6 : return HK660;
						}
						return HKUNKVER;
					}

					case 7: {
						switch (versions[1]) {
							case 0 : return HK700;
							case 1 : return HK710;
						}
						return HKUNKVER;
					}

					case 2010: {
						switch (versions[1]) {
							case 1 : return HK2010_1;
							case 2 : return HK2010_2;
						}
						return HKUNKVER;
					}

					case 2011: {
						switch (versions[1]) {
							case 1 : return HK2011_1;
							case 2 : return HK2011_2;
							case 3 : return HK2011_3;
						}
						return HKUNKVER;
					}

					case 2012: {
						switch (versions[1]) {
							case 1 : return HK2012_1;
							case 2 : return HK2012_2;
						}
						return HKUNKVER;
					}

					case 2013: {
						switch (versions[1]) {
							case 1 : return HK2013;
						}
						return HKUNKVER;
					}

					case 2014: {
						switch (versions[1]) {
							case 1 : return HK2014;
						}
						return HKUNKVER;
					}

					default:
					return HKUNKVER;
				}
			};

			hkToolset toolset = convert();

			if(toolset == HKUNKVER)
				throw std::runtime_error("unkown version HK");
		};
	};
};