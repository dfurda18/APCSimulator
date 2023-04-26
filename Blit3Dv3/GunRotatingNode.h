#pragma once

#include "SceneManager.h"
#include "GunNode.h"

/**
* This class Represents a node that rotates the gun.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class GunRotatingNode : public SceneNode
{
public:
	/**
	* The vector to rotate the gun on
	*/
	glm::vec3 axis;
	/**
	* The angle to rotate the gun
	*/
	float angle;
	/**
	* A pointer to the gun.
	*/
	GunNode* gun;
	/**
	* The default GunRotatingNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	GunRotatingNode();
	/**
	* The GunRotatingNode constructor.
	* @param ParentNode The Parent node the node is instanced from.
	* @param axis The axis to rotate the gun.
	* @param gun The GunNode that contains this node.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	GunRotatingNode(SceneNode* ParentNode, glm::vec3 axis, GunNode* gun);
	/**
	* Updates the Gun rotating logic.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
};
