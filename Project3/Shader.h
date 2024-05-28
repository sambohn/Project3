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


class Shader {
	
private:
	// Member variables
	GLuint id;

	// Private functions
	std::string loadShaderSource(char* fileName) {

		
	}

	GLuint loadShader(GLenum type, char* fileName) {

	}

	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {

	}

public:

	// Constructors & destructors
	Shader(char* vertexFile, char* fragmentFile, char* geometryFile = NULL) {


	}

	~Shader() {

		glDeleteProgram(this->id);
	}

	// Set uniform functions

};