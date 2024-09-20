#pragma once
#include "Tile.h"
#include "Window.h"
#include "ServiceClient.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const ImU32 OUTLINE_COLOR = IM_COL32(187, 173, 160, 255);

class WindowClient : public Window {
public:
    WindowClient();
    virtual ~WindowClient() {};
    virtual void InitGRPC();
    virtual void HandleScreen();
private:
    void LoginPage();
    void GamePage();
    void DrawGrid(ImDrawList* drawList);
    bool IsGameOver();
    void ResetTiles();

    ScoreClient _client;
    Page _screenPage;
    char _username[64], _password[64];
    ImGuiWindowFlags _defaultWindowFlags;

    std::array<std::array<Tile, COLS>, ROWS> _tiles;
};