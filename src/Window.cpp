#include "Window.h"

#include "Application.h"
#include "Logger.h"

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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        glfwSetWindowShouldClose(windowGL, true);
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    HandleScreen();

    // Rendering
    ImGui::Render();
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
