#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "APCNode.h"
#include "HullNode.h"
#include "WheelNode.h"
#include "FrontWheelNode.h"
#include "TorretNode.h"

APCNode::APCNode()
{
	// Set the default angles
	typeId = sceneNodeType::APC;
	position = vec3(0.f, 0.f, 0.f);
	speed = 0.f;
	angle = 0.f;
	torretAngle = 0.f;
	gunAngle = 2.f;
	modelMatrix = glm::mat4(1.f);
	steering = false;
	left = false;
	moving = false;
	forward = false;
	movingTorret = false;
	torretAngleLeft = false;
	movingGun = false;
	gunAngleUp = false;
	parent = NULL;

}

APCNode::APCNode(SceneNode* ParentNode, MeshManager* meshManager)
{
	// Set the default angles
	typeId = sceneNodeType::APC;
	mManager = meshManager;
	modelMatrix = glm::mat4(1.f);
	position = vec3(0.f, 0.f, 0.f);
	speed = 0.f;
	angle = 0.f;
	torretAngle = 0.f;
	wheelSteer = 0.f;
	gunAngle = 2.f;
	steering = false;
	left = false;
	moving = false;
	forward = false;
	movingTorret = false;
	torretAngleLeft = false;
	movingGun = false;
	gunAngleUp = false;
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
	CreateChildren();
}
	
void APCNode::Update(double seconds)
{
	// Make sure this node has a parent.
	assert(parent != NULL && "parent pointer is NULL");
	// Handle the wheel steering
	if (this->steering)
	{
		this->wheelSteer += (this->left ? STEER_DELTA : -STEER_DELTA) * seconds;
		if (this->wheelSteer > TOP_STEER)
		{
			this->wheelSteer = TOP_STEER;
		}
		if (this->wheelSteer < -TOP_STEER)
		{
			this->wheelSteer = -TOP_STEER;
		}
	}
	else
	{
		// Handle when it is also moving, so the steering goes back to normal
		if (this->speed != 0)
		{
			if (this->wheelSteer > 0.f)
			{
				this->wheelSteer -= STEER_DELTA * abs(this->speed) / RETURN_WHEEL_VALUE;
				if (this->wheelSteer < 0.f)
				{
					this->wheelSteer = 0.f;
				}
			}
			if (this->wheelSteer < 0.f)
			{
				this->wheelSteer += STEER_DELTA * abs(this->speed) / RETURN_WHEEL_VALUE;
				if (this->wheelSteer > 0.f)
				{
					this->wheelSteer = 0.f;
				}
			}
		}
	}
	// Handle when the tank is moving
	if (this->moving)
	{
		this->speed += (this->forward ? SPEED_DELTA : -SPEED_DELTA) * seconds;
		if (this->speed > MAX_SPEED)
		{
			this->speed = MAX_SPEED;
		}
		if (this->speed < -MAX_SPEED / 2)
		{
			this->speed = -MAX_SPEED / 2;
		}
	}
	// Reduce the speed if its not moving
	else {
		if (this->speed != 0.f)
		{
			if (this->speed > 0.f)
			{
				this->speed -= FRICTION * seconds;
				if (this->speed < 0.f)
				{
					this->speed = 0.f;
				}
			}
			else
			{
				this->speed += FRICTION * seconds;
				if (this->speed > 0.f)
				{
					this->speed = 0.f;
				}
			}
		}
	}
	// Handle when moving the torret
	if (this->movingTorret)
	{
		this->torretAngle += (this->torretAngleLeft ? TORRET_DELTA : -TORRET_DELTA) * seconds;
		if (this->torretAngle > 360.f)
		{
			this->torretAngle -= 360.f;
		}
		if (this->torretAngle < 360.f)
		{
			this->torretAngle += 360.f;
		}
	}
	// Handle when moving the gun
	if (this->movingGun)
	{
		this->gunAngle += (this->gunAngleUp ? -GUN_DELTA : GUN_DELTA) * seconds;
		if (this->gunAngle > MAX_GUN_ANGLE)
		{
			this->gunAngle = MAX_GUN_ANGLE;
		}
		if (this->gunAngle < MIN_GUN_ANGLE)
		{
			this->gunAngle = MIN_GUN_ANGLE;
		}
	}

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	// Get the wheel steer angle
	float radians = this->wheelSteer * (M_PI / 180);

	// Modify the angle accordingly
	this->angle += TIRE_TRACTION * radians * seconds * (this->speed / MAX_SPEED);
	
	// Calculate the vector where the tank is facing
	glm::vec3 facing;
	facing.x = -std::cos(this->angle);
	facing.z = std::sin(this->angle);

	// Updating the tank position
	this->position += facing * (this->speed / MOVEMENT_CAP);

	//translate
	modelMatrix = glm::translate(modelMatrix, this->position);
	// rotate
	modelMatrix = glm::rotate(modelMatrix, this->angle, vec3(0.f, 1.f, 0.f));
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

void APCNode::CreateChildren()
{
	// Add all the APC's elements
	HullNode* topHull = new HullNode(this, "Data/HullTop.s3d", glm::vec3(0.f,0.f,0.f), mManager);
	HullNode* bottomHull = new HullNode(this, "Data/HullBottom.s3d", glm::vec3(0.45f, -1.f, 0.f), mManager);
	WheelNode* frontLeftWheel = new WheelNode(this, "Data/Wheel.s3d",glm::vec3(-1.1f, -1.f, -0.8f), false, true, mManager);
	WheelNode* frontRightWheel = new WheelNode(this, "Data/Wheel.s3d", glm::vec3(-1.1f, -1.f, 0.8f), true, true, mManager);
	WheelNode* backLeftWheel = new WheelNode(this, "Data/Wheel.s3d", glm::vec3(1.9f, -1.f, -0.8f), false, false, mManager);
	WheelNode* backRightWheel = new WheelNode(this, "Data/Wheel.s3d", glm::vec3(1.9f, -1.f, 0.8f), true, false, mManager);
	TorretNode* torret = new TorretNode(this, "Data/Torret.s3d", glm::vec3(0.f, 0.5f, 0.f), mManager);
}

float APCNode::GetSpeed()
{
	return this->speed;
}

float APCNode::GetTorretAngle()
{
	return this->torretAngle;
}

float APCNode::GetWheelSteer()
{
	return this->wheelSteer;
}
