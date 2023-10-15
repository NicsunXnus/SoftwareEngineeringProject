cmake_minimum_required(VERSION 3.15)

project(
  "AutoTester"
  LANGUAGES CXX
)

file(GLOB SRC_DIR "../source/**/*.cpp")
file(GLOB SRC_DIR_2 "../source/**/**/*.cpp")
file(GLOB SRC_DIR_3 "../source/*.cpp")
file(GLOB HDR_DIR "../source/**/*.h")
file(GLOB HDR_DIR_2 "../source/*.h")
file(GLOB HDR_DIR_3 "../source/**/**/*.h")

set(sources
    ${SRC_DIR}
    ${SRC_DIR_2}
    ${SRC_DIR_3}
)
set(headers
    ${HDR_DIR}
    ${HDR_DIR_2}
    ${HDR_DIR_3}
)
set(exe_sources
	${sources}
	${headers}
)
#
# Set the project standard and warnings
#
target_compile_features(AutoTester PRIVATE cxx_std_17)
include(../../cmake/CompilerWarnings.cmake)
set_project_warnings(AutoTester)
add_executable(autotester WIN32)
target_sources(autotester PRIVATE ${exe_sources} source/AbstractWrapper.h source/TestWrapper.h source/TestWrapper.cpp)
install(TARGETS autotester)