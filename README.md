# synth_ui_lib

A lightweight, cross-platform C++ UI framework for synthesizer and music production interfaces. Designed for low-latency, embedded-friendly rendering with immediate-mode graphics and a purpose-built synthwave color palette.

## Features

- **Zero external dependencies** — Self-contained C++14 library, perfect for embedded systems
- **Immediate-mode rendering** — Stateless widgets for seamless integration with real-time audio loops
- **13 interactive widgets** — Button, Slider, Dropdown, StepGrid (sequencer), ADSR Graph, and more
- **Audio-specific components** — Waveform display, Meter array, XY Pad for 2D parameter control
- **Built-in 5×7 bitmap font** — ASCII 32-127 rendering without external font assets
- **50-color RGB565 palette** — Synthwave-inspired theme optimized for music production UIs
- **Touch-first design** — Calibration support for capacitive/resistive touchscreens
- **Cross-platform** — macOS, Linux, Windows, and embedded systems (CMake-based)
- **Dual output** — Shared library (`.dylib`/`.so`/`.dll`) + static library for embedding

## Building

### Prerequisites

- CMake 3.16+
- C++14 compiler (GCC, Clang, MSVC)

### Build Instructions

```bash
cd /path/to/synth_ui_lib
mkdir build && cd build
cmake ..
make

# On macOS/Linux, produces:
# - libsynth_ui.dylib / libsynth_ui.so (shared)
# - libsynth_ui_static.a (static)

# On Windows, produces:
# - synth_ui.dll (shared)
# - synth_ui_static.lib (static)
```

The build automatically exports compile commands (`compile_commands.json`) for IDE integration.

## Architecture

### Core Layers

```
┌─────────────────────────────────────┐
│    Interactive Widgets (13)         │
│  Button, Slider, Dropdown, etc.     │
├─────────────────────────────────────┤
│  Audio-Specific Components (3 new)  │
│  Waveform, MeterArray, XYPad        │
├─────────────────────────────────────┤
│    Rendering Layer (Framebuffer)    │
│  Primitives, font, colors, overlays │
├─────────────────────────────────────┤
│  Input Layer (TouchEvent + Widget)  │
│  Touch calibration, capture, gesture│
│  dispatch, z-order hit-testing      │
├─────────────────────────────────────┤
│  Theme & Colors (50-color palette)  │
│  RGB565 colors, track palette       │
└─────────────────────────────────────┘
```

### Design Patterns

1. **Immediate-Mode Rendering** — Widgets are stateless; caller controls all rendering
2. **Value-Based State** — Draw methods accept state as parameters, not stored in widget
3. **Overlay System** — Dropdowns/popups deferred-render above other widgets without clipping
4. **Constexpr Everything** — Colors, glyphs, geometry pre-computed at compile time

## Quick Start

### Basic Widget Rendering

```cpp
#include "ui/framebuffer.h"
#include "ui/slider.h"
#include "ui/touch_dispatcher.h"
#include "ui/colors.h"

// Allocate framebuffer (RGB565, 320×240)
uint8_t buffer[320 * 240 * 2]; // 2 bytes per pixel (RGB565)
Framebuffer fb(buffer, 320, 240);

// Create a horizontal slider
Slider volumeSlider(10, 50, 200, 20, 0.0f, 1.0f, "Volume",
                    colors::ACCENT_1, colors::BG_DARK);

// Draw slider at 75% volume
float volumeLevel = 0.75f;
volumeSlider.draw(fb, volumeLevel);

// Handle touch input via dispatcher
TouchDispatcher dispatcher;
dispatcher.add(&volumeSlider);

TouchState points[] = {{.x = 110, .y = 50, .pressed = true}};
dispatcher.dispatch(points, 1);
volumeLevel = volumeSlider.getValue(); // Read updated value
```

### Waveform Display

