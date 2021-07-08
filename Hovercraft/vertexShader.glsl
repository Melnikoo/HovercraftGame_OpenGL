#version 430 core

#define FIELD 0
#define SKY_FRONT 1
#define HOVER 2
#define TRACK 3
#define FANS 4
#define SKY_LEFT 5
#define SKY_RIGHT 6
#define SKY_BACK 7
#define SKY_UP 8
#define CUBE 9

layout(location=0) in vec4 fieldCoords;
layout(location=1) in vec2 fieldTexCoords;


layout(location=2) in vec4 skyCoords;
layout(location=3) in vec2 skyTexCoords;

layout(location=4) in vec3 objectCoords;
layout(location=5) in vec3 objectNormals;
layout(location=6) in vec2 objectTexCoords;





uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;

out vec2 texCoordsExport;
out vec3 normalExport;
vec4 coords;

void main(void)
{   
   if (object == FIELD)
   {
      coords = fieldCoords;
      texCoordsExport = fieldTexCoords;
   }
   if (object == SKY_FRONT || object == SKY_LEFT || object == SKY_RIGHT || object == SKY_BACK || object == SKY_UP )
   {
      coords = skyCoords;
      texCoordsExport = skyTexCoords;
   }
   if (object == HOVER)
   {
      coords = vec4(objectCoords,1.0);
      normalExport = objectNormals;
      texCoordsExport = objectTexCoords;
   }
   if (object == TRACK)
   {
      coords = vec4(objectCoords,1.0);
      normalExport = objectNormals;
      texCoordsExport = objectTexCoords;
   }
   if (object == FANS)
   {
      coords = vec4(objectCoords,1.0);
      normalExport = objectNormals;
      texCoordsExport = objectTexCoords;
   }
   if (object == CUBE)
   {
      coords = vec4(objectCoords,1.0);
      normalExport = objectNormals;
	  texCoordsExport = objectTexCoords;
   }
   gl_Position = projMat * modelViewMat * coords;
}
