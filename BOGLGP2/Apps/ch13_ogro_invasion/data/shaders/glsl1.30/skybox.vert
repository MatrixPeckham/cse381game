#version 130

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

in vec3 a_Vertex;
in vec3 a_Color;
in vec2 a_TexCoord0;

out vec2 texCoord0;
out vec4 color;

void main(void) 
{
	vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);	
	gl_Position = projection_matrix * pos;
	texCoord0 = a_TexCoord0;
	color = vec4(a_Color, 1.0);
}
