cmake_minimum_required(VERSION 3.0.0)

include(NanoGUIUtils.cmake)

SetNanoGUIOptions()

add_subdirectory(third_party/nanogui)
add_subdirectory(src)

SetNanoGUIDependecies()