include_directories(src)

file(GLOB_RECURSE SOURCES "*.cpp")

#install(TARGETS cfr DESTINATION /usr/lib)
install(FILES cfr.hpp DESTINATION include)
add_library(cfr SHARED ${SOURCES})
#set_target_properties(cfr PROPERTIES PUBLIC_HEADER cfromreader.hpp)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS 
	OWNER_READ OWNER_WRITE 
	GROUP_READ WORLD_READ )
