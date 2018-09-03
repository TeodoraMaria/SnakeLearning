#[[
	Macro to load OpenGL on multiple platforms.
	This macro sets the following variables:
		SDL2_INCLUDE_DIRS
		GLEW_INCLUDE_DIRS
		OPENGL_INCLUDE_DIR

		SDL2_LIBRARIES
		GLEW_LIBRARIES
		OPENGL_LIBRARIES
	OPENGL_LIBRARIES and OPENGL_INCLUDE_DIR may be NULL on Windows.
]]

macro(LoadOpenGl)
	if (WIN32)
		set(SDL2_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL2")
		set(GLEW32_DIR "${CMAKE_SOURCE_DIR}/third_party/glew")
		set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")
		set(GLEW_LIB "GLEW32")
	else()
		set(GLEW_LIB "GLEW")
		find_package(OpenGL REQUIRED)
	endif()

	find_package(SDL2 REQUIRED)
	find_package(${GLEW_LIB} REQUIRED)

	if (WIN32)
		set(GLEW_INCLUDE_DIRS ${GLEW32_INCLUDE_DIRS})
		set(GLEW_LIBRARIES ${GLEW32_LIBRARIES})
		set(OPENGL_LIBRARIES opengl32)
	endif()
endmacro()