#pragma once
#include "../stdafx.hpp"

namespace cfr
{
    class BHD5_1
    {
        struct Header
        {
            char magic[4]; // BHD5
            char unk04;
            char unk05;
            char unk06;
            char unk07;
            int32_t unk08; // 1
            int32_t fileSize;
            int32_t bucketCount;
            int32_t bucketsOffset; // 0x18
        };

        struct FileHeader
        {
            int32_t nameHash;
            int32_t fileSize;
            int64_t offset;
        };

        struct Bucket
        {
            int32_t fileHeaderCount;
            int32_t fileHeadersOffset;

            //might need to switch this to unordered map
            FileHeader* fileHeaders;
        };

        FILE* headerSrc;
        FILE* dataSrc;

        BHD5_1(FILE* headerSrc, FILE* dataSrc)
        {

        };

        //find specific file by hash
        int64_t getHashPosition(int32_t hash)
        {
            //return offset for a given hash
            //size *may* not be needed
        };
    };
};