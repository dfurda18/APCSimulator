#pragma once

#include "SceneManager.h"
#include "WheelNode.h"

/**
* This class Represents a node that steers the wheel.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class WheelSteeringNode : public SceneNode
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
	* A pointer to the Wheel
	*/
	WheelNode* wheel;
	/**
	* The default WheelSteeringNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	WheelSteeringNode();
	/**
	* The WheelSteeringNode constructor.
	* @param ParentNode The Parent node the node is instanced from.
	* @param axis The axis to rotate the torret.
	* @param wheelNode The WheelNode that contains this node.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	WheelSteeringNode(SceneNode* ParentNode, glm::vec3 axis, WheelNode* wheelNode);
	/**
	* Updates the Torret rotating logic.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
};