#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"



class Model {


private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 originPoint;
	glm::vec3 position;

	void updateUniforms() {

	}


public:

	Model(glm::vec3 position, Material* material, Texture* orTexDif, Texture*orTexSpec, std::vector<Mesh*> meshes) {

		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;

		for (auto* i : meshes) {// Using iterators. Faster than regular loops
			this->meshes.push_back(new Mesh(*i));
		}
	}

	~Model() {
		for (auto*& i : this->meshes) {
			delete i;
		}
	}

	// Functions

	void update() {

	}

	void render(Shader* shader) {
		this->updateUniforms();

		// Update uniforms (textures)
		this->material->sendToShader(*shader); // update texture in shader [Fragment]

		// Use program
		shader->use();

		// Activate texture (binding)
		this->overrideTextureDiffuse->bind(0);
		this->overrideTextureSpecular->bind(1);



		// Draw
		for (auto& i : this->meshes)
			i->render(shader);

		//// unbinding [CLEANUP]
		//glBindVertexArray(0);
		//glUseProgram(0);
		//glActiveTexture(0);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

};