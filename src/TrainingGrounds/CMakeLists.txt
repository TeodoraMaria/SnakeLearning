cmake_minimum_required(VERSION 3.0.0)
project(TrainingGrounds)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp")
add_executable(${PROJECT_NAME} ${srcs} ${includes})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/)

LoadOpenGl()
include_directories(${SDL2_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}/third_party/json/")

find_package(Tensorflow)
include_directories(${Tensorflow_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC
	HardCoded
	GymEnv
	QLearning
	GeneticAlgorithm
	GameLogic
	ConfigLoading
	TensorflowSandbox
	Utils

	${SDL2_LIBRARIES}
)

SetDefaultTargetProperties()
SetCompileFlags_IfParentRequiresIt()
