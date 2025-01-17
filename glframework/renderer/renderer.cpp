// Not_afraid
// 2024/12/24

#include "renderer.h"
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"
#include "../material/depthMaterial.h"
#include "../material/opacityMaskMaterial.h"
#include "../material/screenMaterial.h"
#include "../material/cubeMaterial.h"
#include "../material/phongEnvMaterial.h"
#include "../material/phontInstanceMaterial.h"
#include "../material/grassInstanceMaterial.h"
#include "../mesh/instanceMesh.h"
#include "../../wrapper/checkError.h"
#include <iostream>
#include <algorithm>

Renderer::Renderer() {
    mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
    mWhiteShader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag");
    mDepthShader = new Shader("assets/shaders/depth.vert", "assets/shaders/depth.frag");
    mOpacityMaskShader = new Shader("assets/shaders/phongOpacityMask.vert", "assets/shaders/phongOpacityMask.frag");
    mScreenShader = new Shader("assets/shaders/screen.vert", "assets/shaders/screen.frag");
    mCubeShader = new Shader("assets/shaders/cube.vert", "assets/shaders/cube.frag");
    mPhongEnvShader = new Shader("assets/shaders/phongEnv.vert", "assets/shaders/phongEnv.frag");
    mPhongInstanceShader = new Shader("assets/shaders/phongInstance.vert", "assets/shaders/phongInstance.frag");
    mGrassInstanceShader = new Shader("assets/shaders/grassInstance.vert", "assets/shaders/grassInstance.frag");
}

Renderer::~Renderer() {
    delete mPhongShader;
    delete mWhiteShader;
}

void Renderer::setClearColor(glm::vec3 color) {
    glClearColor(color.r, color.g, color.b, 1.0);
}

void Renderer::projectObject(Object *obj) {
    if (obj->getType() == ObjectType::Mesh || obj->getType() == ObjectType::InstanceMesh) {
        Mesh* mesh = (Mesh*)obj;
        auto material = mesh->mMaterial;
        if (material->mBlend) {
            mTransparentObjects.push_back(mesh);
        } else {
            mOpacityObjects.push_back(mesh);
        }
    }

    auto children = obj->getChildren();
    for (int i = 0; i < children.size(); ++i) {
        projectObject(children[i]);
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
        case MaterialType::DepthMaterial:
            result = mDepthShader;
            break;
        case MaterialType::OpacityMaskMaterial:
            result = mOpacityMaskShader;
            break;
        case MaterialType::ScreenMaterial:
            result = mScreenShader;
            break;
        case MaterialType::CubeMaterial:
            result = mCubeShader;
            break;
        case MaterialType::PhongEnvMaterial:
            result = mPhongEnvShader;
            break;
        case MaterialType::PhongInstanceMaterial:
            result = mPhongInstanceShader;
            break;
        case MaterialType::GrassInstanceMaterial:
            result = mGrassInstanceShader;
            break;
        default:
            std::cout << "unknown material type" << std::endl;
            break;
    }

    return result;
}

void Renderer::render(Scene* scene, Camera *camera, DirectionalLight *dirLight,
                      AmbientLight *ambLight, unsigned int fbo) {
    glBindFramebuffer(GL_FRAMEBUFFER,  fbo);

    // 1 设置当前帧绘制的时候，opengl的必要状态机参数
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);

    // 开启测试、设置基本写入状态，打开模板测试写入
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilMask(0xFF); // 保证了模板缓冲可以被清理

    // 打开颜色混合
    glDisable(GL_BLEND);

    // 2 清理画布
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    mOpacityObjects.clear();
    mTransparentObjects.clear();

    projectObject(scene);

    std::sort(mTransparentObjects.begin(), mTransparentObjects.end(), [camera](const Mesh* a, const Mesh* b) {
        auto viewMatrix = camera->getViewMatrix();

        auto modelMatrixA = a->getModelMatrix();
        auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
        auto cameraPositionA = viewMatrix * worldPositionA;

        auto modelMatrixB = b->getModelMatrix();
        auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
        auto cameraPositionB = viewMatrix * worldPositionB;

        return cameraPositionA.z < cameraPositionB.z;
    });

    // 3 渲染两个队列
    for (int i = 0; i < mOpacityObjects.size(); ++i) {
        renderObject(mOpacityObjects[i], camera, dirLight, ambLight);
    }

    for (int i = 0; i < mTransparentObjects.size(); ++i) {
        renderObject(mTransparentObjects[i], camera, dirLight, ambLight);
    }
}

