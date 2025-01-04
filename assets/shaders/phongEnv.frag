#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;    // diffuse贴图采样器
uniform sampler2D specularMaskSampler; // specularMask贴图
uniform samplerCube envSampler;

uniform vec3 ambientColor;

// 相机世界位置
uniform vec3 cameraPosition;


uniform float shiness;

struct DirectionalLight {
   vec3 direction;
   vec3 color;
   float specularIntensity;
};

struct PointLight {
   vec3 position;
   vec3 color;
   float specularIntensity;

   float k2;
   float k1;
   float kc;
};

struct SpotLight {
   vec3 position;
   vec3 targetDirection;
   vec3 color;
   float outerLine;
   float innerLine;
   float specularIntensity;
};

uniform DirectionalLight directionalLight;

// 计算漫反射光照
vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal) {
   float diffuse = clamp(dot(-lightDir, normal), 0.0, 1.0);
   vec3 diffuseColor = lightColor * diffuse * objectColor;

   return diffuseColor;
}

// 计算镜面反射光照
vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity) {
   // 1 防止背面光效果
   float dotResult = dot(-lightDir, normal);
   float flag = step(0.0, dotResult);
   vec3 lightReflect = normalize((lightDir, normal));

   // 2 计算specular
   float specular = max(dot(lightReflect, -viewDir), 0.0);

   // 3 控制光斑大小
   specular = pow(specular, shiness);

   float specularMask = texture(specularMaskSampler, uv).r;

   // 4 计算最终颜色
   vec3 specularColor = lightColor * specular * flag * intensity * specularMask;

   return specularColor;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
   // 计算光照的通用数据
   vec3 objectColor = texture(sampler, uv).xyz;
   vec3 lightDir = normalize(worldPosition - light.position);
   vec3 targetDir = normalize(light.targetDirection);

   // 计算spotLight的照射范围
   float cGamma = dot(lightDir, targetDir);
   float intensity = clamp((cGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0, 1.0);

   // 1 计算diffuse
   vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

   // 2 计算specular
   vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

   return (diffuseColor + specularColor) * intensity;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
   // 计算光照的通用数据
   vec3 objectColor = texture(sampler, uv).xyz;
   vec3 lightDir = normalize(light.direction);

   // 1 计算diffuse
   vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

   // 2 计算specular
   vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

   return diffuseColor + specularColor;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir) {
   // 计算光照的通用数据
   vec3 objectColor = texture(sampler, uv).xyz;
   vec3 lightDir = normalize(worldPosition - light.position);

   // 计算衰减
   float dist = length(worldPosition - light.position);
   float attenuation = 1.0 / (light.k2 * dist * dist + light.k1 * dist + light.kc);

   // 1 计算diffuse
   vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

   // 2 计算specular
   vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

   return (diffuseColor + specularColor) * attenuation;
}

vec3 calculateEnv(vec3 normal, vec3 viewDir) {
   vec3 reflectDir = normalize(reflect(viewDir, normal));
   vec3 color = texture(envSampler, reflectDir).rgb;
   return color;
}

void main()
{
   vec3 result = vec3(0.0f, 0.0f, 0.0f);
   // 计算光照的通用数据
   vec3 objectColor = texture(sampler, uv).xyz;
   vec3 normalN = normalize(normal);

   vec3 viewDir = normalize(worldPosition - cameraPosition);

   result += calculateDirectionalLight(directionalLight, normalN, viewDir);

   // 环境光计算
   vec3 ambientColor = objectColor * calculateEnv(normal, viewDir);

   vec3 finalColor = result + ambientColor;

   //   float flag = step(800, gl_FragCoord.x);
   //   vec3 blendColor = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), flag);
   //   finalColor *= blendColor;
   //   finalColor = vec3(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);


   FragColor = vec4(finalColor, 1.0);
}