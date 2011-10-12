find_package(SDL REQUIRED)
find_package(SDL_net REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

set(JCC_SOURCES main.cpp Game.cpp)

if(APPLE)
	set(JCC_SOURCES ${JCC_SOURCES} SDLMain.m)
endif(APPLE)

add_executable (../JumpingCubesClassic ${JCC_SOURCES})

target_link_libraries (../JumpingCubesClassic ${SDL_LIBRARY} SDL_net)
