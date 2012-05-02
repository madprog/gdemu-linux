include_directories(../include)

add_executable(scroll
	scroll.cpp)
target_link_libraries(scroll
	gdemu)

add_executable(palettes
	palettes.cpp)
target_link_libraries(palettes
	gdemu)

add_executable(sprites256
	sprites256.cpp)
target_link_libraries(sprites256
	gdemu)
