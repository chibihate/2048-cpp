#pragma once
#include "Service.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const int32_t ROWS = 4;
const int32_t COLS = 4;

class GLFWwindow;

enum Page {
    LOGIN,
    GAME
};

class Window {
public:
    Window();
    virtual ~Window();
    bool Init(const struct ApplicationConfiguration&);
    void Shutdown();
    void Swapbuffers();
    void PollsEvent();
    bool ShouldClose();
    void DearImGUI();
    virtual void InitGRPC() {};
    virtual void HandleScreen() {};

protected:
    Window(Window&) = default;
    GLFWwindow* windowGL;
};

class WindowServer : public Window {
public:
    WindowServer() {};
    virtual ~WindowServer() {};
    virtual void InitGRPC();
    virtual void HandleScreen();
private:
    ScoreServer _scoreServer;
    grpc::ServerBuilder _builder;
    std::unique_ptr<grpc::Server> _server;
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
    void GenerateRandom();
    void ShiftTile(Tile& first, Tile& second, Tile& third, Tile& forth);
    void MergeTile(Tile& first, Tile& second, Tile& third, Tile& forth);
    bool IsGameOver();

    ScoreClient _client;
    Page _screenPage;
    char _username[64], _password[64];
    ImGuiWindowFlags _defaultWindowFlags;
    std::array<std::array<Tile, COLS>, ROWS> _tiles;
};
