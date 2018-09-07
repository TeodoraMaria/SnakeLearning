include(CheckCXXCompilerFlag)

macro(SetDefaultTargetProperties)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
		LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_SOURCE_DIR}/bin"
		EXECUTABLE_OUTPUT_PATH		"${CMAKE_SOURCE_DIR}/bin"
	)
endmacro()

macro(SetFlagIfSupported flag flagStorage)
	CHECK_CXX_COMPILER_FLAG(${flag} isSupported)
	
	if (${isSupported})
		String(CONCAT ${flagStorage} ${${flagStorage}} " " "${flag}")
	endif()
endmacro()

macro(SetCompileFlags)
	if (NOT WIN32)
		set(Flags "")

		SetFlagIfSupported("-Wall" Flags)
		SetFlagIfSupported("-Wextra" Flags)
		SetFlagIfSupported("-Werror" Flags)
		SetFlagIfSupported("-Waddress" Flags)
		
		set(CMAKE_CXX_FLAGS "${Flags}")
	endif()
endmacro()

macro(SetCompileFlags_IfParentRequiresIt)
	if (${CompileWithFlags})
		SetCompileFlags()
	endif()
endmacro()
