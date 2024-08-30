#include <core/Entry.h>
#include <core/logger/Logger.h>
#include <window/Window.h>

class Game : public Engine::Application {
public:
    Game(const Engine::ApplicationConfiguration& config) : Engine::Application(config) {
    }

    virtual void OnInitClient() override {
        LOG_INFO("Game is init");
    }

    virtual void OnShutdownClient() override {
        LOG_INFO("Game is shutdown");
    }
};

Engine::Application* Engine::CreateApplication() {
    Engine::ApplicationConfiguration appConfig;
    appConfig.width = 400;
    appConfig.height = 400;
    appConfig.title = "2048";

    return new Game(appConfig);
}