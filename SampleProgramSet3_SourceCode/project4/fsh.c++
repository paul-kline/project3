#version 410 core

in vec3 colorToFS;

uniform sampler2D textureMap;
uniform int whichTexture=-1;
out vec4 fragmentColor;
in vec2 texCoordsToFS; // (s,t)


uniform int sceneHasTranslucentObjects = 0;
uniform int drawingOpaqueObjects = 1;

uniform float currentOpacity = 1.0;
vec4 blend(){
  vec4 texColor = texture(textureMap, texCoordsToFS);
  float percentOriginalColor = 0.5;
  if(whichTexture == 2) percentOriginalColor = 0.2; //if we're painting the door, make it stronger. Otherwise it appears too dark.
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

vec4 computeColorAsUsual(){
  
	vec4 color;
	color = vec4(colorToFS, currentOpacity);	  
	if(whichTexture != -1){
	 color = blend();
	  
	}
	return color;
}

void main(){
  //drawingOpaqueObjects = 1;
    vec4 color = computeColorAsUsual();
    //fragmentColor = color;
    if (sceneHasTranslucentObjects == 1 && whichTexture == -1)
    {
        if (drawingOpaqueObjects == 1){
            if (color.a < 1.0)
                discard;
            else
                fragmentColor = color;
	}else if (color.a < 1){
            fragmentColor = color;
	}else{
            discard;
	  //fragmentColor= color;
	}
    }else{
        fragmentColor = color;
    }
   // if(whichTexture != -1) fragmentColor = color;

  
}


