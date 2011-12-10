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
	myID = -99;//if -99 then its the root

	myMinX = -64.0f;
	myMinZ = -64.0f;
	myMaxX = 64.0f;
	myMaxZ = 64.0f;
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

float Node::getMinX()
{
	return myMinX;
}

float Node::getMinZ()
{
	return myMinZ;
}

float Node::getMaxX()
{
	return myMaxX;
}

float Node::getMaxZ()
{
	return myMaxZ;
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

void Node::setMinX(float minX)
{
	myMinX = minX;
}

void Node::setMinZ(float minZ)
{
	myMinZ = minZ;
}

void Node::setMaxX(float maxX)
{
	myMaxX = maxX;
}

void Node::setMaxZ(float maxZ)
{
	myMaxZ = maxZ;
}

void Node::emptyListOfEntitys()
{
	while(!myListOfEntitys->empty())
	{
		myListOfEntitys->pop_back();
	}

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

bool Node::getIsLeafNode()
{
	for(int i = 0; i < 4; i++)
	{
		if(myChildren[i] != NULL)
		{
			return false;
		}
	}

	return true;
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

			myChildren[0]->setMinX(temp.x - (myChildren[0]->getNodeWidth() / 2));
			myChildren[0]->setMinZ(temp.z - (myChildren[0]->getNodeWidth() / 2));
			myChildren[0]->setMaxX(temp.x + (myChildren[0]->getNodeWidth() / 2));
			myChildren[0]->setMaxZ(temp.z + (myChildren[0]->getNodeWidth() / 2));
		}
		else if(nodeIndex == 1)
		{
			temp.x = myChildren[1]->getNodeWidth() / 2.0f;
			temp.z = myChildren[1]->getNodeWidth() / 2.0f;
			
			myChildren[1]->setCenter(temp);	

			myChildren[1]->setMinX(temp.x - (myChildren[1]->getNodeWidth() / 2));
			myChildren[1]->setMinZ(temp.z - (myChildren[1]->getNodeWidth() / 2));
			myChildren[1]->setMaxX(temp.x + (myChildren[1]->getNodeWidth() / 2));
			myChildren[1]->setMaxZ(temp.z + (myChildren[1]->getNodeWidth() / 2));
		}
		else if(nodeIndex == 2)
		{
			temp.x = myChildren[2]->getNodeWidth() / 2.0f;
			temp.z = -myChildren[2]->getNodeWidth() / 2.0f;
			
			myChildren[2]->setCenter(temp);

			myChildren[2]->setMinX(temp.x - (myChildren[2]->getNodeWidth() / 2));
			myChildren[2]->setMinZ(temp.z - (myChildren[2]->getNodeWidth() / 2));
			myChildren[2]->setMaxX(temp.x + (myChildren[2]->getNodeWidth() / 2));
			myChildren[2]->setMaxZ(temp.z + (myChildren[2]->getNodeWidth() / 2));
		}
		else if(nodeIndex == 3)
		{
			temp.x = -myChildren[3]->getNodeWidth() / 2.0f;
			temp.z = -myChildren[3]->getNodeWidth() / 2.0f;
			
			myChildren[3]->setCenter(temp);	

			myChildren[3]->setMinX(temp.x - (myChildren[3]->getNodeWidth() / 2));
			myChildren[3]->setMinZ(temp.z - (myChildren[3]->getNodeWidth() / 2));
			myChildren[3]->setMaxX(temp.x + (myChildren[3]->getNodeWidth() / 2));
			myChildren[3]->setMaxZ(temp.z + (myChildren[3]->getNodeWidth() / 2));
		}
	}

	myChildren[nodeIndex]->addEntityToNodeList(element);
}