```cpp
#include "ui/waveform.h"

Waveform waveDisplay(20, 100, 150, 80,
                     colors::ACCENT_2, colors::BG_DARK);

// Draw sine wave at 80% amplitude
waveDisplay.draw(fb, Waveform::SINE, 0.8f, 1.0f, true);

// Or display custom waveform from samples
float customSamples[] = {0.0f, 0.5f, 1.0f, 0.5f, -0.5f, -1.0f, -0.5f, 0.0f};
waveDisplay.drawCustom(fb, customSamples, 8, true);
```

### Meter Array

```cpp
#include "ui/meter_array.h"

// Create stereo meter array (2 channels, vertical style)
MeterArray meters(200, 50, 60, 150, 2,
                  MeterArray::VERTICAL,
                  colors::ACCENT_1, colors::BG_DARK);

// Update with current levels + peak holds
float levels[] = {0.7f, 0.6f};        // Current levels
float peaks[] = {0.9f, 0.85f};        // Peak holds

meters.draw(fb, levels, peaks, true); // showLabels = true
```

### XY Pad for 2D Control

```cpp
#include "ui/xy_pad.h"
#include "ui/touch_dispatcher.h"

XYPad xyControl(50, 50, 120,           // x, y, size
                colors::ACCENT_2,      // marker
                colors::BG_DARK,       // background
                colors::BG_MID);       // grid

float xParam = 0.5f, yParam = 0.5f;

// Draw with crosshair marker and grid
xyControl.draw(fb, xParam, yParam, XYPad::CROSSHAIR, true, false);

// Handle touch input via dispatcher
TouchDispatcher dispatcher;
dispatcher.add(&xyControl);

TouchState points[] = {{.x = 105, .y = 105, .pressed = true}};
dispatcher.dispatch(points, 1);
xParam = xyControl.getX(); // Read updated values
yParam = xyControl.getY();
```

### ADSR Envelope Editor

```cpp
#include "ui/graph.h"
#include "ui/touch_dispatcher.h"

Graph adsrEditor(10, 10, 200, 120,
                 colors::ACCENT_1, colors::BG_DARK);

// ADSR times in milliseconds
AdsrEnvelope env{10.0f, 100.0f, 0.7f, 200.0f};
adsrEditor.setEnvelope(env);

// Draw envelope
adsrEditor.draw(fb, env);

// Handle touch input via dispatcher
TouchDispatcher dispatcher;
dispatcher.add(&adsrEditor);

TouchState points[] = {{.x = 50, .y = 30, .pressed = true}};
dispatcher.dispatch(points, 1);
env = adsrEditor.getEnvelope(); // Read edited envelope
```

### Step Sequencer Grid

```cpp
#include "ui/stepgrid.h"

// 32-step × 8-track sequencer
StepGrid sequencer(10, 10, 300, 100,
                   32, 8,                    // steps × tracks
                   StepGrid::BARS,           // style
                   colors::ACCENT_1,
                   colors::BG_DARK);

bool pattern[32][8] = {...};  // Step states
uint8_t velocities[32][8] = {...};  // Per-step velocities

sequencer.draw(fb, pattern, velocities, 8); // playhead at step 8
sequencer.handleTouch(touch);
```

## Widget Gallery

| Widget | Purpose | Use Case |
|--------|---------|----------|
| **Button** | Momentary/latching input | Transport controls, mode selection |
| **Slider** | Continuous parameter (H/V) | Volume, cutoff frequency, LFO rate |
| **Toggle** | Binary on/off (compact) | Mute, effect bypass |
| **Checkbox** | Boolean selection | Settings, feature flags |
| **Radio** | Single-choice from group | Algorithm selection, waveform pick |
| **Dropdown** | Multi-option menu (4 styles) | Voice select, scale mode, routing |
| **Panel** | Container/grouping | Section headers, visual grouping |
| **TextField** | Text input + display | Patch names, numeric entry |
| **StepGrid** | 2D sequencer/pattern | Step sequencer, drum machine |
| **Graph** | ADSR envelope editor | Amplitude/filter envelopes |
| **HBar** | Progress/metering | Pitch cents display, simple meter |
| **Waveform** | Waveform visualization | Oscillator display, LFO preview |
| **MeterArray** | Multi-channel levels | Stereo/surround metering, mixer |
| **XYPad** | 2D parameter control | Filter sweep, morphing, mixing |

