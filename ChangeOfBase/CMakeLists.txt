cmake_minimum_required(VERSION 3.9)
project(ChangeOfBase C)

set(CMAKE_C_STANDARD 11)
include_directories(/usr/local/Cellar/gmp/6.1.2_1/include/)

add_executable(ChangeOfBase ChangeOfBase.c)