add_executable(snake
	main.cpp
	snake.cpp
	walls.cpp)
include_directories(gdemu
	../include)
target_link_libraries(snake
	gdemu)
