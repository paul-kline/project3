#version 410 core

in vec3 colorToFS;

uniform sampler2D textureMap;
uniform int whichTexture=-1;
out vec4 fragmentColor;
in vec2 texCoords; // (s,t)
void main()
{
	fragmentColor = vec4(colorToFS, 1.0);
}
