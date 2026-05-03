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
  -Werror=format-security
  -Wformat=2
  -ffile-prefix-map=${CMAKE_SOURCE_DIR}/src/=
)

target_link_options("${PROJECT_NAME}" PRIVATE
  -Wl,-e,module_start
  -Wl,-q
  -nostdlib
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
  SceRtc_stub
  QuickMenuReborn_stub_weak
)

vita_create_self("${PROJECT_NAME}.suprx"
  "${PROJECT_NAME}"
  CONFIG "${EXPORTS_YML}"
  UNSAFE
)
