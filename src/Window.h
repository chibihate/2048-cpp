#pragma once

class GLFWwindow;

struct WindowData
{
    int keyCode;
};

class Window {
public:
    Window();
    virtual ~Window();
    virtual bool Init(const struct ApplicationConfiguration&);
    virtual void Shutdown();
    virtual void Swapbuffers();
    virtual void PollsEvent();
    virtual bool ShouldClose();
    virtual void UpdateScreen();

protected:
    Window(Window&) = default;
private:
    GLFWwindow* windowGL;
    WindowData windowData;
};