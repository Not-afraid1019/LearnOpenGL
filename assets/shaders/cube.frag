#version 460 core
out vec4 FragColor;

in vec3 uvw;

//uniform sampler2D diffuse;
uniform samplerCube cubeSampler;

void main()
{
    FragColor = texture(cubeSampler, uvw);
}