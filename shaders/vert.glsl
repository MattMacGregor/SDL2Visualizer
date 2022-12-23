#version 330 core

layout(location=0)in vec3 position; 
layout(location=1)in vec3 normals; // Our second attribute - normals.
layout(location=2)in vec2 texCoord; // Our third attribute - texture coordinates.
layout(location=3)in vec3 tangents; // Our third attribute - texture coordinates.
layout(location=4)in vec3 bitangents; // Our third attribute - texture coordinates.

out vec3 FragPos;
out vec2 TexCoord;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{

	//gl_Position = projection * view * model * vec4(position, 1.0f);
    gl_Position = model * vec4(position, 1.0f);
  	// Store the texture coordinaets which we will output to
  	// the next stage in the graphics pipeline.
    //TexCoord = texCoord;
    FragPos = vec3(model * vec4(position, 1.0f)); 

    vec3 T = normalize(vec3(model * vec4(tangents, 0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangents, 0.0)));
    vec3 N = normalize(vec3(model * vec4(normals, 0.0)));
    mat3 TBN = transpose(mat3(T, B, N));

    TangentLightPos = TBN * lightPos;
    TangentLightPos = TBN * vec3(0.0, 0.0, 0.0);
    TangentViewPos = TBN * vec3(0.0, 0.0, 0.0);
    TangentFragPos = TBN * FragPos;
}
