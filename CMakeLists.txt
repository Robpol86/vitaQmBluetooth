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

# Specify template files to generate.
configure_file("${CMAKE_SOURCE_DIR}/include/config.h.in" "${CMAKE_BINARY_DIR}/cmake_gen/config.h" @ONLY)

add_subdirectory(module_kernel)
add_subdirectory(module_user)
