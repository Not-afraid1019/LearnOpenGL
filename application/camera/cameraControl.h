// Not_afraid
// 2024/11/25

#ifndef LEARNOPENGL_CAMERACONTROL_H
#define LEARNOPENGL_CAMERACONTROL_H

#include "../../glframework/core.h"
#include "camera.h"
#include <map>

class CameraControl {
public:
    CameraControl();
    ~CameraControl();

    virtual void OnMouse(int button, int action, int xpos, int ypos);
    virtual void OnCursor(double xpos, double ypos);
    virtual void OnKey(int key, int action, int mods);
    virtual void OnScroll(double offset);

    // 每一帧渲染之前调用，每一帧更新的行为放在此处
    virtual void update();

    void setCamera(Camera* camera) { mCamera = camera; }
    void setSensitivity(float s) { mSensitivity = s; }
    void setScaleSpeed(float s) { mScaleSpeed = s; }

protected:
    // 鼠标按键状态
    bool mLeftMouseDown = false;
    bool mRightMouseDown = false;
    bool mMiddleMouseDown = false;
    //当前鼠标位置
    float mCurrentX = 0.0f, mCurrentY = 0.0f;
    // 敏感度
    float mSensitivity = 0.2f;
    // 记录键盘相关按键的按下状态
    std::map<int, bool> mKeyMap;
    // 存储当前控制的那一个摄像机
    Camera* mCamera = nullptr;
    // 记录相机缩放的速度
    float mScaleSpeed = 0.2f;
};


#endif //LEARNOPENGL_CAMERACONTROL_H
