if(BUILD_TEST STREQUAL "true")
	project(cfr_test)
	add_executable(cfr_test test/test.cpp)
endif()
unset(BUILD_TEST CACHE)

add_library(cfr SHARED cfromreader.cpp)
set_target_properties(cfr PROPERTIES PUBLIC_HEADER cfromreader.hpp)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE 
    GROUP_READ 
    WORLD_READ )