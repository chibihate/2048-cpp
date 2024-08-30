#pragma once

#include<memory>
#include"window/Window.h"

namespace Engine {
    struct ApplicationConfiguration {
        int width, height;
        const char* title;
    };

    class Application {
    public:
        virtual ~Application() = default;
        bool Init();
        virtual void OnInitClient() = 0;
        void Run();
        virtual void OnShutdownClient() = 0;
        void Shutdown();
    protected:
        Application() = default;
        Application(const ApplicationConfiguration&);
    private:
        ApplicationConfiguration config;
        std::unique_ptr<Window> window;
    };

    extern Application* CreateApplication();
}