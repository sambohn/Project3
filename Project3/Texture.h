#pragma once
#include <iostream>
#include <sstream>
#include <string>

// OpenGL
#include <glew.h>
#include <glfw3.h>

// Load images & textures
#include <SOIL2.h>

class Texture {

private:
	GLuint id;
	int width;
	int height;
    unsigned int type;
    GLint textureUnit;

public:
	Texture(const char* fileName, GLenum type, GLint textureUnit) { // create texture from a file

        if (this->id) {
            glDeleteTextures(1, &this->id); // If already exists, clear memory.
        }

        this->type = type;
        this->textureUnit = textureUnit;

        // TEXTURE INIT
        unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

        // Get texture ID
        glGenTextures(1, &this->id);
        glBindTexture(type, this->id);

        if (image) {
            glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(type); // makes smaller & biger versions for distance

            // Repeat texture to fill canvas
            glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT); // S=Xcoord T=Ycoord
            glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // antialiasing
            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // MAGnification, MINification (no mipmap)


        }
        else {
            std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName <<"\n";
        }

        // Texture cleanup
        glActiveTexture(0); // No active texture
        glBindTexture(type, 0); // Unbind all textures
        SOIL_free_image_data(image); // Free loaded texture from memory
	}

	~Texture() {
		glDeleteTextures(1, &this->id);
	}

    inline GLuint getID() const { return this->id; }

    void bind() {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(type, this->id);
    }

    void unbind() {

        glActiveTexture(0);
        glBindTexture(type, 0); // unbind texture
    }

    inline GLint getTextureUnit() const { return textureUnit; }

    void loadFromFile(const char* fileName) {

        if (this->id) {
            glDeleteTextures(1, &this->id); // If already exists, clear memory.
        }

        // TEXTURE INIT
        unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

        // Get texture ID
        glGenTextures(1, &this->id);
        glBindTexture(this->type, this->id);

        if (image) {
            glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(this->type); // makes smaller & biger versions for distance

            // Repeat texture to fill canvas
            glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT); // S=Xcoord T=Ycoord
            glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // antialiasing
            glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // MAGnification, MINification (no mipmap)


        }
        else {
            std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
        }

        // Texture cleanup
        glActiveTexture(0); // No active texture
        glBindTexture(type, 0); // Unbind all textures
        SOIL_free_image_data(image); // Free loaded texture from memory

    }
};