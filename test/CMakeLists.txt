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
)
target_link_libraries(test_synth_ui PRIVATE synth_ui_lib_static GTest::gtest_main)
target_include_directories(test_synth_ui PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME synth_ui COMMAND test_synth_ui)
