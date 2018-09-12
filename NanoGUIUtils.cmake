macro(SetNanoGUIOptions)
	# Disable building extras we won't need (pure C++ project)
	set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
	set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
	set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

	# `if NOT` is what enables the same bypass for your project
	if(NOT MYPROJ_EIGEN3_INCLUDE_DIR)
	  # Grab or find the Eigen3 include directory.
	  find_package(Eigen3 QUIET)
	  if(EIGEN3_INCLUDE_DIR)
	    set(MYPROJ_EIGEN3_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
	  else()
	    # use the internal NanoGUI copy of Eigen
	    set(MYPROJ_EIGEN3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/ext/eigen)
	  endif()
	endif()

	message(STATUS "Using Eigen3 from directory: ${MYPROJ_EIGEN3_INCLUDE_DIR}")
	set(NANOGUI_EIGEN_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR} CACHE BOOL " " FORCE)
endmacro()

macro(SetNanoGUIDependecies)
	# For reliability of parallel build, make the NanoGUI targets dependencies
	set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")
endmacro()