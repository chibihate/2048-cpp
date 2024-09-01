#include"Window.h"

#include "core/Application.h"
#include "core/logger/Logger.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Engine {
    Window::Window()
    : windowGL(nullptr)
    {
    }

    Window::~Window()
    {
    }

    bool Window::Init(const ApplicationConfiguration& config) {
        if (!glfwInit()) {
            CORE_LOG_CRITICAL("GLFW init failed");
            return false;
        }

        // Setup context for Opengl 4.3 Core
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        windowGL = glfwCreateWindow(config.width, config.height, config.title, nullptr, nullptr);
        if (!windowGL) {
            CORE_LOG_CRITICAL("Window created failed");
            glfwTerminate();
            return false;
        }
        CORE_LOG_INFO("Window created success");

        glfwMakeContextCurrent(windowGL);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();

        /* imgui:  Setup Platform/Renderer backends */
        ImGui_ImplGlfw_InitForOpenGL(windowGL, true);
        ImGui_ImplOpenGL3_Init("#version 130");

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
            CORE_LOG_CRITICAL("Glad loaded failed");
            glfwTerminate();
            return false;
        }
        CORE_LOG_INFO("Glad loaded success");

        return true;
    }

    void Window::Shutdown() {
        /*** Finalize ***/
        /* imgui: cleanup */
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        /* Close OpenGL window and terminate GLFW */
        glfwDestroyWindow(windowGL);
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

        /* imgui: state */
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(1.0f, 0.3f, .8f, 1.0f);

        /* imgui:  Start the Dear ImGui frame */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (show_demo_window)
        // ImGui::ShowDemoWindow(&show_demo_window);
        ImGui::ShowUserGuide();

        // // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        // {
        //     static float f = 0.0f;
        //     static int counter = 0;

            // ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //     ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //     ImGui::Checkbox("Another Window", &show_another_window);

        //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //     ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //         counter++;
        //     ImGui::SameLine();
        //     ImGui::Text("counter = %d", counter);

        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //     ImGui::End();
        // }

        /* Clear the screen */
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
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
}