#ifndef CFR_ZLIB_INF__
#define CFR_ZLIB_INF__

#pragma once
#include "../stdafx.hpp"

#ifndef CHUNK
#	define CHUNK 16384
#endif

int inf(FILE *source, FILE *dest);

#endif