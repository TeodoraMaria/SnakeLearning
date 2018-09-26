cmake_minimum_required(VERSION 3.0.0)
project(TensorflowSandbox)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp")
add_library(${PROJECT_NAME} STATIC ${srcs} ${includes})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/)

LoadOpenGl()
include_directories(${SDL2_INCLUDE_DIRS})

find_package(Tensorflow)
include_directories(${Tensorflow_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}/third_party/json/")

target_link_libraries(${PROJECT_NAME} PUBLIC
	GameLogic
	Utils
	QLearning
	ConfigLoading

	${Tensorflow_LIBRARIES}
)

SetDefaultTargetProperties()
SetCompileFlags_IfParentRequiresIt()

add_compile_definitions(TENSORFLOW)