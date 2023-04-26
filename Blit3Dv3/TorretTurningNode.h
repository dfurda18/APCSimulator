#pragma once

#include "SceneManager.h"
#include "TorretNode.h"

/**
* This class Represents a node that rotates the torret.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class TorretTurningNode : public SceneNode
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
	* A pointer to the Torret
	*/
	TorretNode* torret;
	/**
	* The default TorretTurningNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	TorretTurningNode();
	/**
	* The TorretTurningNode constructor.
	* @param ParentNode The Parent node the node is instanced from.
	* @param axis The axis to rotate the torret.
	* @param torretNode The TorretNode that contains this node.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	TorretTurningNode(SceneNode* ParentNode, glm::vec3 axis, TorretNode* torretNode);
	/**
	* Updates the Torret rotating logic.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
};