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

add_executable(rotate
	rotate.cpp)
target_link_libraries(rotate
	gdemu)

add_executable(collision
	collision.cpp)
target_link_libraries(collision
	gdemu)
