cmake_minimum_required(VERSION 3.0.0)
project(GymEnv)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp")

file(GLOB stateObserverCPP
	"StateObserver/*.cpp"
)

file(GLOB stateObserverHPP
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/StateObserver/*.hpp"
)

source_group("StateObserver\\src" FILES ${stateObserverCPP})
source_group("StateObserver\\includes" FILES ${stateObserverHPP})

add_library(${PROJECT_NAME} STATIC
	${srcs} ${includes}
	${stateObserverCPP} ${stateObserverHPP})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/)

include_directories("${CMAKE_SOURCE_DIR}/third_party/json/")

target_link_libraries(${PROJECT_NAME}
	GameView
	GameLogic
)

SetDefaultTargetProperties()
SetCompileFlags_IfParentRequiresIt()