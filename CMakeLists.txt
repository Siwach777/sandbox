cmake_minimum_required(VERSION 3.20)
project(world_sim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -Wall -Wextra -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(world_sim
	src/main.cpp
	src/Game.cpp
	src/Grid.cpp
)

target_include_directories(world_sim PRIVATE
    include
)

target_link_libraries(world_sim PRIVATE
	SFML::Graphics
	SFML::Window
	SFML::System
)