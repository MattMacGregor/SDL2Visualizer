#version 330 core

layout(location=0)in vec3 position; 
layout(location=1)in vec3 normals; // Our second attribute - normals.
layout(location=2)in vec2 texCoord; // Our third attribute - texture coordinates.
layout(location=3)in vec3 tangents; // Our third attribute - texture coordinates.
layout(location=4)in vec3 bitangents; // Our third attribute - texture coordinates.

out vec3 FragPos;
out vec3 LightPos;
out vec3 ViewPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

void main()
{

    gl_Position = projection * view * model * vec4(position, 1.0f);
    //gl_Position = model * vec4(position, 1.0f);
  	// Store the texture coordinaets which we will output to
  	// the next stage in the graphics pipeline.
    FragPos = vec3(model * vec4(position, 1.0f)); 

    ViewPos = viewPos;
    Normal = normals; 
}
