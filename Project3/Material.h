#pragma once

// OpenGL
#include <glew.h>
#include <glfw3.h>

// Load images & textures
#include <SOIL2.h>

#include "Shader.h"


// OpenGL Mathematics Library
#include <glm.hpp>
#include <vec2.hpp> // glm::vec2
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <gtc\type_ptr.hpp>


class Material {

private:

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTex;
	GLint specularTex;

public:

	Material(glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	GLint diffuseTex,
	GLint specularTex) {

		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	~Material() {}

	// Function
	void sendToShader(Shader& program) {
		program.setVec3f(this->ambient, "material.ambient");
		program.setVec3f(this->diffuse, "material.diffuse");
		program.setVec3f(this->specular, "material.specular");
		program.set1i(this->diffuseTex, "material.diffuseTex");
		program.set1i(this->specularTex, "material.specularTex");
	}

};