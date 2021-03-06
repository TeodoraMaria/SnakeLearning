cmake_minimum_required(VERSION 3.0.0)
project(GeneticAlgorithm)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/AI/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/AI/${PROJECT_NAME}/*.hpp")

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Widgets Gui Charts REQUIRED)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui")

include_directories("${CMAKE_SOURCE_DIR}/bin")



add_library(${PROJECT_NAME} STATIC ${srcs} ${includes})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/AI/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/
	${CMAKE_SOURCE_DIR}/third_party/json/
)

target_link_libraries(${PROJECT_NAME}
	Utils
	GameLogic

	Qt5::Widgets 
	Qt5::Core 
	Qt5::Gui
	Qt5::Charts
)

SetDefaultTargetProperties()
SetCompileFlags_IfParentRequiresIt()