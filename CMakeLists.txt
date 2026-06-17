cmake_minimum_required(VERSION 3.16)
project(synth_ui_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(UI_SOURCES
    src/ui/framebuffer.cpp
    src/ui/touch.cpp
    src/ui/button.cpp
    src/ui/slider.cpp
    src/ui/toggle.cpp
    src/ui/checkbox.cpp
    src/ui/radio.cpp
    src/ui/panel.cpp
    src/ui/hbar.cpp
    src/ui/textfield.cpp
    src/ui/graph.cpp
    src/ui/dropdown.cpp
    src/ui/stepgrid.cpp
)

add_library(${PROJECT_NAME} SHARED ${UI_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC src)
set_target_properties(${PROJECT_NAME} PROPERTIES
  PREFIX ""
  OUTPUT_NAME synth_ui
  SUFFIX ".dylib"
)

add_library(${PROJECT_NAME}_static STATIC ${UI_SOURCES})
target_include_directories(${PROJECT_NAME}_static PUBLIC src)
set_target_properties(${PROJECT_NAME}_static PROPERTIES
  OUTPUT_NAME synth_ui_static
)
