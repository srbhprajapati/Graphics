#include "PbrShader.h"


std::string PBRVertexShader = R"(
#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texCoord;


out VS_OUT
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoord;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	vs_out.fragPos = vec3(model * vec4(pos, 1.0f));
	vs_out.normal = mat3(transpose(inverse(model)) * normal;
	vs_out.texCoord = texCoord;

	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
)";


std::string PBRFragmentShader = R"(

#version 450 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} fs_in;

// material textures
uniform sampler2D albedoMap;
uniform sampler2D normalMap;   // optional
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// lights
struct Light {
    vec3 position;
    vec3 color;
};
uniform Light lights[4];
uniform int lightCount;

// camera
uniform vec3 camPos;

// -----------------------------------------------------
// Helper functions
// -----------------------------------------------------

const float PI = 3.14159265359;

// Normal Distribution Function (GGX)
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

// Geometry Function (Smith)
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel (Schlick’s approximation)
vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// -----------------------------------------------------
// Main
// -----------------------------------------------------

void main() {
    vec3 albedo     = pow(texture(albedoMap, fs_in.texCoord).rgb, vec3(2.2)); // gamma correct
    float metallic  = texture(metallicMap, fs_in.texCoord).r;
    float roughness = texture(roughnessMap, fs_in.texCoord).r;
    float ao        = texture(aoMap, fs_in.texCoord).r;

    vec3 N = normalize(fs_in.normal);
    vec3 V = normalize(camPos - fs_in.fragPos);

    // Base reflectivity
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // Final color
    vec3 Lo = vec3(0.0);

    for (int i = 0; i < lightCount; ++i) {
        // calculate per-light radiance
        vec3 L = normalize(lights[i].position - fs_in.fragPos);
        vec3 H = normalize(V + L);
        float distance    = length(lights[i].position - fs_in.fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = lights[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator    = NDF * G * F;
        float denom       = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular     = numerator / denom;

        // kS is Fresnel
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // ambient (IBL would be better, here simple AO)
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping + gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}

)";


PbrShader::PbrShader() : Shader(PBRVertexShader, PBRFragmentShader)
{
}



void PbrShader::SetDiffuseMap(GLuint texId)
{
    this->SetUniformInt("albedoMap", texId);
}


void PbrShader::SetNormalMap(GLuint texId)
{
    this->SetUniformInt("normalMap", texId);
}


void PbrShader::SetSpecularMap(GLuint texId)
{
    this->SetUniformInt("metallicMap", texId);
}


void PbrShader::SetEmissiveMap(GLuint texId)
{
    //this->SetUniformInt("albedoMap", texId);
}