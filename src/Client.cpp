#include "Client.h"

int main() {
    Application* application = CreateApplication();

    if (application->Init()) {
        application->Run();
    }

    application->Shutdown();
    delete application;

    return 0;
}