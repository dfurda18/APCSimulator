#pragma once

#include "SceneManager.h"
#include "Mesh.h"

/**
* This class Represents a HullNode.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class HullNode : public SceneNode
{
public:
	/**
	* The Mesh Manager
	*/
	MeshManager* mManager = NULL;
	/**
	* The Node mesh
	*/
	std::string meshFile;
	/**
	* The vector with the distance needed to place the Node to its proper location.
	*/
	glm::vec3 traslation;
	/**
	* The default HullNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	HullNode();
	/**
	* The HullNode constructor.
	* @param ParentNode The Parent node the node is instanced from.
	* @param meshFile The node's mesh file.
	* @param traslation The vector with the distance needed to place the node to its proper location.
	* @param meshManager The Mesh Manager.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	HullNode(SceneNode* ParentNode, std::string meshFile, glm::vec3 traslation, MeshManager* meshManager);
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
};