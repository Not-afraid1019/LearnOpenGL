// Not_afraid
// 2024/12/23

#ifndef LEARNOPENGL_TRACKBALLCAMERACONTROL_H
#define LEARNOPENGL_TRACKBALLCAMERACONTROL_H

#include "cameraControl.h"

class TrackBallCameraControl : public CameraControl {
public:
    TrackBallCameraControl();
    ~TrackBallCameraControl();

    // 父类中的接口函数，是否需要重写
    virtual void OnCursor(double xpos, double ypos) override;

    virtual void OnScroll(double offset) override;
private:
    void pitch(float angle);
    void yaw(float angle);

private:
    float mMoveSpeed = 0.005f;
};


#endif //LEARNOPENGL_TRACKBALLCAMERACONTROL_H
