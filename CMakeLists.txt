cmake_minimum_required(VERSION 3.10)

project(NephilimOpenGL)
set(APP_EXE NephilimOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB src_files
    "src/*.h"
    "src/*.cpp"
    "src/*.c"
    "src/Engine/*.h"
    "src/Engine/*.cpp"
    "src/Engine/Editor/*.cpp"
    "src/Engine/Editor/*.h"
    "src/Graphics/*.h"
    "src/Graphics/*.cpp"
    "src/Graphics/RenderGraph/*.h"
    "src/Graphics/RenderGraph/*.cpp"
    "src/Engine/Node/*.h"
    "src/Engine/Node/*.cpp"
    "src/ImGUI/**.cpp"
    "src/ImGUI/**.h"
)

add_executable(${APP_EXE} ${src_files})

add_subdirectory( pybind11 )
add_subdirectory( json )

link_directories("C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Python37_64\\libs")

target_link_directories(${APP_EXE} PUBLIC "lib")

target_link_libraries(${APP_EXE} PUBLIC glfw3 )
target_link_libraries(${APP_EXE} PUBLIC Python37)

set(CMAKE_SUPPRESS_REGENERATION true)

include_directories(${APP_EXE} "include/")
target_include_directories(${APP_EXE} PUBLIC "pybind11/include")
target_include_directories(${APP_EXE} PUBLIC "json/single_include")
target_include_directories(${APP_EXE} PUBLIC "src")