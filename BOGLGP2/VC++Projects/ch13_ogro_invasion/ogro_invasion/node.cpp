#include "node.h"

Node::Node()
{
	myListOfEntitys = new std::vector<Entity*>();
	myListInit = false;
}

Node* Node::getChildren()
{
	return *myChildren;
}


std::vector<Entity*> Node::getLOE()
{
	return *myListOfEntitys;
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
	//if(myChildren[nodeIndex] == NULL)
	//{
	//	myChildren[nodeIndex] = new Node();
	//}

	myChildren[nodeIndex]->addEntityToNodeList(element);
}
