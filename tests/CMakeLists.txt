cmake_minimum_required(VERSION 3.20)  # TODO dedupe with top file?

project(vitaQmBluetoothTests LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Download cmocka.
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(cmocka
  URL                         https://cmocka.org/files/2.0/cmocka-2.0.2.tar.xz  # TODO cache in GHA
  URL_HASH                    SHA256=39f92f366bdf3f1a02af4da75b4a5c52df6c9f7e736c7d65de13283f9f0ef416
  DOWNLOAD_EXTRACT_TIMESTAMP  TRUE
)
# set(WITH_STATIC_LIB ON CACHE BOOL "CMocka: Build with a static library" FORCE)  # TODO needed?
# set(WITH_CMOCKERY_SUPPORT OFF CACHE BOOL "CMocka: Install a cmockery header" FORCE)
# set(WITH_EXAMPLES OFF CACHE BOOL "CMocka: Build examples" FORCE)
# set(UNIT_TESTING OFF CACHE BOOL "CMocka: Build with unit testing" FORCE)
# set(PICKY_DEVELOPER OFF CACHE BOOL "CMocka: Build with picky developer flags" FORCE)
# set(CMOCKA_TESTS OFF CACHE BOOL "Disable CMocka self-tests")
FetchContent_MakeAvailable(cmocka)
# add_library(cmocka::cmocka ALIAS cmocka)

enable_testing()

file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
foreach(src ${TEST_SRCS})
  file(RELATIVE_PATH rel "${CMAKE_CURRENT_SOURCE_DIR}" "${src}")
  string(REGEX REPLACE "\\.c$" "" name "${rel}")
  string(REGEX REPLACE "/" "_" name "${name}")
  add_executable("${name}" "${src}")
  target_link_libraries("${name}" cmocka::cmocka)
  add_test(NAME "${name}" COMMAND "${name}")  # TODO show N>1 tests per file.
endforeach()
