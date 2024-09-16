#include "Logger.h"
#include "Application.h"

const float TARGET_FPS = 60.0f;
const std::chrono::duration<float> TARGET_FRAMETIME(1.0f / TARGET_FPS);

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
        auto frameStart = std::chrono::high_resolution_clock::now();
        _window->PollsEvent();
        _window->DearImGUI();
        _window->Swapbuffers();
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEnd - frameStart;
    
        while (frameDuration < TARGET_FRAMETIME)
        {
            frameEnd = std::chrono::high_resolution_clock::now();
            frameDuration = frameEnd - frameStart;
        }
    }
}

void Application::Shutdown() {
    LOG_INFO("App is closed");
    _window->Shutdown();
}