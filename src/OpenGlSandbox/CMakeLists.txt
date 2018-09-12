cmake_minimum_required(VERSION 3.0.0)
project(OpenGlSandBox)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp")
add_executable(${PROJECT_NAME} ${srcs} ${includes})

LoadOpenGl()
include_directories(${SDL2_INCLUDE_DIRS})

add_definitions(${NANOGUI_EXTRA_DEFS})
include_directories(${NANOGUI_EXTRA_INCS})
include_directories("${CMAKE_SOURCE_DIR}/third_party/nanogui/include/")
include_directories(${MYPROJ_EIGEN3_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/)
	
target_link_libraries(${PROJECT_NAME}
	GameView

	nanogui ${NANOGUI_EXTRA_LIBS}
)

SetDefaultTargetProperties()
SetCompileFlags_IfParentRequiresIt()
