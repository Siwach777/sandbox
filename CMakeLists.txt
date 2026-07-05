cmake_minimum_required(VERSION 3.20)
project(world_sim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -Wall -Wextra -fsanitize=address -Wswitch,undefined")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

include(FetchContent)

FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.91.8
	GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
	imgui-SFML
	GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
	GIT_TAG v3.0
	GIT_SHALLOW TRUE 
)
FetchContent_MakeAvailable(imgui-sfml)

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
	src/Systems/BehaviorSystem.cpp
	src/Systems/InteractionSystem.cpp
	src/Systems/PheromoneRenderSystem.cpp
	src/Systems/PheromoneSenseSystem.cpp
	src/UI/DebugPanel.cpp
	src/World/PheromoneGrid.cpp
)

target_include_directories(world_sim PRIVATE
    include
	src
)

target_link_libraries(world_sim PRIVATE
	SFML::Graphics
	SFML::Window
	SFML::System
	ImGui-SFML::ImGui-SFML
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})