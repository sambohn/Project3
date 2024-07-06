#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once
#include <glew.h>
#include <SOIL2.h>
#include <string>
#include <iostream>

class Texture {
private:
    GLuint id;
    int width;
    int height;
    unsigned int type;
    GLint textureUnit;

public:
    Texture(const char* fileName, GLenum type) { // Create texture from a file
        this->id = 0; // Initialize id to 0
        this->type = type;

        // TEXTURE INIT
        unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

        if (image) {
            glGenTextures(1, &this->id);
            glBindTexture(type, this->id);

            glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(type); // Generate mipmap for distance

            // Set texture parameters
            glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            // Texture cleanup
            glActiveTexture(0); // No active texture
            glBindTexture(type, 0); // Unbind all textures
            SOIL_free_image_data(image); // Free loaded texture from memory
        }
        else {
            std::cerr << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
        }
    }

    ~Texture() {
        if (this->id) {
            glDeleteTextures(1, &this->id); // Delete texture if it exists
        }
    }

    inline GLuint getID() const { return this->id; }

    void bind(const GLint texture_unit) {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(type, this->id);
    }

    void unbind() {
        glActiveTexture(0);
        glBindTexture(type, 0); // Unbind texture
    }

    void loadFromFile(const char* fileName) {
        if (this->id) {
            glDeleteTextures(1, &this->id); // If already exists, clear memory.
        }

        // TEXTURE INIT
        unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

        if (image) {
            glGenTextures(1, &this->id);
            glBindTexture(this->type, this->id);

            glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(this->type); // Generate mipmap for distance

            // Set texture parameters
            glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            // Texture cleanup
            glActiveTexture(0); // No active texture
            glBindTexture(type, 0); // Unbind all textures
            SOIL_free_image_data(image); // Free loaded texture from memory
        }
        else {
            std::cerr << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
        }
    }

    GLint getTextureUnit() { return this->textureUnit; }
};

#endif // TEXTURE_H
