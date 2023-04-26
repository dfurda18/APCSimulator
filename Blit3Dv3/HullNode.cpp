#include "HullNode.h"
#include "SceneNodes.h"

HullNode::HullNode()
{
	typeId = sceneNodeType::APC;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;

}

HullNode::HullNode(SceneNode* ParentNode, std::string meshFile, vec3 traslation, MeshManager* meshManager)
{
	// Set the initial values
	typeId = sceneNodeType::APC;
	mManager = meshManager;
	this->meshFile = meshFile;
	this->traslation = traslation;
	modelMatrix = glm::mat4(1.f);
	// Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
	CreateChildren();
}

void HullNode::Update(double seconds)
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

void HullNode::CreateChildren()
{
	// Create all children
	TranslaterNode* t1 = new TranslaterNode(this, this->traslation);
	RotatorNode* r1 = new RotatorNode(t1, glm::vec3(0.f, 1.f, 0.f), 90);
	RotatorNode* r2 = new RotatorNode(r1, glm::vec3(1.f, 0.f, 0.f), 198);
	MeshNode* m1 = new MeshNode(r2, this->meshFile, mManager);
}