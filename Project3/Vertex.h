#ifndef VERTEX_H
#define VERTEX_H

// OpenGL Mathematics Library
#include <glm.hpp>
#include <mat4x4.hpp> // glm::mat4
#include <gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <gtc/type_ptr.hpp>

// Load images & textures
#include <SOIL2.h>

// classes
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

// Define a vertex structure for OpenGL rendering
struct Vertex {
    glm::vec3 position;   // Vertex position in 3D space
    glm::vec3 color;      // Vertex color (RGB)
    glm::vec2 texcoord;   // Texture coordinates (UV)
    glm::vec3 normal;     // Vertex normal vector (perpendicular vector)
};

#endif // VERTEX_H
