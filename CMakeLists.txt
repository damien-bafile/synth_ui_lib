cmake_minimum_required(VERSION 3.16)
project(synth_ui_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

# Export compile commands for IDE support and tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create a repo-root compile_commands.json for LSP integration.
# Windows gets a copy; Unix-like systems get a symlink.
if(WIN32)
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json
  )
else()
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json
  )
endif()

set(UI_SOURCES
    src/ui/framebuffer.cpp
    src/ui/touch.cpp
    src/ui/widget.cpp
    src/ui/touch_dispatcher.cpp
    src/ui/button.cpp
    src/ui/slider.cpp
    src/ui/toggle.cpp
    src/ui/checkbox.cpp
    src/ui/radio.cpp
    src/ui/panel.cpp
    src/ui/hbar.cpp
    src/ui/textfield.cpp
    src/ui/keyboard.cpp
    src/ui/graph.cpp
    src/ui/dropdown.cpp
    src/ui/stepgrid.cpp
    src/ui/waveform.cpp
    src/ui/meter_array.cpp
    src/ui/xy_pad.cpp
    src/ui/radial_dial.cpp
    src/ui/layout.cpp
    src/ui/page_layout.cpp
    src/ui/popup_menu.cpp
)

add_library(${PROJECT_NAME} SHARED ${UI_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC src)

# Cross-platform library settings
if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME synth_ui
    SUFFIX ".dylib"
  )
elseif(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME synth_ui
    SUFFIX ".dll"
  )
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX "lib"
    OUTPUT_NAME synth_ui
    SUFFIX ".so"
  )
endif()

add_library(${PROJECT_NAME}_static STATIC ${UI_SOURCES})
target_include_directories(${PROJECT_NAME}_static PUBLIC src)
set_target_properties(${PROJECT_NAME}_static PROPERTIES
  OUTPUT_NAME synth_ui_static
)
