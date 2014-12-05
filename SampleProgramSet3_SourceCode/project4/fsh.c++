#version 410 core

in vec3 colorToFS;

uniform sampler2D textureMap;
uniform int whichTexture=-1;
out vec4 fragmentColor;
in vec2 texCoordsToFS; // (s,t)
void main()
{
  
	if(whichTexture != -1){
	 fragmentColor = texture(textureMap, texCoordsToFS);
	  
	}else{
	fragmentColor = vec4(colorToFS, 1.0);	  
	}
}
