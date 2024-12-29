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
#include "glframework/material/depthMaterial.h"

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

// �ƹ���
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
    camera = new PerspectiveCamera(90.0f,
                                   (float)glApp->getWidth() / (float)glApp->getHeight(),
                                   0.1f,
                                   1000.0f);

    auto control = new GameCameraControl();
    control->setSpeed(0.005f);
//    control->setSpeed(1.0f);
    cameraControl = control;
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.4f);
}

void prepare() {
    renderer = new Renderer();

    scene = new Scene();

    // ------------ A�����ʵ������ ---------------
    // 1 ����һ����ͨ����
    auto geometry = Geometry::createBox(4);
    auto materialA = new PhongMaterial();
    materialA->mDiffuse = Texture::createTexture("assets/textures/goku.jpg", 0);

    materialA->mStencilTest = true;
    materialA->mSFail = GL_KEEP;
    materialA->mZFail = GL_KEEP;
    materialA->mZPass = GL_REPLACE;
    materialA->mStencilMask = 0xFF;
    materialA->mStencilFunc = GL_ALWAYS;
    materialA->mStencilRef = 1;
    materialA->mStencilFuncMask = 0xFF;

    auto meshA = new Mesh(geometry, materialA);
    scene->addChild(meshA);

    // 2 ����һ�����߷���
    auto materialABound = new WhiteMaterial();
    materialABound->mDepthWrite = false;

    materialABound->mStencilTest = true;
    materialABound->mSFail = GL_KEEP;
    materialABound->mZFail = GL_KEEP;
    materialABound->mZPass = GL_KEEP;
    materialABound->mStencilMask = 0x00;
    materialABound->mStencilFunc = GL_NOTEQUAL;
    materialABound->mStencilRef = 1;
    materialABound->mStencilFuncMask = 0xFF;

    auto meshABound = new Mesh(geometry, materialABound);
    meshABound->setPosition(meshA->getPosition());
    meshABound->setScale(glm::vec3(1.2f));
    scene->addChild(meshABound);

    // ------------ B�����ʵ������ ---------------
    // 1 ����һ����ͨ����
    auto geometryB = Geometry::createBox(4);
    auto materialB = new PhongMaterial();
    materialB->mDiffuse = Texture::createTexture("assets/textures/wall.jpg", 0);

    materialB->mStencilTest = true;
    materialB->mSFail = GL_KEEP;
    materialB->mZFail = GL_KEEP;
    materialB->mZPass = GL_REPLACE;
    materialB->mStencilMask = 0xFF;
    materialB->mStencilFunc = GL_ALWAYS;
    materialB->mStencilRef = 1;
    materialB->mStencilFuncMask = 0xFF;

    auto meshB = new Mesh(geometryB, materialB);
    meshB->setPosition(glm::vec3(3.0f, 1.0f, 1.0f));
    scene->addChild(meshB);

    // 2 ����һ�����߷���
    auto materialBBound = new WhiteMaterial();
    materialBBound->mDepthTest = false;

    materialBBound->mStencilTest = true;
    materialBBound->mSFail = GL_KEEP;
    materialBBound->mZFail = GL_KEEP;
    materialBBound->mZPass = GL_KEEP;
    materialBBound->mStencilMask = 0x00;
    materialBBound->mStencilFunc = GL_NOTEQUAL;
    materialBBound->mStencilRef = 1;
    materialBBound->mStencilFuncMask = 0xFF;

    auto meshBBound = new Mesh(geometryB, materialBBound);
    meshBBound->setPosition(meshB->getPosition());
    meshBBound->setScale(glm::vec3(1.2f));
    scene->addChild(meshBBound);


    dirLight = new DirectionalLight();
    dirLight->mDirection = glm::vec3(1.0f);
    dirLight->mSpecularIntensity = 0.1f;

    ambLight = new AmbientLight();
    ambLight->mColor = glm::vec3(1.0f);
}

void initIMGUI() {
    ImGui::CreateContext();     // ����imgui������
    ImGui::StyleColorsDark();   // ѡ��һ������

    // ����ImGui��GLFW��OpenGL�İ�
    ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void renderIMGUI() {
    // 1 ������ǰ��IMGUI��Ⱦ
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 2 ������ǰ��GUI��������Щ�ؼ������ϵ���
    ImGui::Begin("Hello World!");
    ImGui::Text("ChangeColor Demo");
    ImGui::Button("Test Button", ImVec2(40, 20));
    ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
    ImGui::End();

    // 3 ִ��UI��Ⱦ
    ImGui::Render();
    // ��ȡ��ǰ����Ŀ��
    int display_w, display_h;
    glfwGetFramebufferSize(glApp->getWindow(), &display_w, &display_h);
    // �����ӿڴ�С
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

    // ����opengl�ӿ��Լ�������ɫ
    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    prepareCamera();
    prepare();
    initIMGUI();

    while (glApp->update()) {
        cameraControl->update();
        renderer->setClearColor(clearColor);
        renderer->render(scene, camera, dirLight, ambLight);
        renderIMGUI();
    }
    // �˳�����ǰ���������
    glApp->destroy();
    return 0;
}