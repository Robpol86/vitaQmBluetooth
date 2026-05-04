# Set variables.
set(EXPORTS_YML "${CMAKE_CURRENT_BINARY_DIR}/cmake_gen/exports.yml")
set(PROJECT_ELF "${PROJECT_NAME}.elf")
set(PROJECT_LIB "${PROJECT_NAME}_stub")

# Specify template files to generate.
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/exports.yml.in" "${EXPORTS_YML}" @ONLY)

add_executable("${PROJECT_ELF}"
  src/main.c
)

target_compile_options("${PROJECT_ELF}" PRIVATE
  -Wall
  -Werror=format-security
  -Wformat=2
  -ffile-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}/src/=
)

target_link_options("${PROJECT_ELF}" PRIVATE
  -Wl,-e,module_start
  -Wl,-q
  -nostdlib
)

target_include_directories("${PROJECT_ELF}" PRIVATE
  "${CMAKE_BINARY_DIR}/cmake_gen"
)

target_link_libraries("${PROJECT_ELF}" PRIVATE
  gcc

  taihenForKernel_stub
  taihenModuleUtils_stub
  SceSysmemForDriver_stub
  SceSysclibForDriver_stub
  SceSysrootForKernel_stub
  SceKernelUtilsForDriver_stub

  SceNetPsForDriver_stub
  SceSblSsMgrForDriver_stub
  SceThreadmgrForDriver_stub
  SceSblACMgrForDriver_stub

  SceQafMgrForDriver_stub

  SceModulemgrForDriver_stub
  SceIofilemgrForDriver_stub
  SceDebugForDriver_stub

  SceRtc_stub
)

vita_create_self("${PROJECT_NAME}.skprx"
  "${PROJECT_ELF}"
  CONFIG "${EXPORTS_YML}"
  UNSAFE
)

vita_create_stubs(stubs
  "${PROJECT_ELF}"
  "${EXPORTS_YML}"
  KERNEL
)

add_library("${PROJECT_LIB}" STATIC IMPORTED GLOBAL)

add_dependencies("${PROJECT_LIB}" stubs)

set_target_properties("${PROJECT_LIB}" PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/stubs/lib${PROJECT_LIB}.a"
)
