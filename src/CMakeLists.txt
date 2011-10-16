find_package(Qt4)

if(NOT QT4_FOUND)
	find_package(SDL REQUIRED)
	include_directories(${SDL_INCLUDE_DIR})

	if(APPLE)
		set(JCC_SOURCES SDLMain.m)
	endif(APPLE)

	set(LIBS ${SDL_LIBRARY})
	set(JCC_SOURCES ${JCC_SOURCES} main_SDL.cpp)
else
	set(JCC_SOURCES ${JCC_SOURCES} main_Qt.cpp)
endif(NOT QT4_FOUND)

set(JCC_SOURCES ${JCC_SOURCES} Game.cpp)

add_executable (../JumpingCubesClassic ${JCC_SOURCES})

target_link_libraries (../JumpingCubesClassic ${LIBS})
