cmake_minimum_required(VERSION 2.8)

project(time-wheel)

aux_source_directory(src SRC)

add_executable(Timer ${SRC})
