#pragma once
#include <iostream>

#include <vector>

// OpenGL
#include <glew.h>
#include <glfw3.h>

// OpenGL Mathematics Library
#include <glm.hpp>
#include <vec3.hpp> // glm::vec3

#include <gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <gtc\type_ptr.hpp>

enum direction {FORWARD = 0, BACKWARD, LEFT, RIGHT};

class Camera {

private:
	glm::mat4 ViewMatrix;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;


	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	void updateCameraVectors() {
		this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front.y = sin(glm::radians(this->pitch));
		this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->front = glm::normalize(this->front); // Always normalize vectors
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}


public:

	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp) {
		this->ViewMatrix = glm::mat4(1.f);

		this->movementSpeed = 3.f;
		this->sensitivity = 20.f;
		this->worldUp = worldUp;
		this->position = position;
		this->right = glm::vec3(0.f);
		this->up = worldUp;
		
		this->pitch = 0.f;
		this->yaw = -90.f;
		this->roll = 0.f;
	}

	~Camera() {

	}

	// Accessors
	const glm::mat4 getViewMatrix() {
		this->updateCameraVectors();
		this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
		return this->ViewMatrix;
	}


	const glm::vec3 getPosition() {
		return this->position;
	}

	void move(const float& dt, const int direction) {
		// Update position vector
		switch (direction) {
		case FORWARD:
			this->position += this->front * this->movementSpeed * dt;
			break;
		case BACKWARD:
			this->position -= this->front * this->movementSpeed * dt;
			break;
		case LEFT:
			this->position -= this->right * this->movementSpeed * dt;
			break;
		case RIGHT:
			this->position += this->right * this->movementSpeed * dt;
			break;
		default:
			break;
		}
	}

	void updateMouseInput(const float& dt, const double& offsetX, const double& offsetY) {
		// Invert the mouse Y offset
		this->pitch -= static_cast<GLfloat>(offsetY) * this->sensitivity * dt; // Pitch = up & down (inverted)
		this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity * dt; // Yaw = Left & Right

		// Check bounds
		if (this->pitch > 80.f)
			this->pitch = 80.f;
		else if (this->pitch < -80.f)
			this->pitch = -80.f;

		if (this->yaw > 360.f || this->yaw < -360.f)
			this->yaw = 0.f;
	}

	void updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY) {
		this->move(dt, direction);
		this->updateMouseInput(dt, offsetX, offsetY);
		
	}
};