void Renderer::renderObject(Object *object, Camera *camera, DirectionalLight *dirLight, AmbientLight *ambLight) {
    // 1 判断是Mesh还是Object，如果是Mesh才需要渲染
    if (object->getType() == ObjectType::Mesh || object->getType() == ObjectType::InstanceMesh) {
        auto mesh = (Mesh*)object;
        auto geometry = mesh->mGeometry;
        Material* material = nullptr;

        // 考察是否拥有全局材质
        if (mGlobalMaterial != nullptr) {
            material = mGlobalMaterial;
        } else {
            material  = mesh->mMaterial;
        }

        // 设置渲染状态
        setDepthState(material);
        setPolygonOffsetState(material);
        setStencilState(material);
        setBlendState(material);
        setFaceCullingState(material);

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
                shader->setInt("sampler", 0);
                phongMat->mDiffuse->bind();

                shader->setInt("specularMaskSampler", 1);
//                phongMat->mSpecularMask->bind();

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
                shader->setFloat("directionalLight.intensity", dirLight->mIntensity);

                shader->setFloat("shiness", phongMat->mShiness);

                shader->setVector3("ambientColor", ambLight->mColor);
                shader->setVector3("cameraPosition", camera->mPosition);

                // 透明度
                shader->setFloat("opacity", material->mOpacity);
            }
                break;
            case MaterialType::WhiteMaterial: {
                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());
            }
                break;
            case MaterialType::DepthMaterial: {
                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());

                shader->setFloat("near", camera->mNear);
                shader->setFloat("far", camera->mFar);
            }
                break;
            case MaterialType::OpacityMaskMaterial: {
                auto* opacityMat = (OpacityMaskMaterial*)material;
                // diffuse贴图
                shader->setInt("sampler", 0);
                // 将纹理采样器与纹理单元进行挂钩
                opacityMat->mDiffuse->bind();
                // opacityMask的帧更新
                shader->setInt("opacityMaskSampler", 1);
                opacityMat->mOpacityMask->bind();
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
                shader->setFloat("shiness", opacityMat->mShiness);

                shader->setVector3("ambientColor", ambLight->mColor);
                shader->setVector3("cameraPosition", camera->mPosition);

                // 透明度
                shader->setFloat("opacity", material->mOpacity);
            }
                break;
            case MaterialType::ScreenMaterial: {
                auto* screenMat = (ScreenMaterial*)material;
                shader->setInt("screenTexSampler", 0);
                shader->setFloat("texWidth", 800);
                shader->setFloat("texHeight", 600);
                screenMat->mScreenTexture->bind();
            }
                break;
            case MaterialType::CubeMaterial: {
                auto cubeMat = (CubeMaterial*)material;
                mesh->setPosition(camera->mPosition);
                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());

                shader->setInt("cubeSampler", 0);
                cubeMat->mDiffuse->bind();
            }
                break;
            case MaterialType::PhongEnvMaterial: {
                auto* phongMat = (PhongEnvMaterial*)material;
                // diffuse贴图
                shader->setInt("sampler", 0);
                // 将纹理采样器与纹理单元进行挂钩
                phongMat->mDiffuse->bind();
                shader->setInt("envSampler", 1);
                phongMat->mEnv->bind();
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

                // 透明度
                shader->setFloat("opacity", material->mOpacity);
            }
                break;
            case MaterialType::PhongInstanceMaterial: {
                auto* phongMat = (PhongInstanceMaterial*)material;
                auto* im = (InstanceMesh*)mesh;
                shader->setInt("sampler", 0);
                phongMat->mDiffuse->bind();

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

                // 透明度
                shader->setFloat("opacity", material->mOpacity);
            }
                break;
            case MaterialType::GrassInstanceMaterial: {
                auto* grassMat = (GrassInstanceMaterial*)material;
                auto* im = (InstanceMesh*)mesh;

                im->sortMatrices(camera->getViewMatrix());
                im->updateMatrices();

                shader->setInt("sampler", 0);
                grassMat->mDiffuse->bind();

                shader->setInt("opacityMask", 1);
                grassMat->mOpacityMask->bind();

                // MVP
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projMatrix", camera->getProjectionMatrix());

//                auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
//                shader->setMatrix3x3("normalMatrix", normalMatrix);

                // 光源参数
                shader->setVector3("directionalLight.color", dirLight->mColor);
                shader->setVector3("directionalLight.direction", dirLight->mDirection);
                shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
                shader->setFloat("shiness", grassMat->mShiness);

                shader->setVector3("ambientColor", ambLight->mColor);
                shader->setVector3("cameraPosition", camera->mPosition);

                // 透明度
                shader->setFloat("opacity", material->mOpacity);

                // 草地贴图特性
                shader->setFloat("uvScale", grassMat->mUVScale);
                shader->setFloat("brightness", grassMat->mBrightness);
                shader->setFloat("time", glfwGetTime());
                // 风力
                shader->setFloat("windScale", grassMat->mWindScale);
                shader->setFloat("phaseScale", grassMat->mPhaseScale);
                shader->setVector3("windDirection", grassMat->mWindDirection);

                // 云层
                shader->setInt("cloudMask", 2);
                grassMat->mCloudMask->bind();
                shader->setVector3("cloudWhiteColor", grassMat->mCloudWhiteColor);
                shader->setVector3("cloudBlackColor", grassMat->mCloudBlackColor);
                shader->setFloat("cloudUVScale", grassMat->mCloudUVScale);
                shader->setFloat("cloudSpeed", grassMat->mCloudSpeed);
                shader->setFloat("cloudLerp", grassMat->mCloudLerp);
            }
                break;
            default:
                break;
        }

        // 3 绑定vao
        glBindVertexArray(geometry->getVao());

        // 4 执行绘制命令
        if (object->getType() == ObjectType::InstanceMesh) {
            auto* im = (InstanceMesh*)mesh;
            glDrawElementsInstanced(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0, im->mInstanceCount);
        } else {
            glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
        }
    }
}

