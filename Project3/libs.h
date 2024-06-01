#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// OpenGL
#include <glew.h>
#include <glfw3.h>

// OpenGL Mathematics Library
#include <glm.hpp>
#include <vec2.hpp> // glm::vec2
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <gtc\type_ptr.hpp>

// Load images & textures
#include <SOIL2.h>

// classes
#include "Shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal; // perpendicular vector
};


