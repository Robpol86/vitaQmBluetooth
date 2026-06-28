# Shared CMake helpers for the project.

# Set MODULE_NAME and PROJECT_NAME as compile definitions so code can read these variables from CMake.
function(forward_module_name_project_name target)
  target_compile_definitions("${target}" PRIVATE ENABLE_VITASDK_BUILD_ASSERT
    MODULE_NAME="${MODULE_NAME}"
    PROJECT_NAME="${PROJECT_NAME}"
  )
endfunction()
