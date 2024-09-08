#include"Window.h"

#include "Application.h"
#include "Logger.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

Window::Window()
: windowGL(nullptr)
{
}

Window::~Window()
{
}

bool Window::Init(const ApplicationConfiguration& config) {
    if (!glfwInit()) {
        LOG_CRITICAL("GLFW init failed");
        return false;
    }

    // Setup context for Opengl 4.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    windowGL = glfwCreateWindow(config.width, config.height, config.title, nullptr, nullptr);
    if (!windowGL) {
        LOG_CRITICAL("Window created failed");
        glfwTerminate();
        return false;
    }
    LOG_INFO("Window created success");

    glfwMakeContextCurrent(windowGL);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(windowGL, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        LOG_CRITICAL("Glad loaded failed");
        glfwTerminate();
        return false;
    }
    LOG_INFO("Glad loaded success");

    return true;
}

void Window::Shutdown() {
    LOG_INFO("Window closed");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Window::DearImGUI() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    HandleScreen();

    // Rendering
    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::Swapbuffers() {
    glfwSwapBuffers(windowGL);
}

void Window::PollsEvent() {
    glfwPollEvents();
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(windowGL);
}

void WindowServer::InitGRPC()
{
    LOG_INFO("Init gRPC server");
    _builder.AddListeningPort("0.0.0.0:9999", grpc::InsecureServerCredentials());
    _builder.RegisterService(&_scoreServer);
    _server = _builder.BuildAndStart();
}

void WindowServer::HandleScreen()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("##2048", NULL, window_flags);

    if (!_scoreServer.GetScore().empty())
    {
        auto memberMap = _scoreServer.GetScore();
        // Create a vector of pairs to store the map's elements
        std::vector<std::pair<std::string, int>> memberVec(memberMap.begin(), memberMap.end());

        // Sort the vector by value in descending order
        std::sort(memberVec.begin(), memberVec.end(),
            [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                return a.second > b.second; // Sort by value (second), descending order
        });

        if (ImGui::BeginTable("Rank",2))
        {
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Score");
            ImGui::TableHeadersRow();
            for (const auto& member: memberVec)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                auto name = member.first.c_str();
                ImGui::Text(name);
                ImGui::TableNextColumn();
                auto score = std::to_string(member.second).c_str();
                ImGui::Text(score);
            }
            ImGui::EndTable();
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        glfwSetWindowShouldClose(windowGL, true);
    }
    ImGui::End();
}

WindowClient::WindowClient()
    : _client(grpc::CreateChannel("0.0.0.0:9999", grpc::InsecureChannelCredentials()))
    , _screenPage()
    , _username()
    , _password()
{
}

void WindowClient::InitGRPC()
{
}

void WindowClient::HandleScreen()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("##2048", NULL, window_flags);

    switch (_screenPage)
    {
    case LOGIN:
        LoginPage();
        break;
    case GAME:
        GamePage();
        break;
    default:
        break;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        glfwSetWindowShouldClose(windowGL, true);
    }
    ImGui::End();
}

void WindowClient::LoginPage()
{
    auto windowWidth = ImGui::GetWindowSize().x;
    auto windowHeight = ImGui::GetWindowSize().y;
    const uint32_t TEXT_WIDTH = 200;

    ImGui::SetNextItemWidth(TEXT_WIDTH);
    ImGui::SetCursorPosX((windowWidth - TEXT_WIDTH) * 0.5f);
    ImGui::SetCursorPosY((windowHeight - 100) * 0.5f);
    ImGui::InputTextWithHint("##username", "Username", _username, IM_ARRAYSIZE(_username));

    ImGui::SetNextItemWidth(TEXT_WIDTH);
    ImGui::SetCursorPosX((windowWidth - TEXT_WIDTH) * 0.5f);
    ImGui::InputTextWithHint("##password", "Password", _password, IM_ARRAYSIZE(_password), ImGuiInputTextFlags_Password);

    ImVec2 button_size = ImVec2(50, 20);
    ImGui::SetCursorPosX((windowWidth - 50) * 0.5f);
    if (ImGui::Button("Login", button_size))
    {
        // if ((strcmp(username, "1")==0) && (strcmp(password, "1")==0))
        // {
            LOG_INFO("Success");
            _screenPage = GAME;
        // }
    }
}

void WindowClient::GamePage()
{
    static int counter = 0;
    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, false)
        || ImGui::IsKeyPressed(ImGuiKey_RightArrow, false)
        || ImGui::IsKeyPressed(ImGuiKey_UpArrow, false)
        || ImGui::IsKeyPressed(ImGuiKey_DownArrow, false))
    {
        counter = _client.UpdateScore(_username, _password,ImGuiKey_LeftArrow);
    }
    ImGui::Text("counter = %d", counter);
}