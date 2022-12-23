// ==================================================================
#version 330 core

layout(location=0)in vec3 position; 
layout(location=1)in vec3 normals; // Our second attribute - normals.
layout(location=2)in vec2 texCoord; // Our third attribute - texture coordinates.
layout(location=3)in vec3 tangents; // Our third attribute - texture coordinates.
layout(location=4)in vec3 bitangents; // Our third attribute - texture coordinates.

out vec3 fragPos;
out vec2 v_texCoord;

uniform mat4 model;

void main()
{

	gl_Position = model * vec4(position, 1.0f);

  	// Store the texture coordinaets which we will output to
  	// the next stage in the graphics pipeline.
  	v_texCoord = texCoord;
    fragPos = vec3(model * vec4(position, 0.0f)); 
}
// ==================================================================
