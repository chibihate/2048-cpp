#pragma once

#include <memory>
#include "Window.h"

struct ApplicationConfiguration {
    int width, height;
    const char* title;
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
    ApplicationConfiguration config;
    std::unique_ptr<Window> window;
};

extern Application* CreateApplication();