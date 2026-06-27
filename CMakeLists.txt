cmake_minimum_required(VERSION 3.20)
project(world_sim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -Wall -Wextra -fsanitize=address -Wswitch,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(world_sim
	src/main.cpp
	src/Game.cpp
	src/Grid.cpp
	src/Camera.cpp
	src/HUD.cpp
	src/ECS/World.cpp
	src/ECS/EntityFactory.cpp
	src/Systems/RenderSystem.cpp
	src/Systems/WanderSystem.cpp
	src/Systems/MovementSystem.cpp
)

target_include_directories(world_sim PRIVATE
    include
	src
)

target_link_libraries(world_sim PRIVATE
	SFML::Graphics
	SFML::Window
	SFML::System
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})