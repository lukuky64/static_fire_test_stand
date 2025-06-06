#pragma once
#include <Arduino.h>

// https://javl.github.io/image2cpp/

// A simple structure to hold bitmap data.
struct Bitmap
{
    uint8_t width;
    uint8_t height;
    const uint8_t *bmp; // pointer to raw bitmap bits
};

static const uint8_t PROGMEM logoBMP[] =
    {
        0b00000000, 0b11000000,
        0b00000001, 0b11000000,
        0b00000001, 0b11000000,
        0b00000011, 0b11100000,
        0b11110011, 0b11100000,
        0b11111110, 0b11111000,
        0b01111110, 0b11111111,
        0b00110011, 0b10011111,
        0b00011111, 0b11111100,
        0b00001101, 0b01110000,
        0b00011011, 0b10100000,
        0b00111111, 0b11100000,
        0b00111111, 0b11110000,
        0b01111100, 0b11110000,
        0b01110000, 0b01110000,
        0b00000000, 0b00110000};

static const Bitmap logo = {16, 16, logoBMP};

// 16x16 icons

// 'armed_off', 16x16px
static const uint8_t armed_off_BMP[] PROGMEM = {
    0x00, 0x00, 0x3f, 0xfc, 0x20, 0x04, 0x2f, 0xf4, 0x28, 0x14, 0x28, 0x1c, 0x28, 0x00, 0x28, 0x00,
    0x7f, 0xfe, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x7f, 0xfe, 0x00, 0x00};
// 'armed_on', 16x16px
static const uint8_t armed_on_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x38, 0x1c, 0x38, 0x1c,
    0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00};
// 'load_cell_off', 16x16px
static const uint8_t load_cell_off_BMP[] PROGMEM = {
    0x00, 0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x08, 0x10, 0x78, 0x1e, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06,
    0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x78, 0x1e, 0x08, 0x10, 0x0f, 0xf0, 0x0f, 0xf0, 0x00, 0x00};
// 'load_cell_on', 16x16px
static const uint8_t load_cell_on_BMP[] PROGMEM = {
    0x00, 0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe,
    0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x00, 0x00};
// 'ready_on', 16x16px
static const uint8_t ready_on_BMP[] PROGMEM = {
    0x00, 0x00, 0x7e, 0xc2, 0x7e, 0xce, 0x66, 0xdc, 0x66, 0xf8, 0x66, 0xe0, 0x66, 0xc0, 0x66, 0xc0,
    0x66, 0xc0, 0x66, 0xc0, 0x66, 0xe0, 0x66, 0xf8, 0x66, 0xdc, 0x7e, 0xce, 0x7e, 0xc2, 0x00, 0x00};
// all black
static const uint8_t ready_off_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 'RF_on', 16x16px
static const uint8_t RF_on_BMP[] PROGMEM = {0x00, 0x00, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x00, 0x00, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00};
// 'RF_off', 16x16px
static const uint8_t RF_off_BMP[] PROGMEM = {
    0x00, 0x00, 0x7f, 0xfe, 0x40, 0x02, 0x7f, 0xfe, 0x00, 0x00, 0x1f, 0xf8, 0x10, 0x08, 0x1f, 0xf8,
    0x00, 0x00, 0x07, 0xe0, 0x04, 0x20, 0x07, 0xe0, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00};
// 'SD_off', 16x16px
static const uint8_t SD_off_BMP[] PROGMEM = {
    0x00, 0x00, 0x1f, 0xfe, 0x1a, 0xaa, 0x72, 0xaa, 0x62, 0xaa, 0x40, 0x02, 0x40, 0x02, 0x4e, 0xe2,
    0x48, 0x92, 0x4e, 0x92, 0x42, 0x92, 0x4e, 0xe2, 0x40, 0x02, 0x40, 0x02, 0x7f, 0xfe, 0x00, 0x00};
// 'SD_on', 16x16px
static const uint8_t SD_on_BMP[] PROGMEM = {
    0x00, 0x00, 0x1f, 0xfe, 0x1d, 0x56, 0x7d, 0x56, 0x7d, 0x56, 0x7f, 0xfe, 0x7f, 0xfe, 0x71, 0x1e,
    0x77, 0x6e, 0x71, 0x6e, 0x7d, 0x6e, 0x71, 0x1e, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00};

static const Bitmap armed_off = {16, 16, armed_off_BMP};
static const Bitmap armed_on = {16, 16, armed_on_BMP};
static const Bitmap load_cell_off = {16, 16, load_cell_off_BMP};
static const Bitmap load_cell_on = {16, 16, load_cell_on_BMP};
static const Bitmap ready_on = {16, 16, ready_on_BMP};
static const Bitmap ready_off = {16, 16, ready_off_BMP};
static const Bitmap RF_on = {16, 16, RF_on_BMP};
static const Bitmap RF_off = {16, 16, RF_off_BMP};
static const Bitmap SD_off = {16, 16, SD_off_BMP};
static const Bitmap SD_on = {16, 16, SD_on_BMP};

