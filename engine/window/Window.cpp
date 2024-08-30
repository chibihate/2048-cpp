#include"Window.h"

#include "core/Application.h"
#include "core/logger/Logger.h"

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

        glfwSetWindowUserPointer(windowGL, &windowData);

        glfwSetKeyCallback(windowGL, [](GLFWwindow* window, int keyCode, int scanCode, int action, int mods) {
            WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

            if (action == GLFW_PRESS) 
            {
                switch (keyCode)
                {
                case GLFW_KEY_UP:
                    CORE_LOG_INFO("Up is pressed");
                    break;
                case GLFW_KEY_DOWN:
                    CORE_LOG_INFO("Down is pressed");
                    break;
                case GLFW_KEY_LEFT:
                    CORE_LOG_INFO("Left is pressed");
                    break;
                case GLFW_KEY_RIGHT:
                    CORE_LOG_INFO("Right is pressed");
                    break;
                default:
                    break;
                }
                data->keyCode = keyCode;
            }
            });

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
            CORE_LOG_CRITICAL("Glad loaded failed");
            glfwTerminate();
            return false;
        }
        CORE_LOG_INFO("Glad loaded success");

        return true;
    }

    void Window::Shutdown() {
        glfwTerminate();
    }

    void Window::UpdateScreen() {
        switch (windowData.keyCode)
        {
        case GLFW_KEY_UP:
            glClearColor(1.0f, 0.3f, .8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            break;
        case GLFW_KEY_DOWN:
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            break;
        case GLFW_KEY_LEFT:
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            break;
        case GLFW_KEY_RIGHT:
            glClearColor(1.0f, 0.5f, .3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            break;
        default:
            break;
        }
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