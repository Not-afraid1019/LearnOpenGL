// Not_afraid
// 2024/12/23

#include "trackBallCameraControl.h"


TrackBallCameraControl::TrackBallCameraControl() {

}

TrackBallCameraControl::~TrackBallCameraControl() {

}

void TrackBallCameraControl::OnCursor(double xpos, double ypos) {
    if (mLeftMouseDown) {
        // 调整相机的各类参数
        // 计算经线与纬线旋转的增量角度
        float deltaX = (xpos - mCurrentX) * mSensitivity;
        float deltaY = (ypos - mCurrentY) * mSensitivity;

        /// 分开计算pitch和yaw
        pitch(-deltaY);
        yaw(-deltaX);
    } else if (mMiddleMouseDown) {
        float deltaX = (xpos - mCurrentX) * mMoveSpeed;
        float deltaY = (ypos - mCurrentY) * mMoveSpeed;

        mCamera->mPosition += mCamera->mUp * deltaY;
        mCamera->mPosition -= mCamera->mRight * deltaX;
    }

    mCurrentX = xpos;
    mCurrentY = ypos;
}

void TrackBallCameraControl::pitch(float angle) {
    // 绕着mRight向量在旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
    // 影响当前相机的up向量和位置
    mCamera->mUp = mat * glm::vec4 (mCamera->mUp, 0.0f);    // vec4给到vec3，给了xyz
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void TrackBallCameraControl::yaw(float angle) {
    // 绕着世界坐标系的y轴旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
    mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void TrackBallCameraControl::OnScroll(double offset) {
    mCamera->scale(mScaleSpeed * offset);
}