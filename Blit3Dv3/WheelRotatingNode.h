#pragma once

#include "SceneManager.h"
#include "WheelNode.h"

/**
* This class Represents a node that spinds the wheel.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class WheelRotatingNode : public SceneNode
{
public:
	/**
	* The axis to rotate the torret.
	*/
	glm::vec3 axis;
	/**
	* The rotation angle.
	*/
	float angle;
	/**
	* A pointer to the WheelNode
	*/
	WheelNode* wheel;
	/**
	* The default WheelRotatingNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	WheelRotatingNode();
	/**
	* The TorretTurningNode constructor.
	* @param ParentNode The Parent node the node is instanced from.
	* @param axis The axis to spin the sheel.
	* @param wheelNode The WheelNode that contains this node.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	WheelRotatingNode(SceneNode* ParentNode, glm::vec3 axis, WheelNode* wheelNode);
	/**
	* Updates the Torret rotating logic.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
};