#pragma once
#include "libs.h"

class Light {
protected:
    float intensity;
    glm::vec3 color;

public:
    Light(float intensity, glm::vec3 color) : intensity(intensity), color(color) {}
    virtual ~Light() {}

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
        : Light(intensity, color), position(position), constant(constant), linear(linear), quadratic(quadratic) {}

    ~PointLight() {}

    // Setters
    void setPosition(glm::vec3 position) {
        this->position = position;
    }

    // Function
    void sendToShader(Shader& program) override {
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
        program.set1f(this->intensity, "directionalLight.intensity");
    }

};

class SpotLight : public Light {
protected:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;

public:
    SpotLight(glm::vec3 position, glm::vec3 direction, float intensity = 1.f, glm::vec3 color = glm::vec3(1.f),
        float cutOff = 12.5f, float outerCutOff = 17.5f,
        glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f),
        float constant = 1.f, float linear = 0.09f, float quadratic = 0.032f)
        : Light(intensity, color) {

        this->position = position;
        this->direction = direction;
        this->cutOff = cos(glm::radians(cutOff));
        this->outerCutOff = cos(glm::radians(outerCutOff));

        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;

        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
    }

    ~SpotLight() {}

    // Setters
    void setPosition(glm::vec3 position) {
        this->position = position;
    }

    void setDirection(glm::vec3 direction) {
        this->direction = direction;
    }

    void setCutOff(float cutOff) {
        this->cutOff = glm::cos(glm::radians(cutOff));
    }

    // Function
    void sendToShader(Shader& program) override {
        program.setVec3f(this->position, "spotLight.position");
        program.setVec3f(this->direction, "spotLight.direction");
        program.set1f(this->intensity, "spotLight.intensity");
        program.setVec3f(this->color, "spotLight.color");
        program.set1f(this->cutOff, "spotLight.cutOff");
        program.set1f(this->cutOff, "spotLight.outerCutOff");
        program.setVec3f(this->ambient, "spotLight.ambient");
        program.setVec3f(this->diffuse, "spotLight.diffuse");
        program.setVec3f(this->specular, "spotLight.specular");
        program.set1f(this->constant, "spotLight.constant");
        program.set1f(this->linear, "spotLight.linear");
        program.set1f(this->quadratic, "spotLight.quadratic");
    }
};
