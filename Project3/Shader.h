#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// OpenGL
#include <glew.h>
#include <glfw3.h>

// OpenGL Mathematics Library
#include <glm.hpp>
#include <vec2.hpp> // glm::vec2
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <gtc\type_ptr.hpp>


class Shader {
	
private:
	// Member variables
	GLuint id;


	// Private functions
	std::string loadShaderSource(char* fileName) {

		char infoLog[512];
		GLint success;

		// Read from file
		std::string temp = ""; // Get each line
		std::string src = ""; // Gource

		std::ifstream in_file;

		// VERTEX ---
		in_file.open(fileName); // GLSL is a shader coding language.
		if (in_file.is_open()) { // Read file
			while (std::getline(in_file, temp))
				src += temp + "\n";
		}
		else { // Failed to open
			std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE" << fileName << "\n";
		}
		in_file.close(); // close file

		return src;
	}

	GLuint loadShader(GLenum type, char* fileName) {

		char infoLog[512];
		GLint success;

		// Create vertex shader in the background
		GLuint shader = glCreateShader(type); // get & init shader ID

		// Set source for shader
		std::string str_src = this->loadShaderSource(fileName);
		const GLchar* src = str_src.c_str(); // C type string format
		glShaderSource(shader, 1, &src, NULL); // Set source to new shader
		glCompileShader(shader); // Compile shader

		// Error check compile shaders
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // get check
		if (!success) { // FAIL
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
			std::cout << infoLog << "\n"; // Print what happened
		}
	}

	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {

		char infoLog[512];
		GLint success;

		this->id = glCreateProgram();

		glAttachShader(this->id, vertexShader);

		if (geometryShader)
			glAttachShader(this->id, geometryShader);

		glAttachShader(this->id, fragmentShader);

		glLinkProgram(this->id);

		// Error check linking
		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success) { // FAIL
			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
			std::cout << infoLog << "\n"; // print what happened
		}

		glUseProgram(0); // Resets(binding) what program we are using
	}

	

public:

	// Constructors & destructors
	Shader(char* vertexFile, char* fragmentFile, char* geometryFile = NULL) {

		// 0 == empty shader
		GLuint vertexShader = 0;
		GLuint geometryShader = 0;
		GLuint fragmentShader = 0;

		vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);

		if(geometryFile != "")
			geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);

		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

		// Link program
		this->linkProgram(vertexShader, geometryShader, fragmentShader);

		// End [Once created, linked to program & are local. Free up memory.]
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
	}

	~Shader() {

		glDeleteProgram(this->id);
	}

	// Set uniform functions

};