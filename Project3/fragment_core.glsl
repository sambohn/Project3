#version 440

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseTex;
    sampler2D specularTex;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float intensity;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float intensity;
    vec3 color;
    float cutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

// Uniforms
uniform Material material;
uniform PointLight pointLight;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform vec3 cameraPos;

// Functions
vec3 calculateAmbient(Material material) {
    return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightDir) {
    float diffuse = max(dot(normalize(vs_normal), normalize(lightDir)), 0.0);
    return material.diffuse * diffuse;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightDir, vec3 cameraPos) {
    vec3 viewDir = normalize(cameraPos - vs_position);
    vec3 reflectDir = reflect(-normalize(lightDir), normalize(vs_normal));
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    return material.specular * specular * texture(material.specularTex, vs_texcoord).rgb;
}

void main() {
    // Directional Light Calculations
    vec3 dirLightDir = normalize(-directionalLight.direction); // light direction is opposite to the light's direction vector
    vec3 dirAmbient = directionalLight.ambient * material.ambient;
    vec3 dirDiffuse = directionalLight.diffuse * calculateDiffuse(material, vs_position, vs_normal, dirLightDir);
    vec3 dirSpecular = directionalLight.specular * calculateSpecular(material, vs_position, vs_normal, dirLightDir, cameraPos);

    // Point Light Calculations
    vec3 pointAmbient = pointLight.color * material.ambient;
    vec3 pointLightDir = pointLight.position - vs_position;
    vec3 pointDiffuse = pointLight.color * calculateDiffuse(material, vs_position, vs_normal, pointLightDir);
    vec3 pointSpecular = pointLight.color * calculateSpecular(material, vs_position, vs_normal, pointLightDir, cameraPos);

    float pointDistance = length(pointLightDir);
    float pointAttenuation = 1.0 / (pointLight.constant + pointLight.linear * pointDistance + pointLight.quadratic * (pointDistance * pointDistance));
    pointAmbient *= pointAttenuation;
    pointDiffuse *= pointAttenuation;
    pointSpecular *= pointAttenuation;

    // Spotlight Calculations
    vec3 spotLightDir = normalize(spotLight.position - vs_position);
    float theta = dot(spotLightDir, normalize(-spotLight.direction));

    vec3 spotAmbient = vec3(0.0);
    vec3 spotDiffuse = vec3(0.0);
    vec3 spotSpecular = vec3(0.0);
    if (theta > spotLight.cutOff) { // Assuming cutOff is in cosine of the angle
        spotAmbient = spotLight.ambient * texture(material.diffuseTex, vs_texcoord).rgb;
        vec3 norm = normalize(vs_normal);
        float diff = max(dot(norm, spotLightDir), 0.0);
        spotDiffuse = spotLight.diffuse * diff * texture(material.diffuseTex, vs_texcoord).rgb;

        vec3 reflectDir = reflect(-spotLightDir, norm);
        float spec = pow(max(dot(normalize(cameraPos - vs_position), reflectDir), 0.0), material.shininess);
        spotSpecular = spotLight.specular * spec * texture(material.specularTex, vs_texcoord).rgb;

        float distance = length(spotLight.position - vs_position);
        float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));

        spotAmbient *= attenuation;
        spotDiffuse *= attenuation;
        spotSpecular *= attenuation;
    }

    // Combine results
    vec3 ambient = pointAmbient + dirAmbient + spotAmbient;
    vec3 diffuse = pointDiffuse + dirDiffuse + spotDiffuse;
    vec3 specular = pointSpecular + dirSpecular + spotSpecular;

    fs_color = texture(material.diffuseTex, vs_texcoord) * vec4(ambient + diffuse + specular, 1.0);
}
