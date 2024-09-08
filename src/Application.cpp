#include "Logger.h"
#include "Application.h"

Application::Application(const ApplicationConfiguration& config)
    : _config(config)
{
    switch (config.window)
    {
    case CLIENT:
        _window.reset(new WindowClient());
        break;
    case SERVER:
        _window.reset(new WindowServer());
        break;
    default:
        _window.reset(new Window());
        break;
    }
}

bool Application::Init() {
    Logger::Init();

    if (!_window->Init(_config)) {
        return false;
    }
    _window->InitGRPC();

    return true;
}

void Application::Run() {
    LOG_INFO("App is running: ({0}, {1}, {2})", _config.width, _config.height, _config.title);

    while (!_window->ShouldClose()) {
        _window->PollsEvent();
        _window->DearImGUI();
        _window->Swapbuffers();
    }
}

void Application::Shutdown() {
    LOG_INFO("App is closed");
    _window->Shutdown();
}