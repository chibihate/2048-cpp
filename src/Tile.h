#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdint>

const int32_t ROWS = 4;
const int32_t COLS = 4;

const int32_t GAME_WIDTH = 480;
const int32_t GAME_HEIGHT = 480;
const int32_t RECT_WIDTH = GAME_WIDTH / ROWS;
const int32_t RECT_HEIGHT = GAME_HEIGHT / COLS;

const ImU32 BACKGROUND_COLOR = IM_COL32(205,193,180,255);

const ImU32 COLORS[11] = {
    IM_COL32(238, 228, 218, 255),
    IM_COL32(237, 224, 200, 255),
    IM_COL32(242, 177, 121, 255),
    IM_COL32(245, 149, 99, 255),
    IM_COL32(246, 124, 95, 255),
    IM_COL32(246, 94, 59, 255),
    IM_COL32(237, 207, 114, 255),
    IM_COL32(237, 204, 97, 255),
    IM_COL32(237, 200, 80, 255),
    IM_COL32(237, 197, 63, 255),
    IM_COL32(237, 194, 46, 255),
};

class Tile {
public:
    Tile();
    Tile(uint16_t row, uint16_t col);
    virtual ~Tile() {};
    const ImU32 GetColor() const;
    void Draw(ImDrawList* drawList);

    void SetValue(uint16_t value);
    const uint16_t GetValue() const;
    const uint16_t GetX() const;
    const uint16_t GetY() const;

private:    
    uint16_t _value;
    uint16_t _x;
    uint16_t _y;
};