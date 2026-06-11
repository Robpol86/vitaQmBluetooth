cmake_minimum_required(VERSION 3.20)  # TODO dedupe with top file?

project(vitaQmBluetoothTests LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# TODO
if(APPLE)
  execute_process(
    COMMAND xcrun --show-sdk-path
    OUTPUT_VARIABLE MACOS_SDK_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE XCRUN_STATUS
  )
  if(XCRUN_STATUS EQUAL 0 AND IS_DIRECTORY "${MACOS_SDK_PATH}")
    add_compile_options(-isysroot "${MACOS_SDK_PATH}")
  else()
    message(WARNING "xcrun --show-sdk-path failed; clang-tidy may not find system headers")
  endif()
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

file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/module_*/*.c"
)

set(TEST_SHIMS
  "${CMAKE_CURRENT_SOURCE_DIR}/shims/shims.c"
)
foreach(src ${TEST_SRCS})
  file(RELATIVE_PATH rel "${CMAKE_CURRENT_SOURCE_DIR}" "${src}")
  get_filename_component(reldir "${rel}" DIRECTORY)   # -> "module_user" or "module_kernel"
  string(REGEX REPLACE "\\.c$" "" name "${rel}")
  string(REGEX REPLACE "/" "_" name "${name}")

  if(reldir STREQUAL "module_user")
    set(MODULE_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/../module_user/src/qm_state.c"
    )
  # elseif(reldir STREQUAL "module_kernel")
    # set(MODULE_SOURCES
    #   "${CMAKE_CURRENT_SOURCE_DIR}/../module_kernel/src/bluetooth.c"
    # )
  else()
    set(MODULE_SOURCES "")
  endif()


  add_executable("${name}"
    "${src}"
    ${MODULE_SOURCES}
    ${TEST_SHIMS}
  )
  target_link_libraries("${name}" PRIVATE cmocka::cmocka)
  target_compile_definitions("${name}" PRIVATE
    MODULE_NAME="${name}"
    PROJECT_NAME="${CMAKE_PROJECT_NAME}"
    ENABLE_VITASDK_BUILD_ASSERT)
  target_include_directories("${name}" PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/shims"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../${reldir}/src")
  add_test(NAME "${name}" COMMAND "${name}")  # TODO show N>1 tests per file.
endforeach()
