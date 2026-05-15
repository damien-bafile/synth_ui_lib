"""Python ctypes bindings for synth_ui_lib.

Usage:
    from synth_ui import FB, fill_rect, draw_text, UI_ACCENT_1

    fb = FB(bytearray(320 * 480 * 2), 320, 480)
    fill_rect(fb, 0, 0, 320, 240, UI_BG_DARK)
    draw_text(fb, 4, 4, "Hello", UI_TEXT, UI_BG_DARK)
"""

import ctypes
import os
from ctypes import POINTER, c_char_p, c_int, c_uint16, c_void_p

_LIB = None

# Resolve the .dylib path.
# Priority: SYNTH_UI_PATH env var, then ~/Dev/synth_ui_lib/build/synth_ui.dylib
_DEFAULT_PATH = os.path.expanduser("~/Dev/synth_ui_lib/build/synth_ui.dylib")
_LIB_PATH = os.environ.get("SYNTH_UI_PATH", _DEFAULT_PATH)


def _load():
    global _LIB
    if _LIB is not None:
        return _LIB
    if not os.path.exists(_LIB_PATH):
        raise FileNotFoundError(
            f"synth_ui.dylib not found at {_LIB_PATH}. "
            f"Run: cmake -B build -S ~/Dev/synth_ui_lib && cmake --build ~/Dev/synth_ui_lib/build"
        )
    _LIB = ctypes.CDLL(_LIB_PATH)
    _LIB.ui_set_pixel.argtypes = [c_void_p, c_int, c_int, c_uint16]
    _LIB.ui_fill_rect.argtypes = [c_void_p, c_int, c_int, c_int, c_int, c_uint16]
    _LIB.ui_draw_rect.argtypes = [c_void_p, c_int, c_int, c_int, c_int, c_uint16]
    _LIB.ui_fill_screen.argtypes = [c_void_p, c_uint16]
    _LIB.ui_draw_char.argtypes = [c_void_p, c_int, c_int, c_char_p, c_uint16, c_uint16]
    _LIB.ui_draw_text.argtypes = [c_void_p, c_int, c_int, c_char_p, c_uint16, c_uint16]
    _LIB.ui_text_width.argtypes = [c_char_p]
    _LIB.ui_text_width.restype = c_int
    _LIB.ui_grid_cell.argtypes = [
        POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int),
        c_int, c_int, c_int, c_int, c_int, c_int, c_int,
    ]
    return _LIB


def _fb_ptr(fb):
    if isinstance(fb, c_void_p):
        return fb
    return ctypes.cast(fb.data_ptr, c_void_p)


class FB:
    """Wraps a Python bytearray as a framebuffer for the shared library."""

    def __init__(self, buf, width: int, height: int):
        if isinstance(buf, bytearray):
            self._buf = buf
            self._ptr = (ctypes.c_uint8 * len(buf)).from_buffer(buf)
            self.data_ptr = ctypes.cast(self._ptr, c_void_p)
        elif isinstance(buf, bytes):
            ba = bytearray(buf)
            self._buf = ba
            self._ptr = (ctypes.c_uint8 * len(ba)).from_buffer(ba)
            self.data_ptr = ctypes.cast(self._ptr, c_void_p)
        else:
            raise TypeError("Expected bytearray or bytes")
        self.width = width
        self.height = height

    @property
    def raw(self) -> memoryview:
        return memoryview(self._buf)


# ---- Public functions ----

def set_pixel(fb, x, y, color):
    _load().ui_set_pixel(_fb_ptr(fb), x, y, color)


def fill_rect(fb, x, y, w, h, color):
    _load().ui_fill_rect(_fb_ptr(fb), x, y, w, h, color)


def draw_rect(fb, x, y, w, h, color):
    _load().ui_draw_rect(_fb_ptr(fb), x, y, w, h, color)


def fill_screen(fb, color):
    _load().ui_fill_screen(_fb_ptr(fb), color)


def draw_char(fb, x, y, ch, fg, bg):
    _load().ui_draw_char(_fb_ptr(fb), x, y, ch.encode("ascii"), fg, bg)


def draw_text(fb, x, y, text, fg, bg):
    _load().ui_draw_text(_fb_ptr(fb), x, y, text.encode("ascii"), fg, bg)


def text_width(text):
    return _load().ui_text_width(text.encode("ascii"))


def grid_cell(fb, col, row, cols, rows, padding=1):
    x = c_int()
    y = c_int()
    w = c_int()
    h = c_int()
    _load().ui_grid_cell(
        ctypes.byref(x), ctypes.byref(y), ctypes.byref(w), ctypes.byref(h),
        col, row, cols, rows, fb.width, fb.height, padding,
    )
    return (x.value, y.value, w.value, h.value)


# ---- Color palette (mirrors ui.h) ----

UI_BLACK      = 0x0000
UI_WHITE      = 0xFFFF
UI_RED        = 0xF800
UI_GREEN      = 0x07E0
UI_BLUE       = 0x001F
UI_YELLOW     = 0xFFE0
UI_CYAN       = 0x07FF
UI_MAGENTA    = 0xF81F

UI_GRAY_DARK  = 0x2965
UI_GRAY_MID   = 0x630C
UI_GRAY_LIGHT = 0xBDF7
UI_ORANGE     = 0xFD20
UI_PURPLE     = 0x881F
UI_TEAL       = 0x05D6
UI_PINK       = 0xFC18
UI_LIME       = 0x87E0
UI_NAVY       = 0x000F
UI_MAROON     = 0x7800
UI_OLIVE      = 0x6380
UI_SILVER     = 0xCE79
UI_GOLD       = 0xFD40
UI_CORAL      = 0xFCD0

UI_BG_DARK    = 0x10A4
UI_BG_MID     = 0x2108
UI_BG_SURFACE = 0x39AC
UI_TEXT       = 0xDEDB
UI_TEXT_DIM   = 0x7BEF
UI_ACCENT_1   = 0xF99E
UI_ACCENT_2   = 0x2EBF
UI_ACCENT_3   = 0xB51F
UI_ACCENT_4   = 0x36F0
UI_HIGHLIGHT  = 0xFD60
UI_WARN       = 0xFC00
UI_RECORD     = 0xF880
UI_PLAY       = 0x07E0
UI_MUTE       = 0xFD60
UI_SOLO       = 0xFFE0

UI_TRACK_0 = 0xF99E
UI_TRACK_1 = 0x2EBF
UI_TRACK_2 = 0xB51F
UI_TRACK_3 = 0x36F0
UI_TRACK_4 = 0xFD60
UI_TRACK_5 = 0xFCA0
UI_TRACK_6 = 0x8EE0
UI_TRACK_7 = 0xC9CE

_TRACK_COLORS = [
    UI_TRACK_0, UI_TRACK_1, UI_TRACK_2, UI_TRACK_3,
    UI_TRACK_4, UI_TRACK_5, UI_TRACK_6, UI_TRACK_7,
]


def track_color(index: int) -> int:
    return _TRACK_COLORS[index % len(_TRACK_COLORS)]
