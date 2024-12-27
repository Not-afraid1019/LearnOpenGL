// Not_afraid
// 2024/12/24

#include "renderer.h"
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"
#include "../../wrapper/checkError.h"
#include <iostream>

Renderer::Renderer() {
    mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
    mWhiteShader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag");
}

Renderer::~Renderer() {
    delete mPhongShader;
    delete mWhiteShader;
}

void Renderer::setClearColor(glm::vec3 color) {
    glClearColor(color.r, color.g, color.b, 1.0);
}

void
Renderer::render(Scene* scene, Camera *camera, DirectionalLight* dirLight, const std::vector<PointLight*>& pointLights, SpotLight *spotLight, AmbientLight *ambLight) {
    // 1 设置当前帧绘制的时候，opengl的必要状态机参数
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // 2 清理画布
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    renderObject(scene, camera, dirLight, pointLights, spotLight, ambLight);
}

void Renderer::renderObject(Object *object, Camera *camera, DirectionalLight* dirLight, const std::vector<PointLight*>& pointLights, SpotLight *spotLight, AmbientLight *ambLight) {
    if (object->getType() == ObjectType::Mesh) {
        auto mesh = (Mesh*)object;
        auto geometry = mesh->mGeometry;
        auto material  = mesh->mMaterial;

        // 1 决定使用哪个Shader
        Shader* shader = pickShader(material->mType);
        if (!shader) {
            std::cout << "no shader picked!" << std::endl;
            return;
        }

        // 2 更新shader的uniform
        shader->start();

        switch (material->mType) {
            case MaterialType::PhongMaterial: {
                auto* phongMat = (PhongMaterial*)material;
                // diffuse贴图
                shader->setInt("sampler", 0);
                // 将纹理采样器与纹理单元进行挂钩
                phongMat->mDiffuse->bind();
                shader->setInt("specularMaskSampler", 1);
//                phongMat->mSpecularMask->bind();
                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());
                auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
                shader->setMatrix3x3("normalMatrix", normalMatrix);
                // 光源参数的uniform更新
                // spotLight的更新
                shader->setVector3("spotLight.position", spotLight->getPosition());
                shader->setVector3("spotLight.color", spotLight->mColor);
                shader->setFloat("spotLight.specularIntensity", spotLight->mSpecularIntensity);
                shader->setVector3("spotLight.targetDirection", spotLight->mTargetDirection);
                shader->setFloat("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
                shader->setFloat("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));

                // directionalLight的更新
                shader->setVector3("directionalLight.color", dirLight->mColor);
                shader->setVector3("directionalLight.direction", dirLight->mDirection);
                shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

                // pointLight的更新
                for (int j = 0; j < pointLights.size(); ++j) {
                    auto pointLight = pointLights[j];
                    std::string baseName = "pointLights[";
                    baseName.append(std::to_string(j));
                    baseName.append("]");
                    shader->setVector3(baseName + ".color", pointLight->mColor);
                    shader->setVector3(baseName + ".position", pointLight->getPosition());
                    shader->setFloat(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
                    shader->setFloat(baseName + ".k2", pointLight->mK2);
                    shader->setFloat(baseName + ".k1", pointLight->mK1);
                    shader->setFloat(baseName + ".kc", pointLight->mKc);
                }

                shader->setFloat("shiness", phongMat->mShiness);

                shader->setVector3("ambientColor", ambLight->mColor);
                shader->setVector3("cameraPosition", camera->mPosition);
            }
                break;
            case MaterialType::WhiteMaterial: {
                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());
            }
                break;
            default:
                break;
        }

        // 3 绑定vao
        glBindVertexArray(geometry->getVao());

        // 4 执行绘制命令
        glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
    }

    // 2 遍历object的子节点，对每个子节点都需要调用renderObject
    auto children = object->getChildren();
    for (int i = 0; i < children.size(); ++i) {
        renderObject(children[i], camera, dirLight, pointLights, spotLight, ambLight);
    }
}


Shader *Renderer::pickShader(MaterialType type) {
    Shader* result = nullptr;

    switch (type) {
        case MaterialType::PhongMaterial:
            result = mPhongShader;
            break;
        case MaterialType::WhiteMaterial:
            result = mWhiteShader;
            break;
        default:
            std::cout << "unknown material type" << std::endl;
            break;
    }

    return result;
}

void Renderer::render(Scene* scene, Camera *camera, DirectionalLight *dirLight,
                      AmbientLight *ambLight) {
    // 1 设置当前帧绘制的时候，opengl的必要状态机参数
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // 2 清理画布
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 3 将scene当做根节点开始递归渲染
    renderObject(scene, camera, dirLight, ambLight);
}

void Renderer::renderObject(Object *object, Camera *camera, DirectionalLight *dirLight, AmbientLight *ambLight) {
    // 1 判断是Mesh还是Object，如果是Mesh才需要渲染
    if (object->getType() == ObjectType::Mesh) {
        auto mesh = (Mesh*)object;
        auto geometry = mesh->mGeometry;
        auto material  = mesh->mMaterial;

        // 1 决定使用哪个Shader
        Shader* shader = pickShader(material->mType);
        if (!shader) {
            std::cout << "no shader picked!" << std::endl;
            return;
        }

        // 2 更新shader的uniform
        shader->start();

        switch (material->mType) {
            case MaterialType::PhongMaterial: {
                auto* phongMat = (PhongMaterial*)material;
                // diffuse贴图
                shader->setInt("sampler", 0);
                // 将纹理采样器与纹理单元进行挂钩
                phongMat->mDiffuse->bind();
                shader->setInt("specularMaskSampler", 1);
                phongMat->mSpecularMask->bind();
                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());
                auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
                shader->setMatrix3x3("normalMatrix", normalMatrix);
                // 光源参数
                shader->setVector3("directionalLight.color", dirLight->mColor);
                shader->setVector3("directionalLight.direction", dirLight->mDirection);
                shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
                shader->setFloat("shiness", phongMat->mShiness);

                shader->setVector3("ambientColor", ambLight->mColor);
                shader->setVector3("cameraPosition", camera->mPosition);
            }
                break;
            case MaterialType::WhiteMaterial: {
                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());
            }
                break;
            default:
                break;
        }

        // 3 绑定vao
        glBindVertexArray(geometry->getVao());

        // 4 执行绘制命令
        glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
    }

    // 2 遍历object的子节点，对每个子节点都需要调用renderObject
    auto children = object->getChildren();
    for (int i = 0; i < children.size(); ++i) {
        renderObject(children[i], camera, dirLight, ambLight);
    }
}


