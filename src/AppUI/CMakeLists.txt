cmake_minimum_required(VERSION 3.0.0)
project(AppUI)

set(CMAKE_CXX_STANDARD 14)

file(GLOB srcs "*.cpp" "*.h" "*.hpp")
file(GLOB includes
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.h"
	"${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/*.hpp"
)
######


# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Widgets Gui Charts REQUIRED)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui")

include_directories("${CMAKE_SOURCE_DIR}/bin")
######

include_directories("${CMAKE_SOURCE_DIR}/third_party/json/")
add_library(${PROJECT_NAME} STATIC ${srcs} ${includes})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes/${PROJECT_NAME}/
	${CMAKE_SOURCE_DIR}/includes/
)

if (NOT WIN32)
	
endif()

set(project_Libs
	GameLogic
	HardCoded
	Utils
	GameLogic
	ConfigLoading
	
	Supervised
	GeneticAlgorithm

	Qt5::Widgets 
	Qt5::Core 
	Qt5::Gui
	Qt5::Charts
)

if (NOT WIN32)
	find_package(Tensorflow)
	include_directories(${Tensorflow_INCLUDE_DIRS})	

	list(APPEND project_Libs TensorflowSandbox)
endif()

target_link_libraries(${PROJECT_NAME} ${project_Libs})


SetDefaultTargetProperties()
SetCompileFlags_IfParentRequiresIt()