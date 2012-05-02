cmake_minimum_required(VERSION 2.6)
project(gdemu)

set(CMAKE_CXX_FLAGS "-g -Wall")

find_package(SDL)

subdirs(src
	examples)
