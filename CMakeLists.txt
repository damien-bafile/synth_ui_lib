cmake_minimum_required(VERSION 3.16)
project(synth_ui_lib LANGUAGES C)

set(CMAKE_C_STANDARD 11)

add_library(${PROJECT_NAME} SHARED src/ui.c)
target_include_directories(${PROJECT_NAME} PUBLIC src)
set_target_properties(${PROJECT_NAME} PROPERTIES
  PREFIX ""
  OUTPUT_NAME synth_ui
  SUFFIX ".dylib"
)

add_library(${PROJECT_NAME}_static STATIC src/ui.c)
target_include_directories(${PROJECT_NAME}_static PUBLIC src)
set_target_properties(${PROJECT_NAME}_static PROPERTIES
  OUTPUT_NAME synth_ui_static
)
