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
, windowData()
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
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    /* imgui:  Setup Platform/Renderer backends */
    ImGui_ImplGlfw_InitForOpenGL(windowGL, true);
    ImGui_ImplOpenGL3_Init();

    // glfwSetWindowUserPointer(windowGL, &windowData);

    // glfwSetKeyCallback(windowGL, [](GLFWwindow* window, int keyCode, int scanCode, int action, int mods) {
    //     WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

    //     if (action == GLFW_PRESS)
    //     {
    //         switch (keyCode)
    //         {
    //         case GLFW_KEY_UP:
    //             CORE_LOG_INFO("Up is pressed");
    //             break;
    //         case GLFW_KEY_DOWN:
    //             CORE_LOG_INFO("Down is pressed");
    //             break;
    //         case GLFW_KEY_LEFT:
    //             CORE_LOG_INFO("Left is pressed");
    //             break;
    //         case GLFW_KEY_RIGHT:
    //             CORE_LOG_INFO("Right is pressed");
    //             break;
    //         default:
    //             break;
    //         }
    //         data->keyCode = keyCode;
    //     }
    //     });

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        LOG_CRITICAL("Glad loaded failed");
        glfwTerminate();
        return false;
    }
    LOG_INFO("Glad loaded success");

    return true;
}

void Window::Shutdown() {
    /*** Finalize ***/
    /* imgui: cleanup */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Window::UpdateScreen() {
    // switch (windowData.keyCode)
    // {
    // case GLFW_KEY_UP:
    //     glClearColor(1.0f, 0.3f, .8f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     break;
    // case GLFW_KEY_DOWN:
    //     glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     break;
    // case GLFW_KEY_LEFT:
    //     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     break;
    // case GLFW_KEY_RIGHT:
    //     glClearColor(1.0f, 0.5f, .3f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     break;
    // default:
    //     break;
    // }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)

    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT);

    /* imgui:  Rendering */
    ImGui::Render();
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