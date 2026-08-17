# Changelog

All notable changes to this project are documented in this file.

## [Unreleased]

- CMake: `project()` now tracks `VERSION 1.0.0`, synced with `library.json`.
- CMake: install rules and a `synth_ui_libConfig.cmake` package config, so
  downstream projects can `find_package(synth_ui_lib)`.
- CMake: `ENABLE_SANITIZERS` option (ASan+UBSan); `just test-asan`.
- Added `.clang-format` and `.clang-tidy` so `just format`/`just lint` have
  real configs to run against.
- Added `Doxyfile` and a `just docs` recipe for generated API docs.
- Wired `examples/` into the build behind `BUILD_EXAMPLES` (OFF by
  default); `audio_reactive_demo` only builds when `synth_dsp_lib` is
  checked out as a sibling directory.
- CI (GitHub Actions) builds and runs the test suite on push/PR to `main`.
- `VUSlider` (`vu_slider.h`/`.cpp`) is now actually compiled into the
  library — it previously existed on disk but wasn't wired into
  `CMakeLists.txt` or `ui.h`.
- Unit test coverage extended to the widget layer (previously only
  base/infra classes had tests): `Button`, `Toggle`, `Checkbox`, `Radio`,
  `Slider`, `VUSlider`, `Label`, `PageIndicator`.

## [1.0.0]

Baseline: ~28 widgets (controls, display/data-viz, composite/overlay),
a 4-theme color system, touch/gesture dispatch, and an animation system
(easing, tweening, timelines). See `ui-widget-summary.org` for the full
widget catalog.
