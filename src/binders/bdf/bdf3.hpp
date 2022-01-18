#pragma once
#include "../stdafx.hpp"

namespace cfr
{
    class BDF3
    {
        char version[8];
        FILE* data;
        int size;

        BDF3(FILE* src)
        {
            this->data = src;

            char magic[4];
            fread(magic,1,4,src);
            assert(memcmp(magic,"BDF3",4));
            
            fread(version,1,8,src);

            fseek(src,0,SEEK_END);
            this->size = ftell(src);
            fseek(src,0,SEEK_SET);
        };
    };
};