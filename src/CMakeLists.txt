add_executable(CFRTEST
	main.cpp
	)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

if(BUILD STREQUAL "debug")
	#enable GDB and disable all optimizations
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -O0")
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

set_property(TARGET CFRTEST PROPERTY "$<TARGET_FILE_DIR:CFRTEST>")
set(COMPILE_OUTPUT_FOLDER ${CMAKE_SOURCE_DIR}/bin/release/)
target_include_directories(CFRTEST PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(CFRTEST)