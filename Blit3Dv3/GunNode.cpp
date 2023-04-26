#include "GunNode.h"
#include "SceneNodes.h"
#include "GunRotatingNode.h"

GunNode::GunNode()
{
	typeId = sceneNodeType::APC;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;

}

GunNode::GunNode(SceneNode* ParentNode, std::string meshFile, vec3 traslation, MeshManager* meshManager, APCNode* apc)
{
	// Set default values
	typeId = sceneNodeType::APC;
	mManager = meshManager;
	this->meshFile = meshFile;
	this->traslation = traslation;
	this->apc = apc;
	modelMatrix = glm::mat4(1.f);
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
	CreateChildren();
}

void GunNode::Update(double seconds)
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

void GunNode::CreateChildren()
{
	// Add the children nodes. The GunRotatingNode will allow it to move.
	TranslaterNode* t1 = new TranslaterNode(this, this->traslation);
	GunRotatingNode* gr1 = new GunRotatingNode(t1, glm::vec3(0.f, 0.f, 1.f), this);
	RotatorNode* r1 = new RotatorNode(gr1, glm::vec3(0.f, 1.f, 0.f), 90);
	RotatorNode* r2 = new RotatorNode(r1, glm::vec3(1.f, 0.f, 0.f), 198);
	MeshNode* m1 = new MeshNode(r2, this->meshFile, mManager);
}

float GunNode::GetGunAngle()
{
	return this->apc->gunAngle;
}