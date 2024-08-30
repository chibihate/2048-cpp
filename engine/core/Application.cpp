#include <iostream>
#include "logger/Logger.h"
#include "Application.h"

namespace Engine {
    Application::Application(const ApplicationConfiguration& _config)
        : config(_config)
    {
        window.reset(new Window());
    }

    bool Application::Init() {
        Logger::Init();

        if (!window->Init(config)) {
            return false;
        }

        return true;
    }

    void Application::Run() {
        CORE_LOG_INFO("App is running: ({0}, {1}, {2})", config.width, config.height, config.title);

        OnInitClient();

        while (!window->ShouldClose()) {
            window->UpdateScreen();
            window->Swapbuffers();
            window->PollsEvent();
        }

        OnShutdownClient();
    }

    void Application::Shutdown() {
        window->Shutdown();
    }
}