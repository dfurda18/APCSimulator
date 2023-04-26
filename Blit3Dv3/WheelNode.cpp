#include "WheelNode.h"
#include "APCNode.h"
#include "SceneNodes.h"
#include "WheelRotatingNode.h"
#include "WheelSteeringNode.h"

WheelNode::WheelNode()
{
	typeId = sceneNodeType::APC;
	this->isRight = false;
	this->isFront = false;
	modelMatrix = glm::mat4(1.f);
	parent = NULL;

}

WheelNode::WheelNode(APCNode* ParentNode, std::string meshFile, glm::vec3 traslation, bool isRight, bool isFront, MeshManager* meshManager)
{
	// Set the initial values.
	typeId = sceneNodeType::APC;
	mManager = meshManager;
	this->meshFile = meshFile;
	this->traslation = traslation;
	this->isRight = isRight;
	this->isFront = isFront;
	this->apc = ParentNode;
	modelMatrix = glm::mat4(1.f);
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
	CreateChildren();
}

void WheelNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

void WheelNode::CreateChildren()
{
	// Create the children
	WheelRotatingNode* wr1;
	TranslaterNode* t1 = new TranslaterNode(this, this->traslation);
	RotatorNode* r1 = new RotatorNode(t1, glm::vec3(0.f, 1.f, 0.f), 90);
	// Front wheels need a steering node before the rotating node.
	if (this->isFront)
	{
		WheelSteeringNode* ws1 = new WheelSteeringNode(r1, glm::vec3(0.f, 1.f, 0.f), this);
		wr1 = new WheelRotatingNode(ws1, glm::vec3(1.f, 0.f, 0.f), this);
	}
	else
	{
		wr1 = new WheelRotatingNode(r1, glm::vec3(1.f, 0.f, 0.f), this);
	}
	
	// Left sheels need to be flipped 180 degrees
	if (this->isRight)
	{
		MeshNode* m1 = new MeshNode(wr1, this->meshFile, mManager);
	}
	else 
	{
		RotatorNode* r2 = new RotatorNode(wr1, glm::vec3(0.f, 1.f, 0.f), 180);
		MeshNode* m1 = new MeshNode(r2, this->meshFile, mManager);
	}
	
}

float WheelNode::GetSpeed()
{
	return this->apc->GetSpeed();
}

float WheelNode::GetWheelSteer()
{
	return this->apc->GetWheelSteer();
}