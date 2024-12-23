// Not_afraid
// 2024/11/21

#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../wrapper/checkError.h"

Application* Application::mInstance = nullptr;
Application* Application::getInstance() {
    if (mInstance == nullptr) {
        mInstance = new Application();
    }

    return mInstance;
}

Application::Application() {

}

Application::~Application() {

}

bool Application::init(const int& width, const int& height) {
    mWidth = width;
    mHeight = height;
    // 初始化GLFW基本环境
    glfwInit();
    // 设置OpenGL主版本号，次版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // 设置OpenGL启用核心模式（非立即渲染模式）
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗体对象
    mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGLStudy", nullptr, nullptr);
    if (mWindow == nullptr) {
        return false;
    }
    // 设置当前窗体对象为OpenGL的绘制舞台
    glfwMakeContextCurrent(mWindow);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);

    // this就是当前全局唯一的Application对象
    glfwSetWindowUserPointer(mWindow, this);

    // 键盘响应
    glfwSetKeyCallback(mWindow, keyCallback);

    // 鼠标点击事件
    glfwSetMouseButtonCallback(mWindow, mouseCallback);

    // 鼠标移动事件
    glfwSetCursorPosCallback(mWindow, cursorCallback);

    // 鼠标滚轮消息
    glfwSetScrollCallback(mWindow, scrollCallback);

    return true;
}

bool Application::update() {
    if (glfwWindowShouldClose(mWindow)) {
        return false;
    }
    // 接收并分发窗体消息
    // 检查消息队列是否有需要处理的鼠标、键盘等消息
    // 如果有的话就将消息批量处理，清空队列
    GL_CALL(glfwPollEvents());

    // 切换双缓存
    GL_CALL(glfwSwapBuffers(mWindow));

    return true;
}

void Application::destroy() {
    // 退出程序前做相关清理
    glfwTerminate();
}

void Application::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    auto* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mResizeCallback != nullptr) {
        self->mResizeCallback(width, height);
    }
//    if (app->mResizeCallback != nullptr) {
//        app->mResizeCallback(width, height);
//    }
}

void Application::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mKeyBoardCallback != nullptr) {
        self->mKeyBoardCallback(key, action, mods);
    }
}

void Application::mouseCallback(GLFWwindow *window, int botton, int action, int mods) {
    auto* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mMouseCallback != nullptr) {
        self->mMouseCallback(botton, action, mods);
    }
}

void Application::cursorCallback(GLFWwindow *window, double xpos, double ypos) {
    auto* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mCursorCallback != nullptr) {
        self->mCursorCallback(xpos, ypos);
    }
}

void Application::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    auto* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mScrollCallback != nullptr) {
        self->mScrollCallback(yoffset);
    }
}

void Application::getCursorPosition(double* x, double* y) {
    glfwGetCursorPos(mWindow, x, y);
}