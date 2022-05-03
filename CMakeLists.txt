cmake_minimum_required(VERSION 3.10)
project(cfr VERSION 0.0.1 DESCRIPTION "cfr 001")

include(GNUInstallDirs)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  add_compile_definitions(_CLANG)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  add_compile_definitions(_GNU)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
  add_compile_definitions(_INTEL)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  add_compile_definitions(_MSCV)
endif()

if(WIN32)
	add_compile_definitions(_WIN32)
endif()

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
link_libraries(${ZLIB_LIBRARIES})

set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")

#copy the filelists needed for dvdbnds
file(COPY "res/" DESTINATION "release/res/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

if(BUILD STREQUAL "debug")
	#enable GDB and disable all optimizations
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0 -g")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -O0 -g")
	#show all types of errors, but limit to 8 at a time
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fmax-errors=8")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -fmax-errors=8")
	#enable this for my debug code preprocessor stuff
	add_compile_definitions(DEBUG)
	#needed to prevent VMA werror stuff from stopping compile.
	add_compile_definitions(VMA_DEBUG_DETECT_CORRUPTION)
	add_compile_definitions(VMA_DEBUG_MARGIN=1)
	message(STATUS "Using debug mode...")
else()
	set(BUILD "release")
	#optimize aggressively
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
	message(STATUS "Using release mode...")
endif()
unset(BUILD CACHE)

include_directories(${CMEM_DIR})

set(SOURCES
	"util/files/files.cpp"
	"util/strings/strings.cpp"
	"util/hashing/hash_util.cpp"
	"util/filetypes.cpp"
	"util/compat/compat.cpp"
	"util/compression/zlib_inf.cpp"
	"binders/bdf/bdf3.cpp"
	"binders/bhd/bhd5_1.cpp"
	"binders/bnd/bnd3.cpp"
	"binders/dcx/dcx.cpp"
	"formats/flver/flver2.cpp"
	"formats/paramdef/paramdef.cpp"
)

add_library(cfr ${SOURCES} "cfr.hpp")

target_include_directories(cfr PUBLIC .)