cmake_minimum_required(VERSION 3.0.0)

include(OpenGlLoader.cmake)
include(CmakeUtils.cmake)

set(CompileWithFlags FALSE)

if (WIN32)
	set(Qt5_DIR "D:/qt2/5.11.1/msvc2015/lib/cmake/Qt5")
endif()

add_subdirectory(GameLogic)
add_subdirectory(GymEnv)
add_subdirectory(AI)
add_subdirectory(Utils)
add_subdirectory(MultiplatformUtils)
add_subdirectory(GameView)
add_subdirectory(ConfigLoading)
add_subdirectory(AppUI)

add_subdirectory(GameSandBox)
add_subdirectory(OpenGlSandbox)
add_subdirectory(TrainingGrounds)
add_subdirectory(TensorflowSandbox)