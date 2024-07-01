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
#include <gtc/type_ptr.hpp>

class Shader {
private:
    GLuint id;
    const int versionMajor;
    const int versionMinor;

    std::string loadShaderSource(const char* fileName) {
        std::string temp = "";
        std::string src = "";

        std::ifstream in_file(fileName);
        if (in_file.is_open()) {
            while (std::getline(in_file, temp))
                src += temp + "\n";
        }
        else {
            std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE " << fileName << "\n";
        }
        in_file.close();

        std::string versionNr = std::to_string(versionMajor) + std::to_string(versionMinor) + "0";
            std::cout << src.replace(src.find("#version"), 12, "#version"+versionNr) << "\n";

        return src;
    }

    GLuint loadShader(GLenum type, const char* fileName) {
        char infoLog[512];
        GLint success;

        GLuint shader = glCreateShader(type);

        std::string str_src = this->loadShaderSource(fileName);
        const GLchar* src = str_src.c_str();
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
            std::cout << infoLog << "\n";
        }

        return shader;
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

        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
            std::cout << infoLog << "\n";
        }

        glUseProgram(0);
    }

public:
    Shader(const int versionMajor, const int versionMinor,
        const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr)
        : versionMajor(versionMajor), versionMinor(versionMinor) {
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
        GLuint geometryShader = 0;
        if (geometryFile)
            geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);
        GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

        this->linkProgram(vertexShader, geometryShader, fragmentShader);

        glDeleteShader(vertexShader);
        if (geometryShader)
            glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader() {
        glDeleteProgram(this->id);
    }

    // Set uniform functions
    void use() {
        glUseProgram(this->id);
    }
    
    void unuse() {
        glUseProgram(0);
    }


    void set1i(GLint value, const GLchar* name) {

        // Use program
        this->use();

        // Set shader
        glUniform1i(glGetUniformLocation(this->id, name), value);

        // Unuse
        this->unuse(); // need to use again to start drawing
    }

    void set1f(GLfloat value, const GLchar* name) {

        // Use program
        this->use();

        // Set shader
        glUniform1f(glGetUniformLocation(this->id, name), value);

        // Unuse
        this->unuse(); // need to use again to start drawing
    }

    void setVec2f(glm::fvec2 value, const GLchar* name) {

        // Use program
        this->use();

        // Set shader
        glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

        // Unuse
        this->unuse(); // need to use again to start drawing
    }

    void setVec3f(glm::fvec3 value, const GLchar* name) {

        // Use program
        this->use();

        // Set shader
        glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

        // Unuse
        this->unuse(); // need to use again to start drawing
    }

    void setVec4f(glm::fvec4 value, const GLchar* name) {

        // Use program
        this->use();

        // Set shader
        glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

        // Unuse
        this->unuse(); // need to use again to start drawing
    }

    void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE) {

        // Use program
        this->use();

        // Set shader
        glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

        // Unuse
        this->unuse(); // need to use again to start drawing
    }

    void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE) {

        // Use program
        this->use();

        // Set shader
        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

        // Unuse
        this->unuse(); // need to use again to start drawing
    }



};
