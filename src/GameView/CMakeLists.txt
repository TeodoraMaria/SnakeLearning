cmake_minimum_required(VERSION 3.0.0)
project(GameView)

set(SDL2_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL2")
set(GLEW32_DIR "${CMAKE_SOURCE_DIR}/third_party/glew")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")

find_package(SDL2 REQUIRED)
find_package(GLEW32 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

include_directories(${GLEW32_INCLUDE_DIRS})

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp")

add_executable(${PROJECT_NAME} ${srcs} ${includes})

#add_library(${PROJECT_NAME} STATIC ${srcs} ${includes})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/
)

target_link_libraries(${PROJECT_NAME}
	GameLogic
	${SDL2_LIBRARIES}
	${GLEW32_LIBRARIES}
	opengl32
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
	EXECUTABLE_OUTPUT_PATH		"${CMAKE_SOURCE_DIR}/bin"
)
