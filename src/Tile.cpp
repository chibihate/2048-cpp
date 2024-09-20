#include "Tile.h"
#include <string>
#include <cmath>

Tile::Tile()
    : _value(0)
    , _x(0)
    , _y(0)
{
}

Tile::Tile(uint16_t row, uint16_t col)
    : _value(0)
    , _x(col*RECT_WIDTH)
    , _y(row*RECT_HEIGHT)
{
}

const ImU32 Tile::GetColor() const
{
    if (_value == 0)
    {
        return BACKGROUND_COLOR;
    }
    ImU32 color;
    int32_t colorIndex = int(log2(double(_value))) - 1;
    colorIndex = colorIndex > 10 ? 10 : colorIndex;
    color = COLORS[colorIndex];
    return color;
}

void Tile::Draw(ImDrawList* drawList)
{
    ImU32 color = GetColor();
    drawList->AddRectFilled(ImVec2(_x,_y),ImVec2(_x+RECT_WIDTH, _y+RECT_HEIGHT), color);

    if (_value != 0)
    {
        const char* text = std::to_string(_value).c_str();
        ImVec2 text_size = ImGui::CalcTextSize(text);
        ImVec2 text_pos = ImVec2(
            _x + RECT_WIDTH / 2 - text_size.x / 2,
            _y + RECT_HEIGHT / 2 - text_size.y / 2
        );
        drawList->AddText(text_pos, IM_COL32_BLACK, text);
    }
}

void Tile::SetValue(uint16_t value)
{
    _value = value;
}

const uint16_t Tile::GetValue() const
{
    return _value;
}

const uint16_t Tile::GetX() const
{
    return _x;
}

const uint16_t Tile::GetY() const
{
    return _y;
}