## Animation

Widgets stay stateless — the animation layer produces plain values (floats and
RGB565 colors) that you feed into the same `draw()` calls. Everything is driven
by the caller's `uint32_t` millisecond clock (the same stamp passed to
`TouchDispatcher::dispatch()`); there is no hidden clock, no update loop, and no
heap allocation. Three pieces, from simplest to most capable:

**1. Easing curves** (`ui/easing.h`) — a curated `Easing` enum
(`LINEAR`, `QUAD_*`, `CUBIC_*`, `BACK_OUT`, `SINE_INOUT`, `EXPO_OUT`,
`ELASTIC_OUT`, `BOUNCE_OUT`). Evaluate one with `applyEasing(easing, t)` for
`t` in `[0,1]` (input is clamped for you). Polynomial curves are `constexpr`.

**2. `Tween`** (`ui/tween.h`) — a caller-owned one-shot interpolation you store
next to the value it drives:

```cpp
#include "ui/tween.h"

Tween fade;
fade.start(0.0f, 1.0f, 250, Easing::CUBIC_OUT, now);  // 0→1 over 250ms

// ...each frame:
slider.draw(fb, fade.value(now));   // read the animated value, draw as usual
if (fade.done(now)) { /* settled */ }
```

`start()` also takes an optional `delayMs` and a `LoopMode`
(`ONCE`/`LOOP`/`PINGPONG`); `retarget(to, now)` redirects to a new target from
wherever it currently is (e.g. a slider chasing input). Helpers `lerp()`,
`lerpColor()` (RGB565 press-flash / state transitions), and `approach()` (a
one-pole smoother for meter ballistics with no fixed start/end) live here too.

**3. `Timeline`** (`ui/anim.h`) — a fixed-capacity, heap-free manager for
fire-and-forget and sequenced animations addressed by handle:

```cpp
#include "ui/anim.h"

Timeline timeline;
auto hIn  = timeline.add(0.0f, 1.0f, 200, Easing::CUBIC_OUT);   // fade in
auto hOut = timeline.then(hIn, 0.0f, 400, Easing::CUBIC_INOUT); // then out

// ...once per frame, before drawing:
timeline.tick(now);
panel.draw(fb, timeline.value(hIn));
```

`tick(now)` advances every animation in one O(N) pass over a small array; the
clock is captured on the first `tick()`, so `add()` needs no timestamp.
Finished `ONCE` animations free their slots automatically, and handles carry a
generation tag so reading a reused slot safely returns `0`. It slots into your
existing render pass with a single added line:

```cpp
uint32_t now = millis();
dispatcher.dispatch(points, n, now);   // existing input
timeline.tick(now);                    // NEW: advance all animations

fb.fillScreen(colors::BG_DARK);
graph.drawPlayheadLine(fb, graph.playheadX(phase, tween.value(now)),
                       colors::ACCENT_1);   // externally-animated position
```

### Data-driven sources (audio / video / any signal)

By themselves, the pieces above are *caller-driven* — you decide when to
`start()` a tween. To make the UI react to a live signal, `ui/anim_source.h`
adds sources that turn a pushed feature value into animation. The library is
**signal-agnostic**: it never parses audio or video. You extract a feature
(audio RMS/peak/FFT-band energy, video brightness/motion, MIDI velocity…) as a
plain `float` and push it — the *same* code drives any data source.

**`EnvelopeFollower`** — attack/release ballistics over a level (fast rise, slow
fall). The core audio-reactive primitive:

```cpp
#include "ui/anim_source.h"

EnvelopeFollower vol;
vol.configure(/*attackPerSec=*/1000.0f, /*releasePerSec=*/6.0f);

// audio side computes a feature, e.g. float rms = computeRms(buf, n);
// each UI frame:
float level = vol.push(rms, now);   // smoothed 0..1
meter.draw(fb, &level, nullptr);    // existing widget, unchanged
```

