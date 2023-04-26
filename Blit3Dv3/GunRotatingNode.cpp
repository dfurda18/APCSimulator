#include "GunRotatingNode.h"
#include "TorretNode.h"
#include <glm/gtc/matrix_transform.hpp>

GunRotatingNode::GunRotatingNode()
{
	angle = 0.f;
	axis = glm::vec3(0.f, 0.f, 1.f);

	parent = NULL;
}

GunRotatingNode::GunRotatingNode(SceneNode* ParentNode, glm::vec3 axis, GunNode* gun)
{
	this->angle = 0.f;
	this->axis = axis;
	this->gun = gun;
	//Set parent and add node to parent children
	this->parent = ParentNode;
	parent->children.push_back(this);
}

void GunRotatingNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	// Set the angle from the parent node.
	this->angle = glm::radians(this->gun->GetGunAngle());

	//translate
	modelMatrix = glm::rotate(modelMatrix, this->angle, this->axis);
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}