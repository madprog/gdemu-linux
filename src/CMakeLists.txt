add_library(gdemu
	GD.cpp)
include_directories(gdemu
	../include
	${SDL_INCLUDE_DIR})
target_link_libraries(gdemu
	${SDL_LIBRARY})
