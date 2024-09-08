#pragma once
#include "Service.h"

class GLFWwindow;

enum Page {
    LOGIN,
    GAME
};

class Window {
public:
    Window();
    virtual ~Window();
    bool Init(const struct ApplicationConfiguration&);
    void Shutdown();
    void Swapbuffers();
    void PollsEvent();
    bool ShouldClose();
    void DearImGUI();
    virtual void HandleScreen() {};
    virtual void InitGRPC() {};

protected:
    Window(Window&) = default;
    GLFWwindow* windowGL;
};

class WindowServer : public Window {
public:
    WindowServer() {};
    virtual ~WindowServer() {};
    virtual void InitGRPC();
    virtual void HandleScreen();
private:
    ScoreServer _scoreServer;
    grpc::ServerBuilder _builder;
    std::unique_ptr<grpc::Server> _server;
};

class WindowClient : public Window {
public:
    WindowClient();
    virtual ~WindowClient() {};
    virtual void InitGRPC();
    virtual void HandleScreen();
    void LoginPage();
    void GamePage();
private:
    ScoreClient _client;
    Page _screenPage;
    char _username[64], _password[64];
};
