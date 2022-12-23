#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_normal1; 

// constant : 1.0f
// ambientIntensity : 0.5
// specularStrength : 0.1
// linear : 0.09
// quadratic : 0.032

void main()
{
    vec3 normal = texture(texture_normal1, TexCoord).rgb;
    normal = normalize(normal * 2.0f - 1.0f);

    vec3 color =  texture(texture_diffuse1, TexCoord).rgb;
    
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float distance = length(TangentLightPos - TangentFragPos);
    float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));

    float ambient = 0.5 * attenuation;
    float diffuse = diff * attenuation;
    float specular = 0.1f * spec * attenuation;
    
    FragColor = vec4((ambient + diffuse + specular) * color, 1.0f);
}
