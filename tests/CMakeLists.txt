cmake_minimum_required(VERSION 3.20)

project(vitaQmBluetoothTests LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# TODO keep below?
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
#TODO above

enable_testing()

file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
foreach(src ${TEST_SRCS})
  file(RELATIVE_PATH rel "${CMAKE_CURRENT_SOURCE_DIR}" "${src}")
  string(REGEX REPLACE "\\.c$" "" name "${rel}")
  string(REGEX REPLACE "/" "_" name "${name}")
  add_executable("${name}" "${src}")
  add_test(NAME "${name}" COMMAND "${name}")  # TODO show N>1 tests per file.
endforeach()
