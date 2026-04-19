set(CMAKE_POLICY_VERSION_MINIMUM 3.5)  # Silences "Compatibility with CMake < 3.5 has been removed from CMake"
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(deplayenabler)
list(REMOVE_ITEM CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "$ENV{VITASDK}/arm-vita-eabi/include")
list(REMOVE_ITEM CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "$ENV{VITASDK}/arm-vita-eabi/include")
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_executable(deplayenabler
  src/main.c
)

target_compile_options(deplayenabler PRIVATE
  -Wformat=2
  -Werror=format-security
)

target_include_directories(deplayenabler PRIVATE
  "$ENV{VITASDK}/arm-vita-eabi/include"
)

target_link_libraries(deplayenabler
  k
  gcc
  SceLibc_stub
  taihen_stub
  SceLibKernel_stub
  SceIofilemgr_stub
  SceSysmodule_stub
  SceKernelModulemgr_stub
)

vita_create_self(deplayenabler.suprx deplayenabler CONFIG exports.yml UNSAFE)

vita_create_stubs(stubs deplayenabler "${CMAKE_SOURCE_DIR}/exports.yml")
