add_library (Game Game.cpp)
add_executable (../JumpingCubesClassic main.cpp)

target_link_libraries (../JumpingCubesClassic Game SDL)
