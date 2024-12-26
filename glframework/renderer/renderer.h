// Not_afraid
// 2024/12/24

#ifndef LEARNOPENGL_RENDERER_H
#define LEARNOPENGL_RENDERER_H

#include "../core.h"
#include <vector>
#include "../mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../light/ambientLight.h"
#include "../shader.h"
#include "../scene.h"

class Renderer {
public:
    Renderer();

    ~Renderer();

    // 渲染功能函数，每次调用都会渲染一帧
    void render(Scene* scene, Camera *camera, DirectionalLight *dirLight, const std::vector<PointLight*>& pointLights, SpotLight *spotLight, AmbientLight *ambLight);

//    void render(const std::vector<Mesh*>& meshes,
//                Camera* camera,
//                PointLight* pointLight,
//                AmbientLight* ambLight);

    void render(Scene* scene, Camera* camera, DirectionalLight* dirLight, AmbientLight* ambLight);

    // 针对单个object进行渲染
    void renderObject(Object* object, Camera* camera, DirectionalLight* dirLight, AmbientLight* ambLight);
    void renderObject(Object *object, Camera *camera, DirectionalLight* dirLight, const std::vector<PointLight*>& pointLights, SpotLight *spotLight, AmbientLight *ambLight);

    void setClearColor(glm::vec3 color);

private:
    // 根据Material类型不同，挑选不同的shader
    Shader *pickShader(MaterialType type);

private:
    // 生成多种不同的shader对象
    // 根据材质类型的不同，挑选使用哪一个shader对象
    Shader *mPhongShader{nullptr};
    Shader *mWhiteShader{nullptr};
};


#endif //LEARNOPENGL_RENDERER_H
