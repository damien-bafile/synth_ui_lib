# anim_demo needs only what's already in synth_ui_lib_static.
add_executable(anim_demo anim_demo.cpp)
target_link_libraries(anim_demo PRIVATE synth_ui_lib_static)

# audio_reactive_demo additionally needs synth_dsp_lib, checked out as a
# sibling of this repo. Compile its handful of required .cpp files directly
# (mirrors the ad hoc command documented in audio_reactive_demo.cpp) rather
# than add_subdirectory()-ing the whole sibling project, which would pull in
# its own ~50-target test suite and a second FetchContent(googletest).
set(SYNTH_DSP_LIB_DIR ${CMAKE_SOURCE_DIR}/../synth_dsp_lib)
if(EXISTS ${SYNTH_DSP_LIB_DIR}/src/dsp/dsp.h)
  add_executable(audio_reactive_demo
    audio_reactive_demo.cpp
    ${SYNTH_DSP_LIB_DIR}/src/dsp/fft.cpp
    ${SYNTH_DSP_LIB_DIR}/src/dsp/window.cpp
    ${SYNTH_DSP_LIB_DIR}/src/dsp/bands.cpp
    ${SYNTH_DSP_LIB_DIR}/src/dsp/onset.cpp
  )
  target_include_directories(audio_reactive_demo PRIVATE ${SYNTH_DSP_LIB_DIR}/src)
  target_link_libraries(audio_reactive_demo PRIVATE synth_ui_lib_static)
  # synth_dsp_lib's headers use C++17 (std::clamp, inline statics); this repo
  # targets C++14, so override the standard for this one target rather than
  # raising it project-wide.
  set_target_properties(audio_reactive_demo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
  )
else()
  message(STATUS "synth_dsp_lib not found alongside synth_ui_lib -- skipping audio_reactive_demo")
endif()
