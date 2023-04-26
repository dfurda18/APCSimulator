#pragma once

#include "SceneManager.h"
#include "Mesh.h"

class TranslaterNode : public SceneNode
{
public:
	//need a vector to translate by
	glm::vec3 offset;
	//override Update() so that we can translate
	void Update(double seconds);
	TranslaterNode();
	TranslaterNode(SceneNode* ParentNode, glm::vec3 offSet);
};

class RotatingNode : public SceneNode
{
public:
	//need a vector to translate by
	glm::vec3 axis;
	int angle;
	float currentAngle;
	//override Update() so that we can translate
	void Update(double seconds);
	RotatingNode();
	RotatingNode(SceneNode* ParentNode, glm::vec3 axis, int angle);
};

class RotatorNode : public SceneNode
{
public:
	//need a vector to translate by
	glm::vec3 axis;
	float angle;
	//override Update() so that we can translate
	void Update(double seconds);
	RotatorNode();
	RotatorNode(SceneNode* ParentNode, glm::vec3 axis, int angle);
};

class FloaterNode : public SceneNode
{
private:
	// Set the angle
	glm::vec3 angle;
	glm::vec3 speed;
	glm::vec3 scale;

	// Modify the angle
	glm::vec3 angle2;
	glm::vec3 speed2;
	glm::vec3 scale2;

public:
	// The actual movement
	glm::vec3 offset;
	//override Update() so that we can translate
	void Update(double seconds);
	FloaterNode();
	FloaterNode(SceneNode* ParentNode, glm::vec3 speed, glm::vec3 speed2, glm::vec3 scale, glm::vec3 scale2);
};

//example node that "draws"
class PrintNode : public SceneNode
{
public:
	void Draw();
	PrintNode();
	PrintNode(SceneNode* ParentNode);
};

class MeshNode : public SceneNode
{
public:
	Mesh* mesh = NULL;
	MeshManager* mManager = NULL;

	void Draw();
	MeshNode();
	~MeshNode();
	MeshNode(std::string fileName, MeshManager* meshManager);
	MeshNode(SceneNode* ParentNode, std::string fileName, MeshManager* meshManager);
};