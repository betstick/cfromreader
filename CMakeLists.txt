cmake_minimum_required(VERSION 3.10)
project(cfr VERSION 0.0.1 DESCRIPTION "cfr 001")

include(GNUInstallDirs)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(WIN32)
	add_compile_definitions(_WIN32)
endif()

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
link_libraries(${ZLIB_LIBRARIES})

#copy the filelists needed for dvdbnds
file(COPY "res/" DESTINATION "release/res/")

include_directories(${CMEM_DIR})
#link_directories(${CMEM_DIR})

set(SOURCES
	${CMEM_DIR}/cmem.c
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

add_library(cfr SHARED ${SOURCES}) #"cfr.hpp")

#set(MY_LIB_HEADER "cfr.hpp")
#set(PUBLIC_HEADERS "cfr.hpp" )

#set_target_properties(cfr PROPERTIES 
#	PUBLIC_HEADER ${PUBLIC_HEADERS}
#	RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

#target_link_libraries(cfr ${CMEM_DIR}/cmem.h)
#link_libraries(${CMEM_DIR}/cmem.c)
target_include_directories(cfr PUBLIC .)
#target_include_directories(cfr PUBLIC ${CMEM_DIR})