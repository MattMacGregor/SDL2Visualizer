#version 330 core

out vec4 FragColor;

in vec3 FragPos;

in vec2 TexCoord;
in vec3 ViewPos;
in vec3 Normal;

struct PointLight{
    vec3 lightColor;
    vec3 lightPos;
    float ambientIntensity;

    float specularStrength;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLights;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{


    // Compute the normal direction
    vec3 norm = normalize(Normal);
    
    // Store our final texture color
    vec3 diffuseColor;
    diffuseColor = vec3(250 / 255.0f, 152 / 255.0f, 54 / 255.0f); //texture(u_DiffuseMap, v_texCoord).rgb;
    //vec3 viewPos = (view * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
    vec3 viewDir = normalize(ViewPos - FragPos);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < 1; i++)
        result += CalcPointLight(pointLights, Normal, FragPos, viewDir);    
    
    //FragColor = vec4(diffuseColor * result,1.0); 
    FragColor = vec4(diffuseColor * result,1.0); 
}

    //int constant = 1.0f;
    //int ambientIntensity = 0.5;
    //int specularStrength = 0.1;
    //int linear = 0.09;
    //int quadratic = 0.032;

    //vec3 color = vec3(0.2, 0.2, 0.2);

    //vec3 viewDir = normalize(ViewPos - FragPos);
    //vec3 lightDir = normalize(LightPos - FragPos);

    //float diff = max(dot(Normal, lightDir), 0.0);
    
    //vec3 reflectDir = reflect(-lightDir, Normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //float distance = length(LightPos - FragPos);
    //float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));

    //float ambient = ambientIntensity * attenuation;
    //float diffuse = diff * attenuation;
    //float specular = 0.1f * spec * attenuation;
    
    //FragColor = vec4((ambient + diffuse + specular) * color, 1.0f);
//}

//calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.lightPos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.lightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.lightColor * light.ambientIntensity;
    vec3 diffuse = light.lightColor * diff;
    vec3 specular = light.lightColor * light.specularStrength * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
