set(GLEW32_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(GLEW32_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/Release/x64/glew32.lib;${CMAKE_CURRENT_LIST_DIR}/lib/Release/x64/glew32s.lib")
else ()
  set(GLEW32_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/Release/Win32/glew32.lib;${CMAKE_CURRENT_LIST_DIR}/lib/Release/Win32/glew32s.lib")
endif ()

string(STRIP "${GLEW32_LIBRARIES}" GLEW32_LIBRARIES)