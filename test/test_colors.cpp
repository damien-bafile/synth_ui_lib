#include <gtest/gtest.h>
#include "ui/colors.h"

static constexpr uint16_t MASK_R = 0xF800;
static constexpr uint16_t MASK_G = 0x07E0;
static constexpr uint16_t MASK_B = 0x001F;

TEST(ColorsTest, RGB565) {
    EXPECT_EQ(0xFFFF, ui::RGB565(255, 255, 255));
    EXPECT_EQ(0x0000, ui::RGB565(0, 0, 0));
    EXPECT_EQ(0xF800, ui::RGB565(255, 0, 0));
    EXPECT_EQ(0x07E0, ui::RGB565(0, 255, 0));
    EXPECT_EQ(0x001F, ui::RGB565(0, 0, 255));
    EXPECT_EQ(0x8410, ui::RGB565(128, 128, 128));

    uint16_t c = ui::RGB565(10, 20, 30);
    uint8_t r = (c & MASK_R) >> 11;
    uint8_t g = (c & MASK_G) >> 5;
    uint8_t b = c & MASK_B;
    EXPECT_EQ(10 >> 3, r);
    EXPECT_EQ(20 >> 2, g);
    EXPECT_EQ(30 >> 3, b);
}

TEST(ColorsTest, Clamp) {
    EXPECT_FLOAT_EQ(0.5f, ui::clamp(0.5f, 0.0f, 1.0f));
    EXPECT_FLOAT_EQ(0.0f, ui::clamp(-0.1f, 0.0f, 1.0f));
    EXPECT_FLOAT_EQ(1.0f, ui::clamp(1.5f, 0.0f, 1.0f));
    EXPECT_FLOAT_EQ(5.0f, ui::clamp(5.0f, 5.0f, 10.0f));
    EXPECT_FLOAT_EQ(10.0f, ui::clamp(10.0f, 5.0f, 10.0f));
}

TEST(ColorsTest, ThemeDefaultMocha) {
    EXPECT_EQ(ui::Theme::MOCHA, ui::currentTheme());
    EXPECT_EQ(ui::RGB565(17, 17, 27), ui::BG_DARK);
    EXPECT_EQ(ui::RGB565(205, 214, 244), ui::TEXT);
}

TEST(ColorsTest, SetThemeLatte) {
    ui::setTheme(ui::Theme::LATTE);
    EXPECT_EQ(ui::Theme::LATTE, ui::currentTheme());
    EXPECT_EQ(ui::RGB565(220, 224, 232), ui::BG_DARK);
    EXPECT_EQ(ui::RGB565(76, 79, 105), ui::TEXT);
    ui::setTheme(ui::Theme::MOCHA);
    EXPECT_EQ(ui::RGB565(17, 17, 27), ui::BG_DARK);
}

TEST(ColorsTest, SetThemeCyberpunk) {
    ui::setTheme(ui::Theme::CYBERPUNK_NEON);
    EXPECT_EQ(ui::Theme::CYBERPUNK_NEON, ui::currentTheme());
    EXPECT_EQ(ui::RGB565(10, 189, 198), ui::TEXT);
    ui::setTheme(ui::Theme::MOCHA);
}

TEST(ColorsTest, SetThemeCyberdream) {
    ui::setTheme(ui::Theme::CYBERDREAM_LIGHT);
    EXPECT_EQ(ui::Theme::CYBERDREAM_LIGHT, ui::currentTheme());
    EXPECT_EQ(ui::RGB565(255, 255, 255), ui::BG_SURFACE);
    ui::setTheme(ui::Theme::MOCHA);
}
