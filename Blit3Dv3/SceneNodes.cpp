#include "SceneNodes.h"
#include <glm/gtc/matrix_transform.hpp>

//for the PrintNode
#include <iostream>

TranslaterNode::TranslaterNode()
{
	typeId = sceneNodeType::TRANSLATE;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

TranslaterNode::TranslaterNode(SceneNode* ParentNode, glm::vec3 offSet)
{
	typeId = sceneNodeType::TRANSLATE;

	modelMatrix = glm::mat4(1.f);
	offset = offSet;
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
}

void TranslaterNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");
	
	//get parent's modelmatrix
	snodeMutex.lock();	
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	//translate
	modelMatrix = glm::translate(modelMatrix, offset);
	snodeMutex.unlock();

	//update all of our children
	for(auto n : children)
	{
		n->Update(seconds);
	}
}

RotatingNode::RotatingNode()
{
	angle = 0.f;
	currentAngle = 0.f;
	axis = glm::vec3(0.f, 0.f, 1.f);

	parent = NULL;
}

RotatingNode::RotatingNode(SceneNode* ParentNode, glm::vec3 axis, int angle)
{
	this->angle = angle;
	this->currentAngle = 0.f;
	this->axis = axis;
	//Set parent and add node to parent children
	this->parent = ParentNode;
	parent->children.push_back(this);
}

void RotatingNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	// Calculate the current angle
	this->currentAngle += glm::radians(this->angle * seconds);
	if (this->currentAngle > 2.f * glm::pi<float>())
	{
		this->currentAngle -= 2.f * glm::pi<float>();
	}

	//rotate
	modelMatrix = glm::rotate(modelMatrix,this->currentAngle, this->axis);
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

RotatorNode::RotatorNode()
{
	angle = 0.f;
	axis = glm::vec3(0.f, 0.f, 1.f);
	parent = NULL;
}

RotatorNode::RotatorNode(SceneNode* ParentNode, glm::vec3 axis, int angle)
{
	this->angle = angle;
	this->axis = axis;
	//Set parent and add node to parent children
	this->parent = ParentNode;
	parent->children.push_back(this);
}

void RotatorNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	//translate
	modelMatrix = glm::rotate(modelMatrix, glm::radians(this->angle), this->axis);
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

FloaterNode::FloaterNode()
{
	this->angle = glm::vec3();
	this->speed = glm::vec3();
	this->scale = glm::vec3();

	this->angle2 = glm::vec3();
	this->speed2 = glm::vec3();
	this->scale2 = glm::vec3();

	parent = NULL;
}

FloaterNode::FloaterNode(SceneNode* ParentNode, glm::vec3 speed, glm::vec3 speed2, glm::vec3 scale, glm::vec3 scale2)
{
	// Set the speed
	this->angle = glm::vec3();
	this->speed = speed;
	this->scale = scale;

	// Set the scale
	this->angle2 = glm::vec3();
	this->speed2 = speed2;
	this->scale2 = scale2;

	//Set parent and add node to parent children
	this->parent = ParentNode;
	parent->children.push_back(this);
}

void FloaterNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	//change the angle variable based on time passed since last update
	this->angle.x += static_cast<float>(seconds) * this->speed.x;
	this->angle.y += static_cast<float>(seconds) * this->speed.y;
	this->angle.z += static_cast<float>(seconds) * this->speed.z;

	this->angle2.x+= static_cast<float>(seconds) * this->speed2.x;
	this->angle2.y += static_cast<float>(seconds) * this->speed2.y;
	this->angle2.z += static_cast<float>(seconds) * this->speed2.z;

	//wrap around for angle
	if (this->angle.x > 360.f) this->angle.x -= 360.f;
	if (this->angle.x < 0.f) this->angle.x += 360.f;

	if (this->angle2.x > 360.f) this->angle2.x -= 360.f;
	if (this->angle2.x < 0.f) this->angle2.x += 360.f;

	if (this->angle.y > 360.f) this->angle.y -= 360.f;
	if (this->angle.y < 0.f) this->angle.y += 360.f;

	if (this->angle2.y > 360.f) this->angle2.y -= 360.f;
	if (this->angle2.y < 0.f) this->angle2.y += 360.f;

	if (this->angle.z > 360.f) this->angle.z -= 360.f;
	if (this->angle.z < 0.f) this->angle.z += 360.f;

	if (this->angle2.z > 360.f) this->angle2.z -= 360.f;
	if (this->angle2.z < 0.f) this->angle2.z += 360.f;

	this->offset.x = cos(glm::radians(this->angle.x)) * this->scale.x;
	this->offset.x += cos(glm::radians(this->angle2.x)) * this->scale2.x;

	this->offset.y = sin(glm::radians(this->angle.y)) * this->scale.y;
	this->offset.y += sin(glm::radians(this->angle2.y)) * this->scale2.y;

	this->offset.z = sin(glm::radians(this->angle.z)) * this->scale.z;
	this->offset.z += sin(glm::radians(this->angle2.z)) * this->scale2.z;

	//Move
	modelMatrix = glm::translate(modelMatrix, this->offset);
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

PrintNode::PrintNode()
{
	typeId = sceneNodeType::PRINT;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

PrintNode::PrintNode(SceneNode* ParentNode)
{
	typeId = sceneNodeType::PRINT;

	modelMatrix = glm::mat4(1.f);
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
}

void PrintNode::Draw()
{
	//we would normally use our modelMatrix to draw, so let's calculate where we 
	//would be in the world based on it
	glm::vec4 position = glm::vec4(0, 0, 0, 1);
	
	snodeMutex.lock();
	position = modelMatrix * position;
	snodeMutex.unlock();
	

	std::cout << "Drawing at: " << position.x << ", " << position.y << ", " << position.z << std::endl;
	//draw all of our children
	for(auto n : children)
	{
		n->Draw();
	}
}

MeshNode::~MeshNode()
{
	mManager->unloadMesh(mesh);
};

MeshNode::MeshNode(std::string fileName, MeshManager* meshManager)
{
	mManager = meshManager;
	typeId = sceneNodeType::MESH;
	mesh = mManager->LoadMesh(fileName);
	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

MeshNode::MeshNode()
{
	typeId = sceneNodeType::MESH;
	mesh = NULL;
	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

MeshNode::MeshNode(SceneNode* ParentNode, std::string fileName, MeshManager* meshManager)
{
	mManager = meshManager;
	typeId = sceneNodeType::MESH;
	mesh = mManager->LoadMesh(fileName);
	modelMatrix = glm::mat4(1.f);
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
}

void MeshNode::Draw()
{
	mesh->Draw(modelMatrix);

	for (auto& n : children) n->Draw();
}
