#include "SceneManager.h"
#include "SceneNodes.h"
#include <cassert>

//base class
SceneNode::SceneNode()
{
	typeId = sceneNodeType::BASE;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

SceneNode::~SceneNode()
{
	//let's unhook from our parent, if they are valid
	if(parent != NULL)
	{
		for(unsigned int i = 0; i < parent->children.size(); ++i)
		{
			if(this == parent->children[i])
			{
				parent->children.erase(parent->children.begin() + i);
				break;
			}
		}
	}
}

void SceneNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");
	//get modelMatrix from our parent
	snodeMutex.lock();
	parent->snodeMutex.lock();
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();
	snodeMutex.unlock();

	//update all of our children
	for(auto n : children)
	{
		n->Update(seconds);
	}
}

void SceneNode::Draw()
{
	//draw all of our children
	for(auto n : children)
	{
		n->Draw();
	}
}

RootNode::RootNode()
{
	typeId = sceneNodeType::ROOT;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

void RootNode::Update(double seconds)
{
	//the root node does not modify or inherit a modelMatrix
	
	//update all of our children
	for(auto n : children)
	{
		n->Update(seconds);
	}
}

SceneManager::SceneManager()
{
	sceneGraph = (SceneNode *)new RootNode();
}

SceneManager::~SceneManager()
{
	//delete the entire scenegraph
	KillNodeAndChildren(sceneGraph);
}

void SceneManager::UpdateWorld(double seconds)
{
	assert(sceneGraph != NULL && "sceneGraph is NULL");
	sceneGraph->Update(seconds);
}

void SceneManager::DrawWorld()
{
	assert(sceneGraph != NULL && "sceneGraph is NULL");
	sceneGraph->Draw();
}

// Function to recursively add all nodes on the scenegraph to a vector
// This is an example of how to process every node on a graph recursively
void SceneManager::AddAllNodesToList(SceneNode* currentNode, std::vector<SceneNode *> *theList)
{
	theList->push_back(currentNode);

	for(auto n : currentNode->children)
		AddAllNodesToList(n, theList);
}

void SceneManager::KillNodeAndChildren(SceneNode *node)
{
	std::vector<SceneNode *> DeleteList;

	AddAllNodesToList(node, &DeleteList);

	for(auto n : DeleteList)
	{
		//since we have auto dynamic typing, we can just delete the objects:
		delete n;
		n = NULL;
		/* if we didn't have auto type, like say we were looping with an index counter,
		we could do the following instead:
		switch(DeleteList[i]->typeId)
		{
		case sceneNodeType::BASE:
			delete DeleteList[i];
			break;

		case sceneNodeType::ROOT:
			delete (RootNode *) DeleteList[i];
			break;

		case sceneNodeType::TRANSLATE:
			delete (TranslaterNode *)DeleteList[i];
			break;

		//...etc.

		default:
			assert(false && "Unknown scene node type");
			//we don't know how to handle this object!
			break;
		}//end switch 
		*/
	}

	DeleteList.clear();
	node = NULL;
}
