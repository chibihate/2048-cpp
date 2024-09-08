#include "Application.h"

class Server : public Application {
public:
    Server(const ApplicationConfiguration& config) 
    : Application(config) 
    {}

};

Application* CreateApplication() {
    ApplicationConfiguration appConfig;
    appConfig.width = 400;
    appConfig.height = 400;
    appConfig.title = "2048";
    appConfig.window = SERVER;

    return new Server(appConfig);
}