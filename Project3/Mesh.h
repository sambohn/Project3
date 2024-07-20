#pragma once

#include<iostream>
#include<vector>

// classes
#include "Vertex.h"
#include "Primitives.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"


class Mesh {

private:
    Vertex* vertexArray;
    GLuint* indexArray;
    unsigned nrOfVertices;
    unsigned nrOfIndices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 origin;
    glm::vec3 scale;
    glm::mat4 ModelMatrix;

    void initVAO() {
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
        glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW); // Data sent to GPU

        // GEN EBO & BIND & SEND DATA
        // Element Buffer Object
        if (this->nrOfIndices > 0) {
            glGenBuffers(1, &this->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW); // Do once (All on GPU side)
        }

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



    void updateUniforms(Shader* shader) {
        shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
    }

    void updateModelMatrix() {
        this->ModelMatrix = glm::mat4(1.f); // Make identity matrix
        this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin); // calculations done right to left
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
        this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
        this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale); // internally reverse. [scale>rot>trans]
    }

public:
    Mesh(Primitive* primitive,
        glm::vec3 position = glm::vec3(0.f),
        glm::vec3 rotation = glm::vec3(0.f),
        glm::vec3 origin = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f)) {

        this->position = position;
        this->rotation = rotation;
        this->origin = origin;
        this->scale = scale;

        this->nrOfVertices = primitive->getNrOfVertices();
        this->nrOfIndices = primitive->getNrOfIndicies();

        // Copy over data [ memory intensive!!!]
        this->vertexArray = new Vertex[this->nrOfVertices];
        for (size_t i = 0; i < this->nrOfVertices; i++) {
            this->vertexArray[i] = primitive->getVertices()[i];
        }

        this->indexArray = new GLuint[this->nrOfIndices];
        for (size_t i = 0; i < this->nrOfIndices; i++) {
            this->indexArray[i] = primitive->getIndicies()[i];
        }
         

        this->initVAO();
        this->updateModelMatrix();
    }

    Mesh(Vertex* vertexArray,
        const unsigned& nrOfVertices,
        GLuint* indexArray,
        const unsigned& nrOfIndices,
        glm::vec3 position = glm::vec3(0.f),
        glm::vec3 origin = glm::vec3(0.f),
        glm::vec3 rotation = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f)) {

        this->position = position;
        this->rotation = rotation;
        this->origin = origin;
        this->scale = scale;

        this->nrOfVertices = nrOfVertices;
        this->nrOfIndices = nrOfIndices;

        // Copy over data [ memory intensive!!!]
        this->vertexArray = new Vertex[this->nrOfVertices];
        for (size_t i = 0; i < nrOfVertices; i++) {
            this->vertexArray[i] = vertexArray[i];
        }

        this->indexArray = new GLuint[this->nrOfIndices];
        for (size_t i = 0; i < nrOfIndices; i++) {
            this->indexArray[i] = indexArray[i];
        }

        this->initVAO();
        this->updateModelMatrix();
    }

    Mesh(const Mesh& obj) {
        this->position = obj.position;
        this->rotation = obj.rotation;
        this->origin = obj.origin;
        this->scale = obj.scale;

        this->nrOfVertices = obj.nrOfVertices;
        this->nrOfIndices = obj.nrOfIndices;

        // Copy over data [ memory intensive!!!]
        this->vertexArray = new Vertex[this->nrOfVertices];
        for (size_t i = 0; i < this->nrOfVertices; i++) {
            this->vertexArray[i] = obj.vertexArray[i];
        }

        this->indexArray = new GLuint[this->nrOfIndices];
        for (size_t i = 0; i < this->nrOfIndices; i++) {
            this->indexArray[i] = obj.indexArray[i];
        }

        this->initVAO();
        this->updateModelMatrix();
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        if (this->nrOfIndices > 0)
            glDeleteBuffers(1, &this->EBO);
        delete[] this->vertexArray;
        delete[] this->indexArray;

    }

    // Modifiers
    void setPosition(const glm::vec3& position) {
        this->position = position;
    }

    void setOrigin(const glm::vec3& origin) {
        this->origin = origin;
    }

    void setRotation(const glm::vec3& rotation) {
        this->rotation = rotation;
    }

    void setScale(const glm::vec3& scale) {
        this->scale = scale;
    }

    void move(const glm::vec3 position) {
        this->position += position;
    }

    void rotate(const glm::vec3 rotation) {
        this->rotation += rotation;
    }

    void scaleUp(const glm::vec3 scale) {
        this->scale += scale;
    }


    // Functions
    void update() {
        // Update logic
    }

    void render(Shader* shader) {
        this->updateModelMatrix();
        // Update uniforms
        this->updateUniforms(shader);

        // Bind vertex array object
        glBindVertexArray(this->VAO);

        shader->use(); // when update uniforms shader is unbound

        // RENDER
        if (this->nrOfIndices == 0) {
            glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
        }
        else
            glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
    }
};
