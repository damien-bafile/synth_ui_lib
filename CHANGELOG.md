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
- Unit test coverage extended to every widget class (previously only
  base/infra classes had tests, then 8 core interactive widgets): all
  ~25 pre-existing widgets now have unit tests, following the same
  construct/simulate-touch/assert pattern.
- Added 8 new widgets closing gaps from the initial audit:
  `PianoKeyboard` (note-range piano input), `SpectrumView` (spectrum/FFT
  display), `ModMatrix` (modulation routing grid), `Tooltip` (floating
  text bubble), `ListView` (scrollable list with optional drag reorder),
  `Spinbox` (stepped numeric field), `MidiLearnBadge` (pulsing MIDI-learn
  indicator). Each has its own unit tests and a `ui-widget-summary.org`
  entry.
- Fixed `RadialDial::setBounds()` storing the raw top-left it was given
  as `x_,y_`, while `draw()`/`isInside()` both use `x_,y_` directly as
  the pixel *center* — every dial rendered and hit-tested shifted up-left
  by its own radius from where callers positioned it (constructor and
  all `setBounds()` call sites pass top-left, per the standard `Widget`
  contract). `setBounds()` now converts top-left to center internally,
  so no caller needs to change.
- `TEXT_DIM` brightened (`RGB565(127,132,156)` -> `RGB565(190,195,215)`)
  to clear ~4.5:1 contrast against `BG_MID`/`BG_SURFACE` in the Mocha
  theme; the previous value measured ~3.3-3.9:1.

## [1.0.0]

Baseline: ~28 widgets (controls, display/data-viz, composite/overlay),
a 4-theme color system, touch/gesture dispatch, and an animation system
(easing, tweening, timelines). See `ui-widget-summary.org` for the full
widget catalog.
