cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(vitaQmBluetooth VERSION 0.1.0 LANGUAGES C)
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

# Explicit includes for clang-tidy.
list(REMOVE_ITEM CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "$ENV{VITASDK}/arm-vita-eabi/include")

# Set cmake options.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set variables.
set(EXPORTS_YML "${CMAKE_CURRENT_BINARY_DIR}/cmake_gen/exports.yml")
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")

# Specify template files to generate.
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_gen/config.h" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/exports.yml.in" "${EXPORTS_YML}" @ONLY)

add_executable("${PROJECT_NAME}"
  src/main.c
  src/quickmenu.c
)

target_compile_options("${PROJECT_NAME}" PRIVATE
  -Wall
  -Wformat=2
  -Werror=format-security
)

target_link_options("${PROJECT_NAME}" PRIVATE
  -Wl,-q
  -nostdlib
  -Wl,-e,module_start
)

target_include_directories("${PROJECT_NAME}" SYSTEM PRIVATE
  "${VENDOR_DIR}/quickmenureborn"
)
target_include_directories("${PROJECT_NAME}" PRIVATE
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_gen"
)

target_link_directories("${PROJECT_NAME}" PRIVATE
  "${VENDOR_DIR}/quickmenureborn/vitasdk_stubs"
)

target_link_libraries("${PROJECT_NAME}" PRIVATE
  SceLibc_stub
  taihen_stub
  SceLibKernel_stub
  QuickMenuReborn_stub_weak
)

vita_create_self("${PROJECT_NAME}.suprx"
  "${PROJECT_NAME}"
  CONFIG "${EXPORTS_YML}"
  UNSAFE
)
