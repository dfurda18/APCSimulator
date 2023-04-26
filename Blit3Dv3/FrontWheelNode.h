#pragma once

#include "WheelNode.h"
#include "APCNode.h"
#include "Mesh.h"

class FrontWheelNode :  WheelNode
{
public:
	FrontWheelNode();
	FrontWheelNode(APCNode* ParentNode, std::string meshFile, glm::vec3 traslation, bool isRight, MeshManager* meshManager);
	void Update(double seconds);
	void CreateChildren();
};