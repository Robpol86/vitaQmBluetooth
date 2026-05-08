# Set variables.
set(EXPORTS_YML "${CMAKE_CURRENT_BINARY_DIR}/cmake_gen/exports.yml")
set(MODULE_NAME "${PROJECT_NAME}User")
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")

# Specify template files to generate.
# TODO switch to target_compile_definitions().
configure_file("${CMAKE_SOURCE_DIR}/include/config.h.in" "${CMAKE_BINARY_DIR}/cmake_gen/config.h" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/exports.yml.in" "${EXPORTS_YML}" @ONLY)

add_executable("${MODULE_NAME}"
  src/main.c
  src/quickmenu.c
)

target_compile_definitions("${MODULE_NAME}" PRIVATE
  MODULE_NAME="${MODULE_NAME}"
)

target_compile_options("${MODULE_NAME}" PRIVATE
  -Wall
  -Werror=format-security
  -Wformat=2
  -ffile-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}/src/=
)

target_link_options("${MODULE_NAME}" PRIVATE
  -Wl,-e,module_start
  -Wl,-q
  -nostdlib
)

target_include_directories("${MODULE_NAME}" SYSTEM PRIVATE
  "${VENDOR_DIR}/quickmenureborn"
)
target_include_directories("${MODULE_NAME}" PRIVATE
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_BINARY_DIR}/cmake_gen"
)

target_link_directories("${MODULE_NAME}" PRIVATE
  "${VENDOR_DIR}/quickmenureborn/vitasdk_stubs"
)

target_link_libraries("${MODULE_NAME}" PRIVATE
  gcc

  SceLibKernel_stub
  SceLibc_stub
  SceRtc_stub

  taihen_stub

  QuickMenuReborn_stub_weak

  "${PROJECT_NAME}_stub"
)

vita_create_self("${PROJECT_NAME}.suprx"
  "${MODULE_NAME}"
  CONFIG "${EXPORTS_YML}"
  UNSAFE
)
