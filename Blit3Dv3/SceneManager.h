#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <mutex>

//this enum is optional, since we can use dynamic typing to free memory of derived classes,
//but maybe we wwant to check node types for other reasons, so I leave it as an example.
enum class sceneNodeType { BASE = 1, ROOT, TRANSLATE, PRINT, MESH, ROTATE, APC };

//base class
class SceneNode
{
public:
	glm::mat4 modelMatrix; //every node needs a modelMatrix, so that Update() can inherit transformations
	SceneNode *parent; //we use parent pointer to "inherit" our parent's transformations
	std::vector<SceneNode *> children;
	sceneNodeType typeId; //not technically necessary, but can be useful
	std::mutex snodeMutex; //for thread safety 

	//default virtual methods: all nodes must support Update() and Draw(),
	//but if they aren't doing anything special with one or the other they can just use the default
	virtual void Update(double seconds);
	virtual void Draw();

	SceneNode();
	virtual ~SceneNode();
};

//needed for scene graph root node, which has no parent
class RootNode : public SceneNode
{
public:
	//we have to override Update() since we have no parent
	void Update(double seconds);
	RootNode();
};

//class to manage our scene graph
class SceneManager
{
public:
	SceneNode *sceneGraph; //our scene graph root node

	//our Update() and Draw() methods should be thread safe
	void UpdateWorld(double seconds);
	void DrawWorld();
	//helper method that adds node and all children to a flat list (flattens the scene graph tree)
	void AddAllNodesToList(SceneNode* currentNode, std::vector<SceneNode *> *theList);
	//delete nodes and their children, and remove the node from it's parent's list of children
	void KillNodeAndChildren(SceneNode *node);
	SceneManager();
	~SceneManager();
};