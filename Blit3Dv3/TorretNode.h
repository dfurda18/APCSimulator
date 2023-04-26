#pragma once

#include "SceneManager.h"
#include "APCNode.h"
#include "Mesh.h"

/**
* This class Represents a TorretNode.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class TorretNode : public SceneNode
{
public:
	/**
	* The mesh manager.
	*/
	MeshManager* mManager = NULL;
	/**
	* The node's mesh.
	*/
	std::string meshFile;
	/**
	* A pointer to the APC.
	*/
	APCNode* apc;
	/**
	* The vector to traslate the node to ensamble it properly
	*/
	glm::vec3 traslation;
	/**
	* The default TorretNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	TorretNode();
	/**
	* The TorretNode constructor.
	* @param apcNode The APCNode the node is instanced from.
	* @param meshFile The node's mesh file.
	* @param traslation The vector with the distance needed to place the node to its proper location.
	* @param meshManager The Mesh Manager.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	TorretNode(APCNode* apcNode, std::string meshFile, glm::vec3 traslation, MeshManager* meshManager);
	/**
	* Updates the Node.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
	/**
	* Creates all the Node's children nodes.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void CreateChildren();
	/**
	* Returns the torret's angle.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	float GetTorretAngle();
};