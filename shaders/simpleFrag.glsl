// ==================================================================
#version 330 core

out vec4 fragColor;

in vec2 v_texCoord;

uniform sampler2D texture_diffuse1; 

void main()
{
	vec3 color =  texture(texture_diffuse1, v_texCoord).rgb;
    
    fragColor = vec4(color, 0.7f);
}
// ==================================================================
