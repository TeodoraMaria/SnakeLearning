cmake_minimum_required(VERSION 3.0.0)
project(GameLogic)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")

file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp"
)

file(GLOB cellInterpreters
	"CellInterpreter/*.cpp"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/CellInterpreter/*.hpp"
)

source_group("CellInterpreter" FILES ${cellInterpreters})

add_library(${PROJECT_NAME} STATIC ${srcs} ${includes} ${cellInterpreters})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/)

target_link_libraries(${PROJECT_NAME}
	MultiplatformUtils
	GymEnv
)

SetDefaultTargetProperties()
SetCompileFlags_IfParentRequiresIt()
