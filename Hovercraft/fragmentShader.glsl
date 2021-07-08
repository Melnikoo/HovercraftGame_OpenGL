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

in vec2 texCoordsExport;
in vec3 normalExport;

struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};
uniform Light light0;

uniform vec4 globAmb;
  
struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};
uniform Material sphereFandB;

uniform sampler2D grassTex;
uniform sampler2D skyTex;
uniform sampler2D metalTex;
uniform sampler2D trackTex;
uniform uint object;

out vec4 colorsOut;

vec4 fieldTexColor, skyTexColor, hoverTexColor, trackTexColor, fansTexColor;
vec3 normal, lightDirection;
vec4 fAndBDif, lightColorOut;

void main(void)
{  
   fieldTexColor = texture(grassTex, texCoordsExport);
   skyTexColor = texture(skyTex, texCoordsExport);
   hoverTexColor = texture(metalTex, texCoordsExport);
   trackTexColor = texture(trackTex, texCoordsExport);
   fansTexColor = texture(metalTex, texCoordsExport);


   if (object == FIELD) colorsOut = fieldTexColor;
   if (object == SKY_FRONT || object == SKY_LEFT || object == SKY_RIGHT || object == SKY_BACK || object == SKY_UP ) colorsOut = skyTexColor;


   if (object == HOVER) {
	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);   
	lightColorOut = vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
	colorsOut = hoverTexColor + lightColorOut * 0;
   }

   if (object == TRACK) {
	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);   
	lightColorOut = vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
	colorsOut = trackTexColor + lightColorOut * 0;
   }

    if (object == FANS) {
	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);   
	lightColorOut = vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
	colorsOut = trackTexColor + lightColorOut * 0;
   }
    if (object == CUBE) {
	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl);   
	lightColorOut = vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
	colorsOut = lightColorOut ;
	}
}