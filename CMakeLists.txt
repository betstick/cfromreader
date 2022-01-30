cmake_minimum_required(VERSION 3.10)

project(cfr VERSION 0.0.1 DESCRIPTION "cfr 001")

enable_language(CXX)

include(GNUInstallDirs)

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

include_directories(src)

#file(GLOB_RECURSE SOURCES "*.cpp")
set(SOURCES 
	"util/hashing/hash_util.cpp"
	"util/filetypes.cpp"
	"util/compression/zlib_inf.cpp"
	"util/memr/memr.cpp"
	"binders/bdf/bdf3.cpp"
	"binders/bhd/bhd5_1.cpp"
	"binders/bnd/bnd3.cpp"
	"binders/dcx/dcx.cpp"
	"formats/flver/flver2.cpp"
)

add_library(cfr SHARED ${SOURCES})


set(MY_LIB_HEADER "cfr.hpp")
set(PUBLIC_HEADERS "cfr.hpp" )

set_target_properties(cfr PROPERTIES 
	PUBLIC_HEADER ${PUBLIC_HEADERS}
	RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})


#set_target_properties(cfr PROPERTIES PUBLIC_HEADER cfr.hpp)
#target_include_directories(cfr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(cfr PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
#target_include_directories(cfr PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/..")

install(TARGETS cfr LIBRARY DESTINATION /usr/lib)
install(FILES cfr.hpp DESTINATION include)

install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	PERMISSIONS 
	OWNER_READ OWNER_WRITE 
	GROUP_READ WORLD_READ )