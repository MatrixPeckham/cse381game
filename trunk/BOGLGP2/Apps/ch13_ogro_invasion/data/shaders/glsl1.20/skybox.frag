#version 120

precision highp float;

uniform sampler2D texture0;

in vec4 color;
in vec2 texCoord0;

out vec4 outColor;

void main(void) 
{
	//Sample the texture
	outColor = color * texture(texture0, texCoord0.st);	

	//If the alpha component is too low then discard
	if (outColor.a < 0.1) 
	{
		discard;
	}
}
