cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(pacemaker VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_BUILD_TYPE Release)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME}
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall -O2>
  $<$<CXX_COMPILER_ID:MSVC>: /W2 /O2>
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)