// 48 x 16 icons

// 'batt_0', 48x16px
static const uint8_t batt_0_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00,
    0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00,
    0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'batt_10', 48x16px
static const uint8_t batt_10_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0x00,
    0x00, 0x00, 0x00, 0x06, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x06,
    0x6e, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0x00, 0x00, 0x00,
    0x00, 0x06, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'batt_20', 48x16px
static const uint8_t batt_20_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xe0, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xe0,
    0x00, 0x00, 0x00, 0x06, 0x6e, 0xe0, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xe0, 0x00, 0x00, 0x00, 0x06,
    0x6e, 0xe0, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xe0, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xe0, 0x00, 0x00,
    0x00, 0x06, 0x6e, 0xe0, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 'batt_30', 48x16px
static const uint8_t batt_30_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee,
    0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0x00, 0x00, 0x00, 0x06,
    0x6e, 0xee, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0x00, 0x00,
    0x00, 0x06, 0x6e, 0xee, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 'batt_40', 48x16px
static const uint8_t batt_40_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xe0, 0x00, 0x00, 0x06, 0x6e, 0xee,
    0xe0, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xe0, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xe0, 0x00, 0x00, 0x06,
    0x6e, 0xee, 0xe0, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xe0, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xe0, 0x00,
    0x00, 0x06, 0x6e, 0xee, 0xe0, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'batt_50', 48x16px
static const uint8_t batt_50_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0x00, 0x00, 0x06, 0x6e, 0xee,
    0xee, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0x00, 0x00, 0x06,
    0x6e, 0xee, 0xee, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0x00,
    0x00, 0x06, 0x6e, 0xee, 0xee, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'batt_60', 48x16px
static const uint8_t batt_60_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xe0, 0x00, 0x06, 0x6e, 0xee,
    0xee, 0xe0, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xe0, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xe0, 0x00, 0x06,
    0x6e, 0xee, 0xee, 0xe0, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xe0, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xe0,
    0x00, 0x06, 0x6e, 0xee, 0xee, 0xe0, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'batt_70', 48x16px
static const uint8_t batt_70_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xee, 0x00, 0x06, 0x6e, 0xee,
    0xee, 0xee, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xee, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xee, 0x00, 0x06,
    0x6e, 0xee, 0xee, 0xee, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xee, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xee,
    0x00, 0x06, 0x6e, 0xee, 0xee, 0xee, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'batt_80', 48x16px
static const uint8_t batt_80_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xe0, 0x06, 0x6e, 0xee,
    0xee, 0xee, 0xe0, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xe0, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xe0, 0x06,
    0x6e, 0xee, 0xee, 0xee, 0xe0, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xe0, 0x06, 0x6e, 0xee, 0xee, 0xee,
    0xe0, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xe0, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'batt_90', 48x16px
static const uint8_t batt_90_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xee, 0x06, 0x6e, 0xee,
    0xee, 0xee, 0xee, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xee, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xee, 0x06,
    0x6e, 0xee, 0xee, 0xee, 0xee, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xee, 0x06, 0x6e, 0xee, 0xee, 0xee,
    0xee, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xee, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'batt_100', 48x16px
static const uint8_t batt_100_BMP[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x6e, 0xee, 0xee, 0xee, 0xee, 0xe6, 0x6e, 0xee,
    0xee, 0xee, 0xee, 0xe6, 0x6e, 0xee, 0xee, 0xee, 0xee, 0xe6, 0x6e, 0xee, 0xee, 0xee, 0xee, 0xe6,
    0x6e, 0xee, 0xee, 0xee, 0xee, 0xe6, 0x6e, 0xee, 0xee, 0xee, 0xee, 0xe6, 0x6e, 0xee, 0xee, 0xee,
    0xee, 0xe6, 0x6e, 0xee, 0xee, 0xee, 0xee, 0xe6, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const Bitmap batt_0 = {48, 16, batt_0_BMP};
static const Bitmap batt_10 = {48, 16, batt_10_BMP};
static const Bitmap batt_20 = {48, 16, batt_20_BMP};
static const Bitmap batt_30 = {48, 16, batt_30_BMP};
static const Bitmap batt_40 = {48, 16, batt_40_BMP};
static const Bitmap batt_50 = {48, 16, batt_50_BMP};
static const Bitmap batt_60 = {48, 16, batt_60_BMP};
static const Bitmap batt_70 = {48, 16, batt_70_BMP};
static const Bitmap batt_80 = {48, 16, batt_80_BMP};
static const Bitmap batt_90 = {48, 16, batt_90_BMP};
static const Bitmap batt_100 = {48, 16, batt_100_BMP};
