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
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};



class Text {

private:
	std::map<char, Character> Characters;

	FT_Library ft_Library;
	FT_Face face;
	glm::mat4 projection;

	unsigned int VAO, VBO;

	int initFont(const char* font) {
		if (FT_Init_FreeType(&this->ft_Library)) {
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return -1;
		}
		if (FT_New_Face(ft_Library, font, 0, &this->face)) {
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			return -1;
		}
		// Set the orthographic projection matrix for 2D rendering
		projection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
		return 0; // Ensure a return value in all control paths
	}




	int loadGlyph(const char glyph) {
		if (FT_Load_Char(this->face, glyph, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph: " << glyph << std::endl;
			return -1;
		}
	}

	void loadCharacters() {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(this->face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
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
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
	texture,
	glm::ivec2(this->face->glyph->bitmap.width, this->face->glyph->bitmap.rows),
	glm::ivec2(this->face->glyph->bitmap_left, this->face->glyph->bitmap_top),
	static_cast<unsigned int>(this->face->glyph->advance.x) // Ensure explicit conversion
			};
			this->Characters.insert(std::pair<char, Character>(c, character));
		}

		
	}

public:
	Text(const char* font, int fontSize = 48.f) {
		initFont(font);
		loadCharacters();
		FT_Set_Pixel_Sizes(this->face, 0, fontSize); // W=0; Dynamically calc. width based on H.
	}

	~Text() {
		FT_Done_Face(face);
		FT_Done_FreeType(ft_Library);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void renderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
	{
		// activate corresponding render state    
		s.use();
		glUniform3f(glGetUniformLocation(s.getID(), "textColor"), color.x, color.y, color.z);
		glUniform1i(glGetUniformLocation(s.getID(), "isTextRendering"), 1); // Set the text rendering flag
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(glGetUniformLocation(s.getID(), "isTextRendering"), 0); // Reset the text rendering flag
	}


};


