#include "node.h"

Node::Node()
{
	for(int i = 0; i < 4; i++)
	{
		myChildren[i] = NULL;
	}

	myListOfEntitys = new std::vector<Entity*>();
	myListInit = false;
	myNodeWidth = -1;
	myID = -1;
}

Node::Node(float width)
{
	myParent = NULL;

	for(int i = 0; i < 4; i++)
	{
		myChildren[i] = NULL;
	}
	
	myListOfEntitys = new std::vector<Entity*>();
	myListInit = false;
	myChildrenInit = false;
	myNodeWidth = width;
	myCenter.x = 0.0f;
	myCenter.y = 0.0f;
	myCenter.z = 0.0f;
	myTempNode = 0;
	myID = 0;//if -99 then its the root
}

Node* Node::getChild(int index)
{
	return myChildren[index];
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

int Node::getTempNode()
{
	return myTempNode;
}

Node* Node::getParent()
{
	return myParent;
}

bool Node::getChildrenInit()
{
	return myChildrenInit;
}

void Node::setID(int id)
{
	myID = id;
}

void Node::setCenter(Vector3 center)
{
	myCenter = center;
}

void Node::setTempNode(int amt)
{
	myTempNode = amt;
}

void Node::setNodeWidth(float amt)
{
	myNodeWidth = amt;
}

void Node::setParent(Node* parent)
{
	myParent = parent;
}

void Node::setChildrenInit(bool amt)
{
	myChildrenInit = amt;
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
		myChildren[nodeIndex]->setParent(this);

		//calculate center for each child
		if(nodeIndex == 0)
		{
			temp.x = -myChildren[0]->getNodeWidth() / 2.0f;
			temp.z = myChildren[0]->getNodeWidth() / 2.0f;
			
			myChildren[0]->setCenter(temp);					     
		}
		else if(nodeIndex == 1)
		{
			temp.x = myChildren[1]->getNodeWidth() / 2.0f;
			temp.z = myChildren[1]->getNodeWidth() / 2.0f;
			
			myChildren[1]->setCenter(temp);					     
		}
		else if(nodeIndex == 2)
		{
			temp.x = myChildren[2]->getNodeWidth() / 2.0f;
			temp.z = -myChildren[2]->getNodeWidth() / 2.0f;
			
			myChildren[2]->setCenter(temp);					     
		}
		else if(nodeIndex == 3)
		{
			temp.x = -myChildren[3]->getNodeWidth() / 2.0f;
			temp.z = -myChildren[3]->getNodeWidth() / 2.0f;
			
			myChildren[3]->setCenter(temp);					     
		}
	}

	myChildren[nodeIndex]->addEntityToNodeList(element);
}
