#include "FrontWheelNode.h"
#include "SceneNodes.h"

FrontWheelNode::FrontWheelNode() : WheelNode()
{
	

}

FrontWheelNode::FrontWheelNode(APCNode* ParentNode, std::string meshFile, glm::vec3 traslation, bool isRight, MeshManager* meshManager)
	: WheelNode(ParentNode, meshFile, traslation, isRight, meshManager)
{
	
}

void FrontWheelNode::Update(double seconds)
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

void FrontWheelNode::CreateChildren()
{
	TranslaterNode* t1 = new TranslaterNode(this, this->traslation);
	RotatorNode* r1 = new RotatorNode(t1, glm::vec3(0.f, 1.f, 0.f), 90);
	if (this->isRight)
	{
		MeshNode* m1 = new MeshNode(r1, this->meshFile, mManager);
	}
	else
	{
		RotatorNode* r2 = new RotatorNode(r1, glm::vec3(0.f, 1.f, 0.f), 180);
		MeshNode* m1 = new MeshNode(r2, this->meshFile, mManager);
	}
}