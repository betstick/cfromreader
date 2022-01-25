#pragma once

//generic types
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <cstdint>
#include <stdlib.h>
#include <vector>
#include <cstdlib>
#include <bits/stdc++.h>
#include <wchar.h>

//memory manipulation
#include <memory>

//testing and verification
#include <assert.h>

//compression libraries
#include <zlib.h>

//data streams sound really cool
#include <iostream>
#include <fstream> 

//error handling
#include <exception>
#include <stdexcept>

#ifdef WIN32
#	include "fmemopen_win/fmemopen_win.h"
#endif

#include "mem.hpp"

#include "hash_util.hpp"
#include "compression/zlib_inf.h"
#include "filetypes.hpp"