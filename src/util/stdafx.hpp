#pragma once
#include "../common/stdafx.hpp"

#ifdef WIN32
#	include "fmemopen_win/fmemopen_win.h"
#endif

#include "mem.hpp"

#include "hash_util.hpp"
#include "compression/zlib_inf.h"
#include "filetypes.hpp"