#include "Server.h"

Application* CreateApplication() {
    ApplicationConfiguration appConfig;
    appConfig.width = 400;
    appConfig.height = 400;
    appConfig.title = "2048";
    appConfig.window = SERVER;

    return new Server(appConfig);
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