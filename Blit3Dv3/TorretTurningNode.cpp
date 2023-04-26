#include "TorretTurningNode.h"
#include "TorretNode.h"
#include <glm/gtc/matrix_transform.hpp>

TorretTurningNode::TorretTurningNode()
{
	angle = 0.f;
	axis = glm::vec3(0.f, 0.f, 1.f);

	parent = NULL;
}

TorretTurningNode::TorretTurningNode(SceneNode* ParentNode, glm::vec3 axis, TorretNode* torretNode)
{
	// Set the initial values
	this->angle = 0.f;
	this->axis = axis;
	this->torret = torretNode;
	//Set parent and add node to parent children
	this->parent = ParentNode;
	parent->children.push_back(this);
}

void TorretTurningNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	// Rotate the Torret by the angle from the APC
	this->angle = glm::radians(this->torret->GetTorretAngle());

	//translate
	modelMatrix = glm::rotate(modelMatrix, this->angle, this->axis);
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}