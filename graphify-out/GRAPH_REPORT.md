# Graph Report - .  (2026-06-13)

## Corpus Check
- Corpus is ~2,093 words - fits in a single context window. You may not need a graph.

## Summary
- 29 nodes · 42 edges · 8 communities (4 shown, 4 thin omitted)
- Extraction: 88% EXTRACTED · 12% INFERRED · 0% AMBIGUOUS · INFERRED: 5 edges (avg confidence: 0.89)
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- [[_COMMUNITY_Glyph Rendering & Icons|Glyph Rendering & Icons]]
- [[_COMMUNITY_Text Rendering & Grid|Text Rendering & Grid]]
- [[_COMMUNITY_Framebuffer & Rect Drawing|Framebuffer & Rect Drawing]]
- [[_COMMUNITY_Build System & Library|Build System & Library]]
- [[_COMMUNITY_Color System|Color System]]
- [[_COMMUNITY_Icon Enums|Icon Enums]]
- [[_COMMUNITY_Framebuffer Type|Framebuffer Type]]

## God Nodes (most connected - your core abstractions)
1. `ui_fb_t` - 8 edges
2. `ui_draw_char()` - 7 edges
3. `ui_draw_synth_icon()` - 7 edges
4. `ui_draw_fx_icon()` - 7 edges
5. `ui_set_pixel()` - 6 edges
6. `ui_fill_rect()` - 4 edges
7. `ui_draw_text()` - 4 edges
8. `ui_draw_rect()` - 3 edges
9. `ui_fill_screen()` - 3 edges
10. `pack_rgb565()` - 2 edges

## Surprising Connections (you probably didn't know these)
- `synth_ui_lib (shared)` --conceptually_related_to--> `synth_ui_lib`  [EXTRACTED]
  CMakeLists.txt → library.json
- `synth_ui_lib_static (static)` --conceptually_related_to--> `synth_ui_lib`  [EXTRACTED]
  CMakeLists.txt → library.json
- `ui_draw_char()` --shares_data_with--> `font_5x7`  [EXTRACTED]
  src/ui.c → src/ui_font_5x7.h
- `ui_draw_synth_icon()` --shares_data_with--> `synth_glyphs`  [EXTRACTED]
  src/ui.c → src/ui_icons.h
- `ui_draw_fx_icon()` --shares_data_with--> `fx_glyphs`  [EXTRACTED]
  src/ui.c → src/ui_icons.h

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **Drawing Primitives Layer** — src_ui_pack_rgb565, src_ui_ui_set_pixel, src_ui_ui_fill_rect, src_ui_ui_draw_rect, src_ui_ui_fill_screen [INFERRED 0.95]
- **5x7 Bitmap Glyph Rendering Pattern** — src_ui_ui_draw_char, src_ui_ui_draw_synth_icon, src_ui_ui_draw_fx_icon [INFERRED 0.95]
- **Synth & FX Icon Subsystem** — src_ui_icons_synthicon, src_ui_icons_synth_glyphs, src_ui_icons_fxicon, src_ui_icons_fx_glyphs, src_ui_ui_draw_synth_icon, src_ui_ui_draw_fx_icon [EXTRACTED 1.00]

## Communities (8 total, 4 thin omitted)

### Community 0 - "Glyph Rendering & Icons"
Cohesion: 0.27
Nodes (10): FxIcon, font_5x7, fx_glyphs, synth_glyphs, pack_rgb565(), ui_draw_char(), ui_draw_fx_icon(), ui_draw_synth_icon() (+2 more)

### Community 2 - "Framebuffer & Rect Drawing"
Cohesion: 0.83
Nodes (4): ui_draw_rect(), ui_fill_rect(), ui_fill_screen(), ui_fb_t

### Community 3 - "Build System & Library"
Cohesion: 0.67
Nodes (3): synth_ui_lib (shared), synth_ui_lib_static (static), synth_ui_lib

## Knowledge Gaps
- **11 isolated node(s):** `SynthIcon`, `FxIcon`, `synth_ui_lib (shared)`, `synth_ui_lib_static (static)`, `ui_fb_t` (+6 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **4 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `ui_draw_synth_icon()` connect `Glyph Rendering & Icons` to `Text Rendering & Grid`, `Framebuffer & Rect Drawing`?**
  _High betweenness centrality (0.094) - this node is a cross-community bridge._
- **Why does `ui_draw_fx_icon()` connect `Glyph Rendering & Icons` to `Text Rendering & Grid`, `Framebuffer & Rect Drawing`?**
  _High betweenness centrality (0.094) - this node is a cross-community bridge._
- **Why does `ui_draw_char()` connect `Glyph Rendering & Icons` to `Text Rendering & Grid`, `Framebuffer & Rect Drawing`?**
  _High betweenness centrality (0.055) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `ui_draw_char()` (e.g. with `ui_draw_fx_icon()` and `ui_draw_synth_icon()`) actually correct?**
  _`ui_draw_char()` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Are the 2 inferred relationships involving `ui_draw_synth_icon()` (e.g. with `ui_draw_char()` and `ui_draw_fx_icon()`) actually correct?**
  _`ui_draw_synth_icon()` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Are the 2 inferred relationships involving `ui_draw_fx_icon()` (e.g. with `ui_draw_char()` and `ui_draw_synth_icon()`) actually correct?**
  _`ui_draw_fx_icon()` has 2 INFERRED edges - model-reasoned connections that need verification._
- **What connects `SynthIcon`, `FxIcon`, `synth_ui_lib (shared)` to the rest of the system?**
  _12 weakly-connected nodes found - possible documentation gaps or missing edges._