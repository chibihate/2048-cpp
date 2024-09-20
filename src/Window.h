#pragma once
#include "Tile.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class GLFWwindow;

enum Page {
    LOGIN,
    GAME
};

const int32_t HEIGHT_TITLE_GAME = 60;

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
