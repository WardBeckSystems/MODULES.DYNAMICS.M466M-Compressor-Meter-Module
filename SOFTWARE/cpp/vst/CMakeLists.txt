cmake_minimum_required(VERSION 3.20)
project(WBS_M466_Compressor_Limiter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(WBS_M466_Compressor_Limiter STATIC
    PluginProcessor.cpp
    PluginEditor.cpp
)

target_link_libraries(WBS_M466_Compressor_Limiter PUBLIC wbs_core)
target_include_directories(WBS_M466_Compressor_Limiter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
