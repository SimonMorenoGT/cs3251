cmake_minimum_required(VERSION 3.14)
project(project1 C)

set(CMAKE_C_STANDARD 11)

add_executable(client
        client.c
        )
add_executable(server
        server.c
        )

