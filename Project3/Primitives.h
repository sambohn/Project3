#pragma once
#include <glew.h>
#include <glfw3.h>
#include <vector>
#include "Vertex.h"


class Primitive {

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indicies;


public:
	Primitive() {}
	virtual ~Primitive() {}

	// Functions
	void set(const Vertex* vertices,
		const unsigned nrOfVertices,
		const GLuint* indicies,
		const unsigned nrOfIndices) {

		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertices.push_back(vertices[i]);
		}

		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indicies.push_back(indicies[i]);
		}
	}

	inline Vertex* getVertices() { return this->vertices.data(); }
	inline GLuint* getIndicies() { return this->indicies.data(); }
	inline const unsigned getNrOfVertices() { return this->vertices.size(); }
	inline const unsigned getNrOfIndicies() { return this->indicies.size(); }
};


class Triangle : public Primitive {

public:
	Triangle()
		: Primitive() {
		Vertex vertices[] =
		{
			// Position                         // Color                        // Texcoords                // Normals                  

			glm::vec3(-0.5f, 0.5f, 0.f),         glm::vec3(1.f, 0.f, 0.f),       glm::vec2(0.f, 1.f),        glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),        glm::vec3(0.f, 1.f, 0.5f),       glm::vec2(0.f, 0.f),        glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.5f, -0.5f, 0.f),         glm::vec3(0.f, 0.f, 0.5f),       glm::vec2(1.f, 0.f),        glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.5f, 0.5f, 0.f),          glm::vec3(1.f, 1.f, 0.f),       glm::vec2(1.f, 1.f),        glm::vec3(0.f, 0.f, -1.f)

		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indicies[] =
		{
			0, 1, 2 // Triangle 1
		};
		unsigned nrOfIndicies = sizeof(indicies) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indicies, nrOfIndicies);
	}
};

class Quad : public Primitive {

public:
	Quad() 
		: Primitive() {
		Vertex vertices[] =
		{
			// Position                         // Color                        // Texcoords                // Normals                  

			glm::vec3(-0.5f, 0.5f, 0.f),         glm::vec3(1.f, 0.f, 0.f),       glm::vec2(0.f, 1.f),        glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),        glm::vec3(0.f, 1.f, 0.5f),       glm::vec2(0.f, 0.f),        glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.5f, -0.5f, 0.f),         glm::vec3(0.f, 0.f, 0.5f),       glm::vec2(1.f, 0.f),        glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.5f, 0.5f, 0.f),          glm::vec3(1.f, 1.f, 0.f),       glm::vec2(1.f, 1.f),        glm::vec3(0.f, 0.f, -1.f)

		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indicies[] =
		{
			0, 1, 2, // Triangle 1
			0, 2, 3  // Triangle 2
		};
		unsigned nrOfIndicies = sizeof(indicies) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indicies, nrOfIndicies);
	}
};