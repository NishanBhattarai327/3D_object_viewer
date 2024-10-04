#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// camera movement direction enum
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// default camera values
const float YAW = -90.f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.f;

class Camera {
public:
	// camera attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// euler angle
	float yaw;
	float pitch;

	// camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;  // fov (field of view)

	// constructor with vector values
	Camera( glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), 
			glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), 
			float yaw = YAW, float pitch = PITCH)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), 
		  mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = worldUp;
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), 
		  mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
		//return calculate_lookAt_mat(position, position + front, worldUp);
	}

	void processKeyboard(Camera_Movement direction, float deltaTime) {
		const float velocity = 1.5f * deltaTime;
		if (direction == FORWARD) {
			position += velocity * front;
		}
		if (direction == BACKWARD) {
			position -= velocity * front;
		}
		if (direction == LEFT) {
			position -= glm::normalize(glm::cross(front, up)) * velocity;
		}
		if (direction == RIGHT) {
			position += glm::normalize(glm::cross(front, up)) * velocity;
		}
	}

	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
		yaw += xoffset * mouseSensitivity;
		pitch += yoffset * mouseSensitivity;

		if (constrainPitch) {
			if (pitch > 89.9f)  pitch = 89.9f;
			if (pitch < -89.9f) pitch = -89.9f;
		}

		// update direction vectors (front, right, up)
		updateCameraVectors();
	}

	void processMouseScroll(float yoffset) {
		zoom -= (float)yoffset;
		if (zoom > 45.f)
			zoom = 45.f;
		if (zoom < 1.f)
			zoom = 1.f;	
	}
private:
	glm::mat4 calculate_lookAt_mat(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
		glm::vec3 n = glm::normalize(position - target);        // z-axis (forward)
		glm::vec3 u = glm::normalize(glm::cross(n, worldUp));   // x-axis (right)
		glm::vec3 v = glm::normalize(glm::cross(u, n));         // y-axis (up)

		// In glm we access elements as mat[col][row] due to column-major layout
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0][0] = u.x;
		rotation[1][0] = u.y;
		rotation[2][0] = u.z;
		
		rotation[0][1] = v.x;
		rotation[1][1] = v.y;
		rotation[2][1] = v.z;

		rotation[0][2] = n.x;
		rotation[1][2] = n.y;
		rotation[2][2] = n.z;

		glm::mat4 translation = glm::mat4(1.0f);
		translation[3][0] = -position.x;
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;

		return rotation * translation;
	}

	void updateCameraVectors() {
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		front   = glm::normalize(front);

		right   = glm::normalize(glm::cross(front, worldUp));
		up      = glm::normalize(glm::cross(right, front));
	}
};
