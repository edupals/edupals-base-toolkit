cmake_minimum_required(VERSION 3.0)

project(edupals-base-toolkit)
add_subdirectory(src)

#install headers
install(DIRECTORY include/ DESTINATION include/edupals/base)


