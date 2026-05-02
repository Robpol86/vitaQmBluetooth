cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(vitaQmBluetooth LANGUAGES C)
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

# TODO remove?
list(REMOVE_ITEM CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "$ENV{VITASDK}/arm-vita-eabi/include")
list(REMOVE_ITEM CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "$ENV{VITASDK}/arm-vita-eabi/include")

# TODO prune
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)  # Silences "Compatibility with CMake < 3.5 has been removed from CMake"
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -nostdlib -Wl,-e,module_start")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_executable("${PROJECT_NAME}"
  src/main.c
  src/quickmenu.c
)

# TODO prune
target_include_directories("${PROJECT_NAME}" SYSTEM PRIVATE
  "$ENV{VITASDK}/arm-vita-eabi/include"
  "${VENDOR_DIR}/quickmenureborn"
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

target_link_directories("${PROJECT_NAME}" PRIVATE
  "${VENDOR_DIR}/quickmenureborn/vitasdk_stubs"
)

target_compile_options("${PROJECT_NAME}" PRIVATE
  -Wformat=2
  -Werror=format-security
)

vita_create_self("${PROJECT_NAME}.suprx"
  "${PROJECT_NAME}"
  CONFIG exports.yml
  UNSAFE
)

vita_create_stubs(stubs
  "${PROJECT_NAME}"
  "${CMAKE_SOURCE_DIR}/exports.yml"
)
