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
#include "glframework/material/opacityMaskMaterial.h"
#include "glframework/material/screenMaterial.h"
#include "glframework/material/cubeMaterial.h"
#include "glframework/material/phongEnvMaterial.h"

#include "glframework/mesh.h"
#include "glframework/renderer/renderer.h"
#include "glframework/light/pointLight.h"
#include "glframework/light/spotLight.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glframework/scene.h"
#include "glframework/framebuffer/framebuffer.h"

#include "application/assimpLoader.h"

using namespace std;

Renderer* renderer = nullptr;
Scene* scene = nullptr;

Framebuffer* framebuffer = nullptr;

int WIDTH = 800;
int HEIGHT = 600;
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

void setModelBlend(Object* obj, bool blend, float opacity) {
    if (obj->getType() == ObjectType::Mesh) {
        Mesh* mesh = (Mesh*)obj;
        Material* mat = mesh->mMaterial;
        mat->mBlend = blend;
        mat->mOpacity = opacity;
        mat->mDepthWrite = false;
    }

    auto children = obj->getChildren();
    for (int i = 0; i < children.size(); ++i) {
        setModelBlend(children[i], blend, opacity);
    }
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

    std::vector<std::string> paths = {
            "assets/textures/skybox/right.jpg",
            "assets/textures/skybox/left.jpg",
            "assets/textures/skybox/top.jpg",
            "assets/textures/skybox/bottom.jpg",
            "assets/textures/skybox/front.jpg",
            "assets/textures/skybox/back.jpg",
    };

    Texture* envTexBox = new Texture(paths, 0);

    auto boxGeo = Geometry::createBox(1.0);
    auto boxMat = new CubeMaterial();
    boxMat->mDiffuse = envTexBox;
    boxMat->mDepthWrite = false;
    auto boxMesh = new Mesh(boxGeo, boxMat);
    scene->addChild(boxMesh);

    Texture* envTex = new Texture(paths, 1);
    auto sphereGeo = Geometry::createSphere(4.0f);
    auto sphereMat = new PhongEnvMaterial();
    sphereMat->mDiffuse = new Texture("assets/textures/earth.png", 0);
    sphereMat->mEnv = envTex;
    auto sphereMesh = new Mesh(sphereGeo, sphereMat);
    scene->addChild(sphereMesh);


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

    if (!glApp->init(WIDTH, HEIGHT)) {
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
        // pass 01 ��box��Ⱦ��colorAttachment�ϣ��µ�fbo��
//        renderer->render(sceneOffscreen, camera, dirLight, ambLight, framebuffer->mFBO);
        // pass 02 ��colorAttachment��Ϊ������Ⱦ��������Ļ��
        renderer->render(scene, camera, dirLight, ambLight);
        renderIMGUI();
    }
    // �˳�����ǰ���������
    glApp->destroy();
    return 0;
}