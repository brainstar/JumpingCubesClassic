find_package(Qt4)
find_package(SDL)

set (CMAKE_HAVE_QT false)
if (QT4_FOUND)
	set(CMAKE_HAVE_QT true)
endif (QT4_FOUND)

set(CMAKE_HAVE_SDL false)
if (SDL_FOUND)
	set(CMAKE_HAVE_SDL true)
endif (SDL_FOUND)

configure_file(${JCC_SOURCE_DIR}/src/renderers.h.in src/renderers.h)
include_directories(${JCC_BINARY_DIR}/src)

set(JCC_SOURCES ${JCC_SOURCES} main.cpp Game.cpp)

if (QT4_FOUND)
	file(GLOB_RECURSE QtRenderer2d_SOURCES ${JCC_SOURCE_DIR}/src/QtRenderer2d/*.cpp)
	set(QtRenderer2d_HEADERS QtRenderer2d/JCCQWidget.h QtRenderer2d/QOpenGLWidget.h)
	include(${QT_USE_FILE})
	add_definitions(${QT_DEFINITIONS})
	
	set(QT_USE_QTOPENGL true)
	QT4_WRAP_CPP(QtRenderer2d_HEADERS_MOC ${QtRenderer2d_HEADERS})
	add_library(QtRenderer2d ${QtRenderer2d_SOURCES} ${QtRenderer2d_HEADERS_MOC})
	
	set(LIBS ${LIBS} QtRenderer2d ${QT_QTCORE_LIBRARY} ${QT_QTOPENGL_LIBRARY})
endif (QT4_FOUND)

add_executable (../JumpingCubesClassic ${JCC_SOURCES})

target_link_libraries (../JumpingCubesClassic ${LIBS})
