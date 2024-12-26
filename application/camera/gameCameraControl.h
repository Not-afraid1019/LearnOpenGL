// Not_afraid
// 2024/12/23

#ifndef LEARNOPENGL_GAMECAMERACONTROL_H
#define LEARNOPENGL_GAMECAMERACONTROL_H

#include "../../glframework/core.h"
#include "cameraControl.h"

class GameCameraControl : public CameraControl {
public:
    GameCameraControl();
    ~GameCameraControl();

    virtual void OnCursor(double xpos, double ypos) override;
    virtual void update() override;

    void setSpeed(float s) { mSpeed = s; }
private:
    void pitch(float angle);
    void yaw(float angle);

private:
    float mPitch{0.0f};
    float mSpeed{0.005f};
};


#endif //LEARNOPENGL_GAMECAMERACONTROL_H
