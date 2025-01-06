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
#include "glframework/material/phontInstanceMaterial.h"
#include "glframework/material/grassInstanceMaterial.h"

#include "glframework/mesh/mesh.h"
#include "glframework/mesh/instanceMesh.h"
#include "glframework/renderer/renderer.h"
#include "glframework/light/pointLight.h"
#include "glframework/light/spotLight.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glframework/scene.h"
#include "glframework/framebuffer/framebuffer.h"

#include "application/assimpLoader.h"
#include "application/assimpInstanceLoader.h"

using namespace std;

Renderer* renderer = nullptr;
Scene* scene = nullptr;

Framebuffer* framebuffer = nullptr;

int WIDTH = 800;
int HEIGHT = 600;

GrassInstanceMaterial* grassMaterial = nullptr;

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
    control->setSpeed(0.02f);
//    control->setSpeed(1.0f);
    cameraControl = control;
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.4f);
}

void setInstanceMatrix(Object* obj, int index, glm::mat4 matrix) {
    if (obj->getType() == ObjectType::InstanceMesh) {
        auto* im = (InstanceMesh*)obj;
        im->mInstanceMatrices[index] = matrix;
    }

    auto children = obj->getChildren();
    for (int i = 0; i < children.size(); ++i) {
        setInstanceMatrix(children[i], index, matrix);
    }
}

void updateInstanceMatrix(Object* obj) {
    if (obj->getType() == ObjectType::InstanceMesh) {
        auto* im = (InstanceMesh*)obj;
        im->updateMatrices();
    }

    auto children = obj->getChildren();
    for (int i = 0; i < children.size(); ++i) {
        updateInstanceMatrix(children[i]);
    }
}

void setInstanceMaterial(Object* obj, Material* material) {
    if (obj->getType() == ObjectType::InstanceMesh) {
        auto* im = (InstanceMesh*)obj;
        im->mMaterial = material;
    }

    auto children = obj->getChildren();
    for (int i = 0; i < children.size(); ++i) {
        setInstanceMaterial(children[i], material);
    }
}

void prepare() {
    renderer = new Renderer();
    scene = new Scene();

    auto boxGeo = Geometry::createBox(1.0);
    auto boxMat = new CubeMaterial();
    boxMat->mDiffuse = new Texture("assets/textures/bk.png", 0);
    auto boxMesh = new Mesh(boxGeo, boxMat);
    scene->addChild(boxMesh);

    auto sphereGeo = Geometry::createSphere(4.0f);
    auto sphereMat = new PhongInstanceMaterial();
    sphereMat->mDiffuse = new Texture("assets/textures/earth.png", 0);

    int rNum = 20;
    int cNum = 20;

//    auto grassModel = AssimpInstanceLoader::load("assets/fbx/grass.obj", rNum * cNum);
    auto grassModel = AssimpInstanceLoader::load("assets/fbx/grass.obj", rNum * cNum);

    glm::mat4 translate;
    glm::mat4 rotate;
    glm::mat4 transform;

    srand(glfwGetTime());
    float radio = 0.5;
    for (int r = 0; r < rNum; ++r) {
        for (int c = 0; c < cNum; ++c) {
            translate = glm::translate(glm::mat4(1.0f), glm::vec3(radio*r, 0.0f, radio*c));
            rotate = glm::rotate(glm::radians((float)(rand() % 90)), glm::vec3(0.0, 1.0, 0.0));
            transform = translate * rotate;
            setInstanceMatrix(grassModel, r * cNum + c, transform);
        }
    }
    updateInstanceMatrix(grassModel);

    grassMaterial = new GrassInstanceMaterial();
    grassMaterial->mDiffuse = new Texture("assets/textures/grass.png", 0);
    grassMaterial->mOpacityMask = new Texture("assets/textures/grassMask.png", 1);
    grassMaterial->mCloudMask = new Texture("assets/textures/cloud.png", 2);
    grassMaterial->mBlend = true;
    grassMaterial->mDepthWrite = false;
    setInstanceMaterial(grassModel, grassMaterial);

    scene->addChild(grassModel);

    dirLight = new DirectionalLight();
    dirLight->mDirection = glm::vec3(1.0f);
    dirLight->mSpecularIntensity = 0.1f;

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
    ImGui::Begin("GrassMaterialEditor");
    ImGui::Text("GrassColor");
    ImGui::SliderFloat("UVScale", &grassMaterial->mUVScale, 0.0f, 100.0f);
    ImGui::InputFloat("Brightness", &grassMaterial->mBrightness);
    ImGui::Text("Wind");
    ImGui::InputFloat("WindScale", &grassMaterial->mWindScale);
    ImGui::InputFloat("PhaseScale", &grassMaterial->mPhaseScale);
    ImGui::ColorEdit3("WindDirection", (float*)&grassMaterial->mWindDirection);
    ImGui::Text("Cloud");
    ImGui::ColorEdit3("CloudWhiteColor", (float*)&grassMaterial->mCloudWhiteColor);
    ImGui::ColorEdit3("CloudBlackColor", (float*)&grassMaterial->mCloudBlackColor);
    ImGui::SliderFloat("CloudUVScale", &grassMaterial->mCloudUVScale, 0.0f, 100.0f);
    ImGui::InputFloat("CloudSpeed", &grassMaterial->mCloudSpeed);
    ImGui::SliderFloat("CloudLerp", &grassMaterial->mCloudLerp, 0.0f, 1.0f);
    ImGui::Text("Light");
    ImGui::InputFloat("Intensity", &dirLight->mIntensity);
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

    if (!glApp->init(WIDTH, HEIGHT)) {
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
        renderer->render(scene, camera, dirLight, ambLight);
        renderIMGUI();
    }
    // 退出程序前做相关清理
    glApp->destroy();
    return 0;
}