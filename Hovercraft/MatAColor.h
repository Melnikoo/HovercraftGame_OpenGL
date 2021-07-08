#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

struct Material
{
	glm::vec4 ambRefl;
	glm::vec4 difRefl;
	glm::vec4 specRefl;
	glm::vec4 emitCols;
	float shininess;
};

struct Light
{
	glm::vec4 ambCols;
	glm::vec4 difCols;
	glm::vec4 specCols;
	glm::vec4 coords;
};