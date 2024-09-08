#include "Application.h"

class Client : public Application {
public:
    Client(const ApplicationConfiguration& config) 
    : Application(config) 
    {}

};

Application* CreateApplication() {
    ApplicationConfiguration appConfig;
    appConfig.width = 400;
    appConfig.height = 400;
    appConfig.title = "2048";
    appConfig.window = CLIENT;

    return new Client(appConfig);
}