#pragma once
#include"libs.h"

class Light {

protected:
	float intensity;
	glm::vec3 color;

public:

	Light(float intensity, glm::vec3 color) {
		this->intensity = intensity;
		this->color = color;
	}

	~Light() {

	}

	// Setters
	void setColor(glm::vec3 color) {
		this->color = color;
	}

	void setIntensity(float intensity) {
		this->intensity = intensity;
	}

	// Function
	virtual void sendToShader(Shader& program) = 0;

};

class PointLight : public Light {

protected:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;


public:
	PointLight(glm::vec3 position, float intensity = 1.f, glm::vec3 color = glm::vec3(1.f),
		float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f)
	: Light(intensity, color) {
		this->position = position;
		this->intensity = intensity;
		this->color = color;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
	}

	~PointLight() {

	}

	// Setters
	void setPosition(glm::vec3 position) {
		this->position = position;
	}

	// Function
	void sendToShader(Shader& program) {
		program.setVec3f(this->position, "pointLight.position");
		program.set1f(this->intensity, "pointLight.intensity");
		program.setVec3f(this->color, "pointLight.color");
		program.set1f(this->constant, "pointLight.constant");
		program.set1f(this->linear, "pointLight.linear");
		program.set1f(this->quadratic, "pointLight.quadratic");
	}


};

class DirectionalLight : public Light {
protected:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	DirectionalLight(glm::vec3 direction, float intensity = 1.f, glm::vec3 color = glm::vec3(1.f),
		glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f))
		: Light(intensity, color) {
		this->direction = direction;
		this->intensity = intensity;
		this->color = color;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	~DirectionalLight() {}

	// Setters
	void setDirection(glm::vec3 direction) {
		this->direction = direction;
	}

	void sendToShader(Shader& program) {
		program.setVec3f(this->direction, "directionalLight.direction");
		program.setVec3f(this->ambient, "directionalLight.ambient");
		program.setVec3f(this->diffuse, "directionalLight.diffuse");
		program.setVec3f(this->specular, "directionalLight.specular");
	}
};
