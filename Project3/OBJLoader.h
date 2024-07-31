#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<algorithm>

#include<glew.h>
#include<glfw3.h>

#include<glm.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include "Vertex.h"

static std::vector<Vertex> loadOBJ(const char* file_name) {

    // Vertex Positions
    std::vector<glm::fvec3> vertex_positions;
    std::vector<glm::fvec2> vertex_texcoords;
    std::vector<glm::fvec3> vertex_normals;

    // Face vectors
    std::vector<GLuint> vertex_position_indices;
    std::vector<GLuint> vertex_texcoord_indices;
    std::vector<GLuint> vertex_normal_indices;

    // Vertex array
    std::vector<Vertex> vertices;

    std::stringstream ss;
    std::ifstream in_file(file_name);
    std::string line = "";
    std::string prefix = "";
    glm::vec3 temp_vec3;
    glm::vec2 temp_vec2;
    GLuint temp_gluint = 0;

    // File open error check
    if (!in_file.is_open()) {
        throw std::runtime_error("ERROR::OBJLOADER::Could not open file.");
    }

    // Read line by line
    while (std::getline(in_file, line)) {
        ss.clear(); // Clear the stringstream
        ss.str(line); // Set the new line
        ss >> prefix;

        if (prefix == "#") {
            // Comment line, skip
        }
        else if (prefix == "v") { // Vertex position
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertex_positions.push_back(temp_vec3);
        }
        else if (prefix == "vt") { // Texcoords
            ss >> temp_vec2.x >> temp_vec2.y;
            vertex_texcoords.push_back(temp_vec2);
        }
        else if (prefix == "vn") { // Vertex normals
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertex_normals.push_back(temp_vec3);
        }
        else if (prefix == "f") { // Face
            std::vector<GLuint> posIndices, texIndices, normIndices;
            std::string vertex;
            while (ss >> vertex) {
                std::stringstream vertexSS(vertex);
                std::string indexStr;
                int index[3] = { 0, 0, 0 };
                int i = 0;
                while (std::getline(vertexSS, indexStr, '/')) {
                    if (!indexStr.empty()) {
                        index[i] = std::stoi(indexStr);
                    }
                    ++i;
                }
                posIndices.push_back(index[0]);
                texIndices.push_back(index[1]);
                normIndices.push_back(index[2]);
            }
            // Triangulate if necessary
            for (size_t i = 1; i < posIndices.size() - 1; ++i) {
                vertex_position_indices.push_back(posIndices[0]);
                vertex_position_indices.push_back(posIndices[i]);
                vertex_position_indices.push_back(posIndices[i + 1]);

                vertex_texcoord_indices.push_back(texIndices[0]);
                vertex_texcoord_indices.push_back(texIndices[i]);
                vertex_texcoord_indices.push_back(texIndices[i + 1]);

                vertex_normal_indices.push_back(normIndices[0]);
                vertex_normal_indices.push_back(normIndices[i]);
                vertex_normal_indices.push_back(normIndices[i + 1]);
            }
        }
    }

    // Build final vertex array (mesh)
    vertices.resize(vertex_position_indices.size(), Vertex());

    // Load in all indices
    for (size_t i = 0; i < vertex_position_indices.size(); ++i) {
        GLuint posIndex = vertex_position_indices[i];
        GLuint texIndex = vertex_texcoord_indices[i];
        GLuint normIndex = vertex_normal_indices[i];

        vertices[i].position = vertex_positions[posIndex - 1];

        // Check if texIndex and normIndex are valid (non-zero)
        if (texIndex > 0) {
            vertices[i].texcoord = vertex_texcoords[texIndex - 1];
        }
        else {
            vertices[i].texcoord = glm::vec2(0.0f, 0.0f); // Default texcoord
        }

        if (normIndex > 0) {
            vertices[i].normal = vertex_normals[normIndex - 1];
        }
        else {
            vertices[i].normal = glm::vec3(0.0f, 0.0f, 0.0f); // Default normal
        }

        vertices[i].color = glm::vec3(1.f, 1.f, 1.f); // Default color
    }

    // DEBUG
    std::cout << "Nr of vertices: " << vertices.size() << "\n";

    // Loaded successfully
    return vertices;
}
