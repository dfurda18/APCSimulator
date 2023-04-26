#include "TorretNode.h"
#include "SceneNodes.h"
#include "TorretTurningNode.h"
#include "GunNode.h"

TorretNode::TorretNode()
{
	typeId = sceneNodeType::APC;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;

}

TorretNode::TorretNode(APCNode* apcNode, std::string meshFile, vec3 traslation, MeshManager* meshManager)
{
	// Set the initial values
	typeId = sceneNodeType::APC;
	mManager = meshManager;
	this->meshFile = meshFile;
	this->traslation = traslation;
	this->apc = apcNode;
	modelMatrix = glm::mat4(1.f);
	//Set parent and add node to parent children
	parent = apcNode;
	parent->children.push_back(this);
	CreateChildren();
}

void TorretNode::Update(double seconds)
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

void TorretNode::CreateChildren()
{
	// Add the children
	TranslaterNode* t1 = new TranslaterNode(this, this->traslation);
	// This node turns the torret
	TorretTurningNode* tt1 = new TorretTurningNode(t1, vec3(0.f, 1.f, 0.f), this);
	// Fix the model original rotation
	RotatorNode* r1 = new RotatorNode(tt1, glm::vec3(0.f, 1.f, 0.f), 90);
	RotatorNode* r2 = new RotatorNode(r1, glm::vec3(1.f, 0.f, 0.f), 198);
	MeshNode* m1 = new MeshNode(r2, this->meshFile, mManager);
	GunNode* gun = new GunNode(tt1, "Data/Gun.s3d", glm::vec3(0.f, 0.f, 0.f), mManager, this->apc);
}

float TorretNode::GetTorretAngle()
{
	return this->apc->GetTorretAngle();
}