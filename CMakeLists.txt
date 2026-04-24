set(CMAKE_POLICY_VERSION_MINIMUM 3.5)  # Silences "Compatibility with CMake < 3.5 has been removed from CMake"
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(vitaQmBluetooth)
list(REMOVE_ITEM CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "$ENV{VITASDK}/arm-vita-eabi/include")
list(REMOVE_ITEM CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "$ENV{VITASDK}/arm-vita-eabi/include")
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

# Vendored dependencies — each subdirectory under vendor/ has its own README.md
# documenting upstream source, version, license, and update procedure.
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")

add_executable("${PROJECT_NAME}"
  src/main.c
)

target_compile_options("${PROJECT_NAME}" PRIVATE
  -Wformat=2
  -Werror=format-security
)

target_include_directories("${PROJECT_NAME}" PRIVATE
  "$ENV{VITASDK}/arm-vita-eabi/include"
  "${VENDOR_DIR}/quickmenureborn/quickmenureborn"
)

target_link_directories("${PROJECT_NAME}" PRIVATE
  "${VENDOR_DIR}/quickmenureborn/vitasdk_stubs"
)

target_link_libraries("${PROJECT_NAME}"
  k
  gcc
  SceLibc_stub
  taihen_stub
  SceLibKernel_stub
  SceIofilemgr_stub
  SceSysmodule_stub
  SceKernelModulemgr_stub
  QuickMenuReborn_stub_weak
)

vita_create_self("${PROJECT_NAME}.suprx" "${PROJECT_NAME}" CONFIG exports.yml UNSAFE)

vita_create_stubs(stubs "${PROJECT_NAME}" "${CMAKE_SOURCE_DIR}/exports.yml")
