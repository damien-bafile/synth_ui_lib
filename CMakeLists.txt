cmake_minimum_required(VERSION 3.16)
project(synth_ui_lib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Sanitizers ─────────────────────────────────────────────────────────
option(ENABLE_SANITIZERS "Build with AddressSanitizer + UndefinedBehaviorSanitizer" OFF)
if(ENABLE_SANITIZERS)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

# ── Tests ──────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
  enable_testing()
  include(FetchContent)
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
  )
  FetchContent_MakeAvailable(googletest)
  add_subdirectory(test)
endif()

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
    src/ui/colors.cpp
    src/ui/framebuffer.cpp
    src/ui/touch.cpp
    src/ui/widget.cpp
    src/ui/touch_dispatcher.cpp
    src/ui/button.cpp
    src/ui/slider.cpp
    src/ui/vu_slider.cpp
    src/ui/toggle.cpp
    src/ui/checkbox.cpp
    src/ui/radio.cpp
    src/ui/panel.cpp
    src/ui/hbar.cpp
    src/ui/textfield.cpp
    src/ui/keyboard.cpp
    src/ui/piano_keyboard.cpp
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
    src/ui/popup_slider.cpp
    src/ui/infobar.cpp
    src/ui/transport_bar.cpp
    src/ui/label.cpp
    src/ui/page_indicator.cpp
    src/ui/tabbar.cpp
    src/ui/voice_meter.cpp
    src/ui/waveform_view.cpp
    src/ui/spectrum_view.cpp
    src/ui/mod_matrix.cpp
    src/ui/tooltip.cpp
    src/ui/list_view.cpp
    src/ui/spinbox.cpp
    src/ui/midi_learn_badge.cpp
    src/ui/anim.cpp
)

add_library(${PROJECT_NAME} SHARED ${UI_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
)

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
target_include_directories(${PROJECT_NAME}_static PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(${PROJECT_NAME}_static PROPERTIES
  OUTPUT_NAME synth_ui_static
)

# ── Install / package config ──────────────────────────────────────────
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_static
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY src/ui/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ui
  FILES_MATCHING PATTERN "*.h"
)
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE synth_ui::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
  cmake/${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# ── Examples ───────────────────────────────────────────────────────────
option(BUILD_EXAMPLES "Build example programs" OFF)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