**`Trigger`** — fires a `Tween` pulse when a signal crosses a threshold (beat,
onset, gate), with hysteresis + a refractory window. `fire(now)` launches the
same pulse from any discrete event (MIDI note-on, a tap):

```cpp
Trigger beat;
// on/off thresholds, pulse 1→0 over 180ms, min 120ms between fires
beat.configure(0.6f, 0.4f, 1.0f, 0.0f, 180, Easing::CUBIC_OUT, 120);

beat.process(onsetStrength, now);   // or beat.fire(now) on note-on
uint16_t glow = lerpColor(colors::BG_DARK, colors::ACCENT_1, beat.value(now));
```

**`FollowerBank<N>`** — a heap-free bank of followers, so an FFT (audio) or a
region grid (video) drives the array-taking widgets directly:

```cpp
FollowerBank<8> spectrum;
spectrum.configure(1000.0f, 8.0f);

float levels[8];
spectrum.push(bandEnergies, 8, now);   // FFT bands or per-region brightness
spectrum.read(levels, 8);
meters.draw(fb, levels);               // MeterArray, unchanged
```

**`mapRange(x, inLo, inHi, outLo, outHi, easing)`** — remap a raw feature (dB,
0–255 brightness) into a UI range or a `lerpColor` `t`, with a curve and
saturating clamp.

Because it is signal-agnostic, driving the UI from **video** is the identical
pattern — push frame brightness into an `EnvelopeFollower`, or per-region
brightness into a `FollowerBank`; nothing audio-specific is involved.

A host-side smoke test covering the invariants (easing endpoints, tween
timing/looping, timeline sequencing, pool exhaustion, follower/trigger/bank
behavior) is in `examples/anim_demo.cpp` — see the header comment for the
one-line compile.

## Color Palette

The library includes 50 predefined colors optimized for music production:

```cpp
// Base colors
colors::BLACK, colors::WHITE

// UI theme
colors::BG_DARK, colors::BG_MID, colors::TEXT
colors::ACCENT_1, colors::ACCENT_2, colors::ACCENT_3, colors::ACCENT_4
colors::HIGHLIGHT, colors::WARN, colors::RECORD, colors::PLAY

// Track colors (8 distinct)
colors::TRACK_0 through colors::TRACK_7

// Custom RGB565 color creation
RGB565 customColor = RGB565(31, 63, 31); // Red, Green, Blue (5-6-5 bits)
```

## Touch Input & Calibration

```cpp
#include "ui/touch.h"
#include "ui/touch_dispatcher.h"

// Raw touch coordinates need calibration
TouchState rawTouch{.x = 512, .y = 256, .pressed = true};

// Set up affine transformation (calibrate with known points)
TouchCalibration calibration;
calibration.setMatrix(
    100, 0,   10,    // X: 100*raw_x + 0*raw_y + 10
    0,   100, 20     // Y: 0*raw_x + 100*raw_y + 20
);

TouchState calibratedTouch = calibration.transform(rawTouch);

// Create dispatcher, register widgets
TouchDispatcher dispatcher;
dispatcher.add(&myButton);
dispatcher.add(&mySlider);

// Dispatch touch events — the dispatcher handles hit-testing,
// touch capture, and gesture detection (tap, drag, long-press).
TouchState points[] = { calibratedTouch };
dispatcher.dispatch(points, 1);

// Query widget state after dispatch
if (myButton.wasTapped()) { /* button was tapped */ }
float val = mySlider.getValue(); // slider was dragged
```

## Integration Examples

### With Real-Time Audio Loop

