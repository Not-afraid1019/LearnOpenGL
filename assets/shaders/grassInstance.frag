#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
in vec2 worldXZ;

uniform sampler2D sampler;    // diffuse贴图采样器
uniform sampler2D opacityMask; // 透明蒙版
uniform sampler2D cloudMask; // 透明蒙版
uniform float cloudSpeed;
uniform float cloudLerp;

uniform float time;
uniform vec3 windDirection;

uniform vec3 ambientColor;

// 相机世界位置
uniform vec3 cameraPosition;

uniform float shiness;

uniform float opacity;

uniform float uvScale;
uniform float brightness;

uniform vec3 cloudWhiteColor;
uniform vec3 cloudBlackColor;
uniform float cloudUVScale;

struct DirectionalLight {
   vec3 direction;
   vec3 color;
   float specularIntensity;
};

uniform DirectionalLight directionalLight;

// 计算漫反射光照
vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal) {
   float diffuse = clamp(dot(-lightDir, normal), 0.0, 1.0);
   vec3 diffuseColor = lightColor * diffuse * objectColor;

   return diffuseColor;
}

vec3 calculateDirectionalLight(vec3 objectColor, DirectionalLight light, vec3 normal, vec3 viewDir) {
   // 计算光照的通用数据
   vec3 lightDir = normalize(light.direction);

   // 1 计算diffuse
   vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

   return diffuseColor;
}

void main()
{
   vec2 worldUV = worldXZ / uvScale;
   vec3 objectColor = texture(sampler, worldUV).xyz * brightness;
   vec3 result = vec3(0.0f, 0.0f, 0.0f);
   // 计算光照的通用数据
   vec3 normalN = normalize(normal);
   vec3 viewDir = normalize(worldPosition - cameraPosition);

   result += calculateDirectionalLight(objectColor, directionalLight, normalN, viewDir);

   float alpha = texture(opacityMask, uv).r;

   vec3 ambientColor = objectColor * ambientColor;

   vec3 grassColor = result + ambientColor;

   vec3 windDirN = normalize(windDirection);
   vec2 cloudUV = worldXZ / cloudUVScale;
   cloudUV = cloudUV + time * cloudSpeed * windDirN.xz;
   float cloudMask = texture(cloudMask, cloudUV).r;
   vec3 cloudColor = mix(cloudBlackColor, cloudWhiteColor, cloudMask);

   vec3 finalColor = mix(grassColor, cloudColor, cloudLerp);

   FragColor = vec4(finalColor, alpha * opacity);
}