cmake_minimum_required(VERSION 3.0.0)

include(OpenGlLoader.cmake)
include(CmakeUtils.cmake)

set(CompileWithFlags FALSE)

add_subdirectory(GameLogic)
add_subdirectory(GymEnv)
add_subdirectory(AI)
add_subdirectory(Utils)
add_subdirectory(MultiplatformUtils)
add_subdirectory(GameView)
add_subdirectory(ConfigLoading)

add_subdirectory(GameSandBox)
add_subdirectory(OpenGlSandbox)
add_subdirectory(TrainingGrounds)
add_subdirectory(GUIPresentation)