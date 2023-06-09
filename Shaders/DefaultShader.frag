#version 330 core


struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
};


struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    float innerCutOff;
    float outerCutOff;
};

#define NR_POINT_LIGHTS 1

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================

    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    result += CalcPointLight(pointLight, norm, FragPos, viewDir);
    // phase 3: spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = light.ambient* vec3(texture(texture_diffuse1, TexCoords));

    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

    return (ambient + diffuse);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1 + 0.09f * distance + 0.032f * (distance * distance));
    // combine results
    vec3 ambient = light.ambient* vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    //specular *= attenuation;
    return ambient + diffuse;
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1 + 0.09f * distance + 0.032f * (distance * distance));

    // spotlight intensity
    float theta = dot(-lightDir, normalize(light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 texColor= vec3(texture(texture_diffuse1, TexCoords));
    vec3 ambient =light.ambient*texColor;
    vec3 diffuse = light.diffuse * diff *texColor;

    ambient *=  intensity;
    diffuse *=  intensity;


    //theta=pow(theta,10);
    return ambient+diffuse;
}