cmake_minimum_required(VERSION 3.0.0)
project(GymEnv)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp")

file(GLOB stateObserver
	"StateObserver/*.cpp"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/StateObserver/*.hpp"
)

source_group("StateObserver" FILES ${stateObserver})

add_library(${PROJECT_NAME} STATIC ${srcs} ${includes} ${stateObserver})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/)

target_link_libraries(${PROJECT_NAME}
	GameView
	GameLogic
)

SetDefaultTargetProperties()
SetCompileFlags_IfParentRequiresIt()