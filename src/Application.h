#pragma once

#include <memory>
#include "Window.h"

enum EWindow {
    CLIENT,
    SERVER
};

struct ApplicationConfiguration {
    int width, height;
    const char* title;
    EWindow window;
};

class Application {
public:
    virtual ~Application() = default;
    bool Init();
    void Run();
    void Shutdown();
protected:
    Application() = default;
    Application(const ApplicationConfiguration&);
private:
    ApplicationConfiguration _config;
    std::unique_ptr<Window> _window;
};

// extern Application* CreateApplication();