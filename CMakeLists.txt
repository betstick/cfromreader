cmake_minimum_required(VERSION 3.10)
set(CMAKE_INSTALL_PREFIX /usr)
project(CFRTEST)

message(STATUS ${PROJECT_SOURCE_DIR})

enable_language(CXX)
include(GNUInstallDirs)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
link_libraries(${ZLIB_LIBRARIES})

macro(add_dir DIRS)
    foreach (dir ${DIRS})
        message("adding  ${dir} to ${PROJECT_NAME}")
        include_directories(${dir})
        file(GLOB ${dir}__INCLUDES_H ${dir} ${dir}/*.h)
        file(GLOB ${dir}__INCLUDES_HPP ${dir} ${dir}/*.hpp)
        list(APPEND ${PROJECT_NAME}__INCLUDES ${${dir}__INCLUDES_H} ${${dir}__INCLUDES_HPP})
        file(GLOB ${dir}__SOURCES_CPP ${dir} ${dir}/*.cpp ${dir}/*.cxx)
        file(GLOB ${dir}__SOURCES_C ${dir} ${dir}/*.c)
        list(APPEND ${PROJECT_NAME}__SOURCES ${${dir}__SOURCES_C} ${${dir}__SOURCES_CPP})
    endforeach()
endmacro()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_TYPE x64)
else (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_TYPE x86)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/release/)

#copy the filelists needed for dvdbnds
file(COPY "src/res/" DESTINATION "release/res/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

list(APPEND DIRS
    "."
    )

add_dir("${DIRS}")
add_dir("src/")

add_subdirectory(src)