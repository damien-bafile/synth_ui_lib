set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_synth_ui
  main.cpp
  test_colors.cpp
  test_rect.cpp
  test_easing.cpp
  test_tween.cpp
  test_timeline.cpp
  test_layout.cpp
  test_touch_dispatcher.cpp
  test_touch_calibration.cpp
  test_widget.cpp
  test_page_layout.cpp
  test_framebuffer.cpp
  test_button.cpp
  test_toggle.cpp
  test_checkbox.cpp
  test_radio.cpp
  test_slider.cpp
  test_vu_slider.cpp
  test_label.cpp
  test_page_indicator.cpp
  test_textfield.cpp
  test_hbar.cpp
  test_panel.cpp
  test_infobar.cpp
  test_tabbar.cpp
  test_voice_meter.cpp
  test_stepgrid.cpp
  test_xy_pad.cpp
  test_graph.cpp
  test_radial_dial.cpp
  test_dropdown.cpp
  test_popup_menu.cpp
  test_transport_bar.cpp
  test_keyboard.cpp
  test_meter_array.cpp
  test_waveform.cpp
  test_waveform_view.cpp
  test_piano_keyboard.cpp
  test_spectrum_view.cpp
  test_mod_matrix.cpp
  test_tooltip.cpp
)
target_link_libraries(test_synth_ui PRIVATE synth_ui_lib_static GTest::gtest_main)
target_include_directories(test_synth_ui PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME synth_ui COMMAND test_synth_ui)
