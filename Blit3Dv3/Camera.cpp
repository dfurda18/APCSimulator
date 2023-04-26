#define _USE_MATH_DEFINES
#include <cmath>
#include "Camera.h"

Camera::Camera(APCNode* follow, glm::vec3 offset)
{
	this->offset = offset;
	this->follow = follow;
	this->viewMatrix = glm::mat4();
	this->followMode = false;
}

void Camera::Update(double seconds)
{
	// move and look at the followed entity
	this->viewMatrix = glm::lookAt(this->GetPosition(), this->follow->position, glm::vec3(0.f, 1.f, 0.f));
}

glm::vec3 Camera::GetPosition()
{
	// Return the offset if it's not follosing
	glm::vec3 position = this->offset;
	if (this->followMode)
	{
		// Calculate the position from the follow entity position
		float radians = this->follow->angle + M_PI;
		glm::vec3 facing;
		facing.x = -std::cos(radians) * this->offset.x;
		facing.z = std::sin(radians) * this->offset.x;
		facing.y = this->offset.y;
		position = this->follow->position + facing;
	}
	return position;
}