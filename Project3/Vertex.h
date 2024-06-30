#pragma once

// OpenGL Mathematics Library
#include <glm.hpp>
#include <mat4x4.hpp> // glm::mat4
#include <gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <gtc\type_ptr.hpp>

// Load images & textures
#include <SOIL2.h>

// classes
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal; // perpendicular vector
};