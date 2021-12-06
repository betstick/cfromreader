#pragma once
#include "stdafx.hpp"

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

struct hkxHeaderLayout 
{
    uint8_t bytesInPointer;
    uint8_t littleEndian;
    uint8_t reusePaddingOptimization;
    uint8_t emptyBaseClassOptimization;
};

struct hkxSectionDeader 
{
    struct hkxLocalFixup
    {
        int32_t pointer;
        int32_t destination;
    };

    struct hkxGlobalFixup
    {
        int32_t pointer;
        int32_t sectionID;
        int32_t destination;
    };

    struct hkxVirtualFixup
    {
        int32_t dataOffset;
        int32_t sectionID;
        int32_t classNameOffset;
    };

    uint32_t sectionID;
    std::string buffer;
    std::vector<hkxLocalFixup> localFixups;
    std::vector<hkxGlobalFixup> globalFixups;
};

struct hkxHeaderData 
{
    uint32_t magic1; //this is hacky
    uint32_t magic2; //but using ints works
    uint32_t userTag;
    uint32_t version;
    hkxHeaderLayout layout;
    int32_t numSections;
    int32_t contentsSectionIndex;
    int32_t contentsClassNameSectionIndex;
    int32_t contentsClassNameSectionOffset;
    char contentsVersion[16];
    uint32_t flags;
    int16_t maxPredicate;
    int16_t predicateArraySizePlugPadding;
};

//hktoolset sctruct/var is needed??!?!?!

class hkxOld
{
    private:
    hkxHeaderData headerData;
    hkToolset toolset;

    public:
    hkxOld(FILE* file, uint64_t offset)
    {
        fread(&headerData.magic1,4,1,file);
        fread(&headerData.magic2,4,1,file);

#ifdef DEBUG
        if(headerData.magic1 != 0x57e0e057)
            throw std::runtime_error("invalid HKX magic1");
        if(headerData.magic2 != 0x10c0c010)
            throw std::runtime_error("invalid HKX magic2");
#endif

        fread(&headerData.userTag,4,1,file);
        fread(&headerData.version,4,1,file);

        fread(&headerData.layout.bytesInPointer,1,1,file);
        fread(&headerData.layout.littleEndian,1,1,file);
        fread(&headerData.layout.reusePaddingOptimization,1,1,file);
        fread(&headerData.layout.emptyBaseClassOptimization,1,1,file);

        fread(&headerData.numSections,4,1,file);
        fread(&headerData.contentsSectionIndex,4,1,file);
        fread(&headerData.contentsClassNameSectionIndex,4,1,file);
        fread(&headerData.contentsClassNameSectionOffset,4,1,file);
        fread(&headerData.contentsVersion,16,1,file);
        fread(&headerData.flags,4,1,file);
        fread(&headerData.maxPredicate,2,1,file);

        if(headerData.maxPredicate = -1)
            fread(&headerData.predicateArraySizePlugPadding,2,1,file);
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

		toolset = convert();

		if(toolset == HKUNKVER)
			throw std::runtime_error("unkown version HK");
    };
};