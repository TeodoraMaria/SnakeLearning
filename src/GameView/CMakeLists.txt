cmake_minimum_required(VERSION 3.0.0)
project(GameView)

set(CMAKE_CXX_STANDARD 14)

IF (WIN32)
	message("It's WINDOWS")

	set(SDL2_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL2")
	set(GLEW32_DIR "${CMAKE_SOURCE_DIR}/third_party/glew")
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")

	set(GLEW_LIB "GLEW32")
ELSE()
	message("It's not WINDOWS")

	set(GLEW_LIB "GLEW")

	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIR})
ENDIF()

find_package(SDL2 REQUIRED)
find_package(${GLEW_LIB} REQUIRED)

IF (WIN32)
	set(GLEW_INCLUDE_DIRS ${GLEW32_INCLUDE_DIRS})
	set(GLEW_LIBRARIES ${GLEW32_LIBRARIES})
	set(OPENGL_LIBRARIES opengl32)
ENDIF()

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp"
)

add_executable(${PROJECT_NAME} ${srcs} ${includes})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/
)

target_link_libraries(${PROJECT_NAME}
	GameLogic
	HardCoded
	${SDL2_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	EXECUTABLE_OUTPUT_PATH		"${CMAKE_SOURCE_DIR}/bin"
)
