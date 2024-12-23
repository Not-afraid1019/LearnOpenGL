// Not_afraid
// 2024/11/25

#include "cameraControl.h"
#include <iostream>

CameraControl::CameraControl() {

}

CameraControl::~CameraControl() {

}

void CameraControl::OnMouse(int button, int action, int xpos, int ypos) {
    // 判断当前的按键,是否按下
    bool pressed = action == GLFW_PRESS;

    // 如果按下,记录当前按下的位置
    if (pressed) {
        mCurrentX = xpos;
        mCurrentY = ypos;
    }

    // 根据按下的鼠标按键不同,更改按键状态
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mLeftMouseDown = pressed;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mRightMouseDown = pressed;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mMiddleMouseDown = pressed;
            break;
    }
}

void CameraControl::OnCursor(double xpos, double ypos) {

}

void CameraControl::OnKey(int key, int action, int mods) {
    // 过滤掉repeat的情况
    if (action == GLFW_REPEAT) {
        return;
    }

    // 检测按下或者抬起,给到一个变量
    bool pressed = action == GLFW_PRESS;
    // 记录到keyMap
    mKeyMap[key] = pressed;
}

void CameraControl::update() {
}

void CameraControl::OnScroll(double offset) {

}