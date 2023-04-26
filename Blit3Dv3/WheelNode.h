#pragma once

#include "SceneManager.h"
#include "APCNode.h"
#include "Mesh.h"

/**
* This class Represents a WheelNode.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class WheelNode : public SceneNode
{
public:
	/**
	* The Mesh Manager.
	*/
	MeshManager* mManager = NULL;
	/**
	* A pointer to the APC.
	*/
	APCNode* apc;
	std::string meshFile;
	/**
	* The wheel mesh file.
	*/
	glm::vec3 traslation;
	/**
	* This tells if the wheel is on the right side.
	*/
	bool isRight;
	/**
	* This tells if the wheel is in the front.
	*/
	bool isFront;
	/**
	* The default WheelNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	WheelNode();
	/**
	* The WheelNode constructor.
	* @param ParentNode The Parent node the node is instanced from.
	* @param meshFile The node's mesh file.
	* @param traslation The vector with the distance needed to place the node to its proper location.
	* @param isRight Whether the wheel is in the right side.
	* @param isfront Whether the wheel is in the front side.
	* @param meshManager The Mesh Manager.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	WheelNode(APCNode* ParentNode, std::string meshFile, glm::vec3 traslation, bool isRight, bool isFront, MeshManager* meshManager);
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
	* Returns the APC's speed.
	* @return The APC's speed.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	float GetSpeed();
	/**
	* Returns the APC's wheel steering value.
	* @return The APC's wheel steering value.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	float GetWheelSteer();
};