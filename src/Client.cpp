#include "Client.h"

Application* CreateApplication() {
    ApplicationConfiguration appConfig;
    appConfig.width = 480;
    appConfig.height = 540;
    appConfig.title = "2048";
    appConfig.window = CLIENT;

    return new Client(appConfig);
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