void Renderer::setDepthState(Material *material) {
    // 设置渲染状态
    if (material->mDepthTest) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(material->mDepthFunc);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    if (material->mDepthWrite) {
        glDepthMask(GL_TRUE);
    } else {
        glDepthMask(GL_FALSE);
    }
}

void Renderer::setPolygonOffsetState(Material *material) {
    if (material->mPolygonOffset) {
        glEnable(material->mPolygonOffsetType);
        glPolygonOffset(material->mFactor, material->mUnit);
    } else {
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);
    }
}

void Renderer::setStencilState(Material *material) {
    if (material->mStencilTest) {
        glEnable(GL_STENCIL_TEST);
        glStencilOp(material->mSFail, material->mZFail, material->mZPass);
        glStencilMask(material->mStencilMask);
        glStencilFunc(material->mStencilFunc, material->mStencilRef, material->mStencilFuncMask);
    } else {
        glDisable(GL_STENCIL_TEST);
    }
}

void Renderer::setBlendState(Material *material) {
    if (material->mBlend) {
        glEnable(GL_BLEND);
        glBlendFunc(material->mSFactor, material->mDFactor);
    } else {
        glDisable(GL_BLEND);
    }
}

void Renderer::setFaceCullingState(Material *material) {
    if (material->mFaceCulling) {
        glEnable(GL_CULL_FACE);
        glFrontFace(material->mFrontFace);
        glCullFace(material->mCullFace);
    } else {
        glDisable(GL_CULL_FACE);
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