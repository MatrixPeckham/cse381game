#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "entity.h"
#include <vector>

class Node
{
private:
	Node* myChildren[4];
	std::vector<Entity*> *myListOfEntitys;
	bool myListInit;

public:
	Node::Node();

	Node* Node::getChildren();
	std::vector<Entity*> Node::getLOE();

	void Node::addEntityToNodeList(Entity* element);
	void Node::addEntityToChildNode(int nodeIndex, Entity* element);
};

#endif