```cpp
void audioCallback(float* audioBuffer, int numSamples) {
    static uint8_t fbBuffer[320 * 240 * 2];
    static Framebuffer fb(fbBuffer, 320, 240);
    static Slider volumeSlider(10, 50, 200, 20, colors::ACCENT_1, colors::BG_DARK);
    
    // Process audio
    for (int i = 0; i < numSamples; i++) {
        audioBuffer[i] *= volumeSlider.getValue();
    }
    
    // Update UI (called less frequently than audio)
    if (frameCounter++ % audioFramesPerUIFrame == 0) {
        fb.clear(colors::BG_DARK);
        volumeSlider.draw(fb, currentVolume, isDragging);
        displayFramebuffer(fb); // Send to screen
    }
}
```

### Embedded Teensy Integration

```cpp
// On Teensy with capacitive touchscreen + small display
#include <ILI9341_t3.h>

ILI9341_t3 tft(10, 8);  // CS, DC pins
Button recordBtn(100, 50, 60, 40, colors::RECORD, colors::BG_DARK);
TouchDispatcher dispatcher;

void setup() {
    tft.begin();
    dispatcher.add(&recordBtn);
}

void loop() {
    TouchState touch = readCapacitiveTouch();
    dispatcher.dispatch(&touch, 1);
    
    if (recordBtn.wasTapped()) {
        recordState = !recordState;
    }
    
    uint16_t* fbBuffer = (uint16_t*)tft.getFrameBuffer();
    Framebuffer fb((uint8_t*)fbBuffer, 320, 240);
    recordBtn.draw(fb, recordState);
    
    tft.updateScreen();
}
```

## Performance

- **Frame time**: 3-8ms for full 320×240 rerender (macOS, all widgets)
- **Memory**: ~150KB for static library
- **Dependencies**: Zero external libraries
- **Target**: 60 FPS on resource-constrained devices (Teensy 4.0+)

## File Structure

```
synth_ui_lib/
├── CMakeLists.txt              # Cross-platform build config
├── README.md                   # This file
├── library.json                # Package metadata
└── src/ui/
    ├── widget.h/cpp            # Widget base class (geometry, touch capture, gestures)
    ├── touch_event.h           # TouchPhase enum, TouchEvent struct
    ├── touch_dispatcher.h/cpp  # Touch routing, hit-testing, gesture detection
    ├── colors.h                # 50-color palette
    ├── framebuffer.h/cpp       # Rendering primitives
    ├── touch.h/cpp             # Touch input & calibration (TouchState, TouchCalibration)
    ├── button.h/cpp            # Momentary/latching button
    ├── slider.h/cpp            # Horizontal/vertical slider
    ├── dropdown.h/cpp          # Multi-style dropdown menu
    ├── stepgrid.h/cpp          # 32-step × 8-track sequencer
    ├── graph.h/cpp             # ADSR envelope editor
    ├── waveform.h/cpp          # Waveform display (NEW)
    ├── meter_array.h/cpp       # Multi-channel metering (NEW)
    ├── xy_pad.h/cpp            # 2D parameter control (NEW)
    ├── easing.h                # Easing curves (Easing enum, applyEasing)
    ├── tween.h                 # Tween + lerp/lerpColor/approach helpers
    ├── anim.h/cpp              # Timeline animation manager
    ├── anim_source.h           # Data-driven sources (follower, trigger, bank)
    └── [other widgets...]
```

## Development

### Building from Source

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Cross-Platform Builds

The library automatically detects the platform:

- **macOS**: Generates `.dylib` shared + `.a` static
- **Linux**: Generates `.so` shared + `.a` static
- **Windows**: Generates `.dll` shared + `.lib` static

### IDE Integration

CMake exports `compile_commands.json` for Clang, IntelliSense, and LSP integration.

## License

[Add your license here]

## Contributing

Contributions welcome! Please ensure:

1. Code follows C++14 conventions
2. No external dependencies added
3. Cross-platform compatibility (test on macOS, Linux, Windows)
4. Performance-conscious (immediate-mode, constexpr where possible)

## Support

- GitHub Issues: [report bugs]
- Documentation: See [ARCHITECTURE.md](ARCHITECTURE.md) for design details
