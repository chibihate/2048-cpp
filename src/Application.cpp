#include <iostream>
#include "Logger.h"
#include "Application.h"

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
    LOG_INFO("App is running: ({0}, {1}, {2})", config.width, config.height, config.title);

    while (!window->ShouldClose()) {
        window->PollsEvent();
        window->UpdateScreen();
        window->Swapbuffers();
    }
}

void Application::Shutdown() {
    window->Shutdown();
}