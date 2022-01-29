#pragma once

#include <assert.h>
#include "stdio.h"
#include "zlib.h"
#include "../memr/memr.hpp"

#ifndef CHUNK
#	define CHUNK 16384
#endif

namespace cfr
{
	int inf(MEM *source, MEM *dest);
};