/*
void
Renderer::render(const std::vector<Mesh *> &meshes, Camera *camera, PointLight *pointLight, AmbientLight *ambLight) {
    // 1 设置当前帧绘制的时候，opengl的必要状态机参数
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // 2 清理画布
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 3 遍历mesh进行绘制
    for (int i = 0; i < meshes.size(); ++i) {
        auto mesh = meshes[i];
        auto geometry = mesh->mGeometry;
        auto material  = mesh->mMaterial;

        // 1 决定使用哪个Shader
        Shader* shader = pickShader(material->mType);
        if (!shader) {
            std::cout << "no shader picked!" << std::endl;
            return;
        }

        // 2 更新shader的uniform
        shader->start();

        switch (material->mType) {
            case MaterialType::PhongMaterial: {
                auto* phongMat = (PhongMaterial*)material;
                // diffuse贴图
                shader->setInt("sampler", 0);
                // 将纹理采样器与纹理单元进行挂钩
                phongMat->mDiffuse->bind();
                shader->setInt("specularMaskSampler", 1);
                phongMat->mSpecularMask->bind();
                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());
                auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
                shader->setMatrix3x3("normalMatrix", normalMatrix);
                // 光源参数
                shader->setVector3("lightPosition", pointLight->getPosition());
                shader->setVector3("lightColor", pointLight->mColor);
                shader->setFloat("specularIntensity", pointLight->mSpecularIntensity);
                shader->setFloat("k1", pointLight->mK1);
                shader->setFloat("k2", pointLight->mK2);
                shader->setFloat("kc", pointLight->mKc);


                shader->setFloat("shiness", phongMat->mShiness);

                shader->setVector3("ambientColor", ambLight->mColor);
                shader->setVector3("cameraPosition", camera->mPosition);
            }
                break;
            case MaterialType::WhiteMaterial: {
                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());
            }
                break;
            default:
                continue;
        }

        // 3 绑定vao
        glBindVertexArray(geometry->getVao());

        // 4 执行绘制命令
        glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
    }
}
*/