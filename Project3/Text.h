#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>
#include <string>

#include <glm.hpp>

#include <glew.h>

#include "Shader.h"

struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class Text {
private:
    std::map<char, Character> Characters;
    FT_Library ft_Library;
    FT_Face face;
    glm::mat4 projection;

    unsigned int VAO, VBO;

    void initFont(const char* font) {
        if (FT_Init_FreeType(&this->ft_Library)) {
            std::cerr << "ERROR::FREETYPE: Could not initialize FreeType Library" << std::endl;
            throw std::runtime_error("Failed to initialize FreeType Library");
        }
        if (FT_New_Face(ft_Library, font, 0, &this->face)) {
            std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
            throw std::runtime_error("Failed to load font");
        }
        // Set the orthographic projection matrix for 2D rendering
        projection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
    }

    void loadCharacters() {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(this->face, c, FT_LOAD_RENDER)) {
                std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
                continue;
            }
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                this->face->glyph->bitmap.width,
                this->face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                this->face->glyph->bitmap.buffer
            );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            Character character = {
                texture,
                glm::ivec2(this->face->glyph->bitmap.width, this->face->glyph->bitmap.rows),
                glm::ivec2(this->face->glyph->bitmap_left, this->face->glyph->bitmap_top),
                static_cast<unsigned int>(this->face->glyph->advance.x)
            };
            this->Characters.insert(std::pair<char, Character>(c, character));
        }

        std::cout << "FREETYPE:: Glyphs successfully loaded" << std::endl;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

public:
    Text() : VAO(0), VBO(0) {
        // Default constructor
        projection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f); // Set default projection matrix
    }

    Text(const char* font, int fontSize = 48) {
        initFont(font);
        FT_Set_Pixel_Sizes(this->face, 0, fontSize);
        loadCharacters();
    }

    Text(const char* font, int fontSize, int windowWidth, int windowHeight) {
        initFont(font);
        FT_Set_Pixel_Sizes(this->face, 0, fontSize);
        loadCharacters();
        projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight));
    }

    void Load(const char* font, int fontSize = 48) {
        if (VAO != 0) {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }
        initFont(font);
        FT_Set_Pixel_Sizes(this->face, 0, fontSize);
        loadCharacters();
    }

    ~Text() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft_Library);
        if (VAO != 0) {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }
    }

    void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color) {
        shader.use();
        glUniform3f(glGetUniformLocation(shader.getID(), "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        for (const char& c : text) {
            Character ch = Characters[c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void setProjectionMatrix(const glm::mat4& projectionMatrix) {
        this->projection = projectionMatrix;
    }

    unsigned int getVAO() const {
        return VAO;
    }
};
