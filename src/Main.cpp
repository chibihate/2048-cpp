#include "Application.h"

class Game : public Application {
public:
    Game(const ApplicationConfiguration& config) : Application(config) 
    {}

};

Application* CreateApplication() {
    ApplicationConfiguration appConfig;
    appConfig.width = 1280;
    appConfig.height = 720;
    appConfig.title = "2048";

    return new Game(appConfig);
}

int main() {
    Application* application = CreateApplication();

    if (application->Init()) {
        application->Run();
    }

    application->Shutdown();
    delete application;

    return 0;
}