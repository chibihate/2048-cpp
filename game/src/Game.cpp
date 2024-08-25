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
	appConfig.Width = 800;
	appConfig.Height = 800;
	appConfig.Title = "Engine Alpha ver";
	appConfig.WindowSpec = Engine::EWindowPlatformSpec::GLFW;

	return new Game(appConfig);
}