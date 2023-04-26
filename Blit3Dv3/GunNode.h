#pragma once

#include "SceneManager.h"
#include "APCNode.h"
#include "Mesh.h"

/**
* This class Represents a Gun node.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class GunNode : public SceneNode
{
public:
	/**
	* The Mesh Manager
	*/
	MeshManager* mManager = NULL;
	/**
	* The Gun mesh.
	*/
	std::string meshFile;
	/**
	* The position of the gun in reference of the torret.
	*/
	glm::vec3 traslation;
	/**
	* The APC used to know the rotation angles.
	*/
	APCNode* apc = NULL;
	/**
	* The default GunNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	GunNode();
	/**
	* The GunNode constructor.
	* @param ParentNode The Parent node the node is instanced from.
	* @param meshFile The gun mesh file.
	* @param traslation The traslation vector from the torret to the gun's position
	* @param meshManager The Mesh Manager.
	* @param apc The APCNode that contains this node.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	GunNode(SceneNode* ParentNode, std::string meshFile, glm::vec3 traslation, MeshManager* meshManager, APCNode* apc);
	/**
	* Updates the Gun.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
	/**
	* Creates all the Gun's children nodes.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void CreateChildren();
	/**
	* Returns the Gun angle.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	float GetGunAngle();
};