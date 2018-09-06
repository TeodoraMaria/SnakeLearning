macro(SetDefaultTargetProperties)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
		LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
		EXECUTABLE_OUTPUT_PATH		"${CMAKE_SOURCE_DIR}/bin"
	)
endmacro()

macro(SetCompileFlags)
	String(CONCAT Flags ${Flags} " " "-Wall")
	String(CONCAT Flags ${Flags} " " "-Wextra")
	String(CONCAT Flags ${Flags} " " "-Werror")
	String(CONCAT Flags ${Flags} " " "-Waddress")
	Set(CMAKE_CXX_FLAGS "${Flags}")
endmacro()

macro(SetCompileFlags_IfParentRequiresIt)
	if (${CompileWithFlags})
		SetCompileFlags()
	endif()
endmacro()
