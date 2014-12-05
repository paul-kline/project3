#version 410 core

in vec3 colorToFS;

uniform sampler2D textureMap;
uniform int whichTexture=-1;
out vec4 fragmentColor;
in vec2 texCoordsToFS; // (s,t)

vec4 blend(){
  vec4 texColor = texture(textureMap, texCoordsToFS);
  float percentOriginalColor = 0.5;
  float percentTexture = 1 - percentOriginalColor;
  
  vec4 result;
  
  
  result.x = percentOriginalColor*colorToFS.x + percentTexture*texColor.x;
  result.y = percentOriginalColor*colorToFS.y + percentTexture*texColor.y;
  result.z = percentOriginalColor*colorToFS.z + percentTexture*texColor.z;
  
  //fix the overflow!! if any..
	float max =((result.x > result.y)? result.x : result.y);
	max = (result.z > max)? result.z : max;
	
	if(max>1){
	 result.x /= max;
	 result.y /= max;
	 result.z /= max;
	}
	
	return result;
  
}

void main()
{
  
	
	fragmentColor = vec4(colorToFS, 1.0);	  
	if(whichTexture != -1){
	 fragmentColor = blend();
	  
	}
}
