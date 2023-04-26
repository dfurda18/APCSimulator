#pragma once

#include "SceneManager.h"
#include "SceneNodes.h"
#include "Mesh.h"

/**
* This class Represents an APC node. It handles all of its interactions.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class APCNode : public SceneNode
{
private:
	// Configurable constants
	/**
	* The max angle the wheels can steer.
	*/
	const float TOP_STEER = 35.f;
	/**
	* The amount of angle per second the wheels can turn.
	*/
	const float STEER_DELTA = 25.f;
	/**
	* The amount of speed tha APC increases per second.
	*/
	const float SPEED_DELTA = 400.f;
	/**
	* The max speed. This number is related to the amount of turns the wheel can make in one second.
	*/
	const float MAX_SPEED = 720.f;
	/**
	* Because of the speed being based on the wheel turning speed, the movement has to be capped. 
	* This number is divided tot he speed to reduce the movement.
	*/
	const float MOVEMENT_CAP = 4000.f;
	/**
	* The speed lost per second when not accelerating.
	*/
	const float FRICTION = 200;
	/**
	* The angle amount the torret is turning per second.
	*/
	const float TORRET_DELTA = 40.f;
	/**
	* Amodifier to return the wheels back to their place when not turning them.
	*/
	const float RETURN_WHEEL_VALUE = 9000.f;
	/**
	* The angle amount the gun is turning per second.
	*/
	const float GUN_DELTA = 5.f;
	/**
	* The max gun angle. This values are reversed. The positive move the gun down.
	*/
	const float MAX_GUN_ANGLE = 6.f;
	/**
	* The min amount for the gun angle. 
	*/
	const float MIN_GUN_ANGLE = -4.f;
	/**
	* A constant that define how well the tires can rotate the tank.
	*/
	const float TIRE_TRACTION = 3.f;
	
public:
	/**
	* The mesh manager.
	*/
	MeshManager* mManager = NULL;
	// Controls
	/**
	* Whether or not the APC is rotating left.
	*/
	bool left;
	/**
	* Whether or not the APC is steering.
	*/
	bool steering;
	/**
	* Whether or not the APC is moving.
	*/
	bool moving;
	/**
	* Whether or not the APC is going forward.
	*/
	bool forward;
	/**
	* Whether or not the APC is rotating the torret.
	*/
	bool movingTorret;
	/**
	* Whether or not the APC's torret is rotating left.
	*/
	bool torretAngleLeft;
	/**
	* Whether or not the APC is moving the gun.
	*/
	bool movingGun;
	/**
	* Whether or not the APC is moving the gun up.
	*/
	bool gunAngleUp;
	// Values
	/**
	* The APC's position.
	*/
	vec3 position;
	/**
	* The APC's speed.
	*/
	float speed;
	/**
	* The APC's angle.
	*/
	float angle;
	/**
	* The APC's torret angle.
	*/
	float torretAngle;
	/**
	* The APC's gun angle.
	*/
	float gunAngle;
	/**
	* The APC's wheel steering angle.
	*/
	float wheelSteer;
	/**
	* The default APCNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	APCNode();
	/**
	* The APCNode constructor.
	* @param ParentNode The Parent node the APC is instanced from.
	* @param meshManager The Mesh Manager.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	APCNode(SceneNode* ParentNode, MeshManager* meshManager);
	/**
	* Updates the APC.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
	/**
	* Creates all the APC's children nodes.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void CreateChildren();
	/**
	* Returns the APC's speed.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	float GetSpeed();
	/**
	* Returns the APC's Torret Angle.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	float GetTorretAngle();
	/**
	* Returns the APC's wheel steering angle.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	float GetWheelSteer();
};