#include <iostream>
#include "glframework/core.h"
#include "glframework/shader.h"
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"

#include "application/camera/perspectiveCamera.h"
#include "application/camera/cameraControl.h"
#include "application/camera/trackBallCameraControl.h"
#include "application/camera/gameCameraControl.h"

#include "glframework/geometry.h"

using namespace std;

Geometry* geometry = nullptr;
Shader* shader = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0f);

PerspectiveCamera* camera = nullptr;
CameraControl* cameraControl = nullptr;

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
    cameraControl->OnKey(key, action, mods);
}

void OnMouse(int button, int action, int mods) {
    double x, y;
    app->getCursorPosition(&x, &y);
    cameraControl->OnMouse(button, action, x, y);
}

void OnCursor(double xpos, double ypos) {
    cameraControl->OnCursor(xpos, ypos);
}

void OnScroll(double offset) {
    cameraControl->OnScroll(offset);
}

void prepareVAO() {
    geometry = Geometry::createSphere(3.0f);
//    geometry = Geometry::createBox(6.0f);
}

void prepareShader() {
    shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

void prepareTexture() {
    texture = new Texture("assets/textures/earth.jpg", 0);
}

void prepareCamera() {
    float size = 6.0f;
    camera = new PerspectiveCamera(60.0f,
                                   (float)app->getWidth() / (float)app->getHeight(),
                                   0.1f,
                                   100.0f);

    cameraControl = new GameCameraControl();
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.4f);
}

void prepareState() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void render() {
    // 执行opengl画布清理操作
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 绑定当前的program
    shader->start();
    shader->setInt("sampler", 0);
    shader->setMatrix4x4("transform", transform);
    shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
    shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());

    // 绑定当前的vao
    GL_CALL(glBindVertexArray(geometry->getVao()));

    // 发出绘制指令
    GL_CALL(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));
    glBindVertexArray(0);

    shader->end();
}

int main() {

    if (!app->init(800, 600)) {
        return -1;
    }

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallback(OnKey);
    app->setMouseCallback(OnMouse);
    app->setCursorCallback(OnCursor);
    app->setScrollCallback(OnScroll);

    // 设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    prepareShader();
    prepareVAO();
    prepareTexture();
    prepareCamera();
    prepareState();

    while (app->update()) {
        cameraControl->update();
        render();
    }
    // 退出程序前做相关清理
    app->destroy();
    return 0;
}