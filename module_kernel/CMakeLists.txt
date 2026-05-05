# Set variables.
set(EXPORTS_YML "${CMAKE_CURRENT_BINARY_DIR}/cmake_gen/exports.yml")
set(MODULE_NAME "${PROJECT_NAME}Kernel")

# Specify template files to generate.
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/exports.yml.in" "${EXPORTS_YML}" @ONLY)

add_executable("${MODULE_NAME}"
  src/main.c
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

target_include_directories("${MODULE_NAME}" PRIVATE
  "${CMAKE_BINARY_DIR}/cmake_gen"
)

target_link_libraries("${MODULE_NAME}" PRIVATE
  gcc

  SceDebugForDriver_stub
)

vita_create_self("${PROJECT_NAME}.skprx"
  "${MODULE_NAME}"
  CONFIG "${EXPORTS_YML}"
  UNSAFE
)

vita_create_stubs(stubs
  "${MODULE_NAME}"
  "${EXPORTS_YML}"
  KERNEL
)

# TODO implement communication between kernel and user modules then prune/reorder these.
add_library("${MODULE_NAME}_stub" STATIC IMPORTED GLOBAL)
add_dependencies("${MODULE_NAME}_stub" stubs)
set_target_properties("${MODULE_NAME}_stub" PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/stubs/lib${PROJECT_LIB}.a"
)
