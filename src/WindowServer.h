#pragma once
#include "Tile.h"
#include "Window.h"
#include "ServiceServer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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