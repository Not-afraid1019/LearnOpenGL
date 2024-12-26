#include <iostream>
#include <vector>
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
#include "glframework/material/phongMaterial.h"
#include "glframework/material/whiteMaterial.h"

#include "glframework/mesh.h"
#include "glframework/renderer/renderer.h"
#include "glframework/light/pointLight.h"
#include "glframework/light/spotLight.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glframework/scene.h"

#include "application/assimpLoader.h"

using namespace std;

Renderer* renderer = nullptr;
Scene* scene = nullptr;

// 灯光们
DirectionalLight* dirLight = nullptr;
SpotLight* spotLight = nullptr;
std::vector<PointLight*> pointLights{};

AmbientLight* ambLight = nullptr;

PerspectiveCamera* camera = nullptr;
CameraControl* cameraControl = nullptr;

glm::vec3 clearColor{};

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
    std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
    cameraControl->OnKey(key, action, mods);
}

void OnMouse(int button, int action, int mods) {
    double x, y;
    glApp->getCursorPosition(&x, &y);
    cameraControl->OnMouse(button, action, x, y);
}

void OnCursor(double xpos, double ypos) {
    cameraControl->OnCursor(xpos, ypos);
}

void OnScroll(double offset) {
    cameraControl->OnScroll(offset);
}

void prepareCamera() {
    float size = 6.0f;
    camera = new PerspectiveCamera(60.0f,
                                   (float)glApp->getWidth() / (float)glApp->getHeight(),
                                   0.1f,
                                   100.0f);

    auto control = new GameCameraControl();
    control->setSpeed(1.0f);
    cameraControl = control;
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.4f);
}

void prepare() {
    renderer = new Renderer();

    scene = new Scene();
//    auto testModel = AssimpLoader::load("assets/fbx/Mercedes-Benz.fbx");
    auto testModel = AssimpLoader::load("assets/fbx/79059.fbx");
    testModel->setScale(glm::vec3(0.01f));
    scene->addChild(testModel);

//    auto geometry = Geometry::createBox(1.0f);
//    // 2 创建一个material并且配置参数
//    auto material = new PhongMaterial();
//    material->mShiness = 64.0f;
//    material->mDiffuse = new Texture("assets/textures/box.png", 0);
//    material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);
//
//    // 3 生成mesh
//    auto mesh = new Mesh(geometry, material);
//    // 创建白色物体
//    auto geometryWhite = Geometry::createSphere(0.1f);
//    auto materialWhite = new WhiteMaterial();
//    auto meshWhite = new Mesh(geometryWhite, materialWhite);
//    meshWhite->setPosition(glm::vec3(2.0, 0.0, 0.0));
//
//    mesh->addChild(meshWhite);
//    scene->addChild(mesh);

    spotLight = new SpotLight();
    spotLight->setPosition(glm::vec3(2.0, 0.0, 0.0));
    spotLight->mTargetDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
    spotLight->mInnerAngle = 30.0f;
    spotLight->mOuterAngle = 45.0f;

    dirLight = new DirectionalLight();
    dirLight->mDirection = glm::vec3(1.0f);

    auto pointLight1 = new PointLight();
    pointLight1->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    pointLight1->mColor = glm::vec3(1.0f, 0.0f, 0.0f);
    pointLight1->mK2 = 0.0f;
    pointLight1->mK1 = 0.0f;
    pointLight1->mKc = 1.0f;
    pointLights.push_back(pointLight1);

    auto pointLight2 = new PointLight();
    pointLight2->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
    pointLight2->mColor = glm::vec3(0.0f, 0.0f, 1.0f);
    pointLight2->mK2 = 0.0f;
    pointLight2->mK1 = 0.0f;
    pointLight2->mKc = 1.0f;
    pointLights.push_back(pointLight2);

    auto pointLight3 = new PointLight();
    pointLight3->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    pointLight3->mColor = glm::vec3(0.0f, 0.0f, 1.0f);
    pointLight3->mK2 = 0.0f;
    pointLight3->mK1 = 0.0f;
    pointLight3->mKc = 1.0f;
    pointLights.push_back(pointLight3);

    auto pointLight4 = new PointLight();
    pointLight4->setPosition(glm::vec3(0.0f, 0.0f, -1.0f));
    pointLight4->mColor = glm::vec3(0.0f, 0.0f, 1.0f);
    pointLight4->mK2 = 0.0f;
    pointLight4->mK1 = 0.0f;
    pointLight4->mKc = 1.0f;
    pointLights.push_back(pointLight4);

    ambLight = new AmbientLight();
    ambLight->mColor = glm::vec3(1.0f);
}

void initIMGUI() {
    ImGui::CreateContext();     // 创建imgui上下文
    ImGui::StyleColorsDark();   // 选定一个主题

    // 设置ImGui与GLFW和OpenGL的绑定
    ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void renderIMGUI() {
    // 1 开启当前的IMGUI渲染
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 2 决定当前的GUI上面有哪些控件，从上到下
    ImGui::Begin("Hello World!");
    ImGui::Text("ChangeColor Demo");
    ImGui::Button("Test Button", ImVec2(40, 20));
    ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
    ImGui::End();

    // 3 执行UI渲染
    ImGui::Render();
    // 获取当前窗体的宽高
    int display_w, display_h;
    glfwGetFramebufferSize(glApp->getWindow(), &display_w, &display_h);
    // 重置视口大小
    glViewport(0, 0, display_w, display_h);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {

    if (!glApp->init(800, 600)) {
        return -1;
    }

    glApp->setResizeCallback(OnResize);
    glApp->setKeyBoardCallback(OnKey);
    glApp->setMouseCallback(OnMouse);
    glApp->setCursorCallback(OnCursor);
    glApp->setScrollCallback(OnScroll);

    // 设置opengl视口以及清理颜色
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    prepareCamera();
    prepare();
    initIMGUI();

    while (glApp->update()) {
        cameraControl->update();
        renderer->setClearColor(clearColor);
        renderer->render(scene, camera, dirLight, pointLights, spotLight, ambLight);
        renderIMGUI();
    }
    // 退出程序前做相关清理
    glApp->destroy();
    return 0;
}