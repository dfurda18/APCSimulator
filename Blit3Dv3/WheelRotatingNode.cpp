#include "WheelRotatingNode.h"
#include "WheelNode.h"
#include <glm/gtc/matrix_transform.hpp>

WheelRotatingNode::WheelRotatingNode()
{
	angle = 0.f;
	axis = glm::vec3(0.f, 0.f, 1.f);

	parent = NULL;
}

WheelRotatingNode::WheelRotatingNode(SceneNode* ParentNode, glm::vec3 axis, WheelNode* wheelNode)
{
	// Set the initial values
	this->angle = 0.f;
	this->axis = axis;
	this->wheel = wheelNode;

	// Set parent and add node to parent children
	this->parent = ParentNode;
	parent->children.push_back(this);
}

void WheelRotatingNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");

	// get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	// apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	

	// Calculate the current angle
	this->angle += glm::radians(-this->wheel->GetSpeed() * seconds);
	if (this->angle > 2.f * glm::pi<float>())
	{
		this->angle -= 2.f * glm::pi<float>();
	}

	// rotate
	modelMatrix = glm::rotate(modelMatrix, this->angle, this->axis);
	snodeMutex.unlock();

	// update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}