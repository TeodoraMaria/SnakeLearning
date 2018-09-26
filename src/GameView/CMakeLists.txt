cmake_minimum_required(VERSION 3.0.0)
project(GameView)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp"
)

add_library(${PROJECT_NAME} STATIC ${srcs} ${includes})

LoadOpenGl()
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/
)

include_directories("${CMAKE_SOURCE_DIR}/third_party/json/")

target_link_libraries(${PROJECT_NAME}
	GameLogic
	HardCoded
	
	${SDL2_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
)

SetDefaultTargetProperties()
SetCompileFlags_IfParentRequiresIt()