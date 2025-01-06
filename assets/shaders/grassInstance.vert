#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 4) in mat4 aInstanceMatrix;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
out vec2 worldXZ;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float time;
uniform float windScale;
uniform vec3 windDirection;
uniform float phaseScale;

void main()
{
    // 将输入的顶点位置，转化为齐次坐标
    vec4 transformPosition = vec4(aPos, 1.0);
    // 计算当前顶点的worldPosition，并且向后传输给FragmentShader
    transformPosition = modelMatrix * aInstanceMatrix * transformPosition;

    worldXZ = transformPosition.xz;

    // 风力变动
    vec3 windDirN = normalize(windDirection);
    float phaseDistance = dot(windDirN, transformPosition.xyz);
    transformPosition += vec4(sin(time + phaseDistance / phaseScale) * (1.0 - aColor.r) * windScale * windDirN, 0.0);

    worldPosition = transformPosition.xyz;

    gl_Position = projMatrix * viewMatrix * transformPosition;
    uv = aUv;
    normal = transpose(inverse(mat3(modelMatrix * aInstanceMatrix))) * aNormal;
//    normal = normalMatrix * aNormal;
}