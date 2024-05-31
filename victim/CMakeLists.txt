cmake_minimum_required(VERSION 3.22.1)
project(NastyJuck C)

set(CMAKE_C_STANDARD 11)

add_executable(NastyJuck main.c
        server.c)
