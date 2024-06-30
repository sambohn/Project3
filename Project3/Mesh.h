#pragma once

#include<iostream>
#include<vector>

// classes
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"

class Mesh {

private:
    unsigned nrOfVertices;
    unsigned nrOfIndices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 ModelMatrix;

    void initVAO(Vertex* vertexArray,
        const unsigned& numVerticies,
        GLuint* indexArray,
        const unsigned& numIndicies) {

        // Set variables -> CPU to GPU
        this->nrOfIndices = numIndicies;
        this->nrOfVertices = numVerticies;

        // Create VAO
        // VAO, VBO, EBO
        // GEN VAO, & BIND
        // VAO: vertex Array Object
        glCreateVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        // GEN VBO & BIND & SEND DATA
        // Vertex Buffer Object
        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW); // Data sent to GPU

        // GEN EBO & BIND & SEND DATA
        // Element Buffer Object
        glGenBuffers(1, &this->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW); // Do once (All on GPU side)

        // SET VERTEXATRIPOINTERS & ENABLE (Input Assembly)
        // GLuint attribloc = glGetAttribLocation(core_program);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        // Texcoord
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
        glEnableVertexAttribArray(2);

        // Normal
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3);

        // BIND VAO 0
        glBindVertexArray(0); // Unbind any active Array
    }

    void initModelMatrix() {
        this->position = glm::vec3(0.f);
        this->rotation = glm::vec3(0.f);
        this->scale = glm::vec3(1.f);

        // MODEL MATRIX
        this->ModelMatrix = glm::mat4(1.f); // Make identity matrix
        this->ModelMatrix = glm::translate(this->ModelMatrix, this->position); // calculations done right to left
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
        this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale); // internally reverse. [scale>rot>trans]
    }

    void updateUniforms(Shader* shader) {
        shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
    }

public:
    Mesh(Vertex* vertexArray,
        const unsigned& numVerticies,
        GLuint* indexArray,
        const unsigned& numIndicies) {

        this->initVAO(vertexArray, numVerticies, indexArray, numIndicies);
        this->initModelMatrix();
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
    }

    void update() {
        // Update logic
    }

    void render(Shader* shader) {
        // Update uniforms
        this->updateUniforms(shader);

        // Bind vertex array object
        glBindVertexArray(VAO);

        shader->use(); // when update uniforms shader is unbound

        // RENDER
        glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
    }
};
