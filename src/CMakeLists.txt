if(BUILD_TEST STREQUAL "true")
	project(cfr_test)
	add_executable(cfr_test test/test.cpp)
endif()
unset(BUILD_TEST CACHE)

add_library(cfr SHARED cfromreader.cpp)
set_target_properties(cfr PROPERTIES PUBLIC_HEADER cfromreader.hpp)

install(TARGETS cfr DESTINATION /usr/lib)
install(FILES cfr.hpp DESTINATION include)