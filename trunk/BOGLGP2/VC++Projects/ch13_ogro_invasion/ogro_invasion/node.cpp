#include "node.h"

Node::Node()
{
	for(int i = 0; i < 4; i++)
	{
		myChildren[i] = NULL;
	}

	myListOfEntitys = new std::vector<Entity*>();
	myListInit = false;
	myNodeWidth = -99;
	myID = -99;
}

Node::Node(float width)
{
	for(int i = 0; i < 4; i++)
	{
		myChildren[i] = NULL;
	}

	myListOfEntitys = new std::vector<Entity*>();
	myListInit = false;
	myNodeWidth = width;
	myID = -99;
}

Node* Node::getChildren()
{
	return *myChildren;
}

int Node::getID()
{
	return myID;
}

std::vector<Entity*> Node::getLOE()
{
	return *myListOfEntitys;
}

Vector3 Node::getCenter()
{
	return myCenter;
}

float Node::getNodeWidth()
{
	return myNodeWidth;
}

void Node::setID(int id)
{
	myID = id;
}

void Node::setCenter(Vector3 center)
{
	myCenter = center;
}

void Node::addEntityToNodeList(Entity* element)
{
	if(myListInit = false)
	{
		myListOfEntitys = new std::vector<Entity*>();
		myListInit = true;
	}

	myListOfEntitys->push_back(element);
}

void Node::addEntityToChildNode(int nodeIndex, Entity* element)
{
	if(myChildren[nodeIndex] == NULL)
	{
		Vector3 temp(0.0f, 0.0f, 0.0f);
		myChildren[nodeIndex] = new Node(myNodeWidth / 2.0f);
		myChildren[nodeIndex]->setID(nodeIndex);

		if(nodeIndex == 0)
		{
			temp.x = -myChildren[0]->getNodeWidth() / 2.0f;
			temp.z = myChildren[0]->getNodeWidth() / 2.0f;
			
			myChildren[0]->setCenter(temp);					     
		}
		else if(nodeIndex == 1)
		{
			temp.x = -myChildren[0]->getNodeWidth() / 2.0f;
			temp.z = myChildren[0]->getNodeWidth() / 2.0f;
			
			myChildren[0]->setCenter(temp);					     
		}
		else if(nodeIndex == 2)
		{
			temp.x = -myChildren[0]->getNodeWidth() / 2.0f;
			temp.z = myChildren[0]->getNodeWidth() / 2.0f;
			
			myChildren[0]->setCenter(temp);					     
		}
		else if(nodeIndex == 3)
		{
			temp.x = -myChildren[0]->getNodeWidth() / 2.0f;
			temp.z = myChildren[0]->getNodeWidth() / 2.0f;
			
			myChildren[0]->setCenter(temp);					     
		}
	}

	myChildren[nodeIndex]->addEntityToNodeList(element);
}
