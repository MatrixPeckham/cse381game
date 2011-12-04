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
	int myID;
	float myNodeWidth;
	Vector3 myCenter;

public:
	Node::Node();
	Node::Node(float width);

	Node* Node::getChildren();
	std::vector<Entity*> Node::getLOE();
	float Node::getNodeWidth();
	int Node::getID();
	Vector3 Node::getCenter();

	void Node::setID(int id);
	void Node::setCenter(Vector3 center);

	void Node::addEntityToNodeList(Entity* element);
	void Node::addEntityToChildNode(int nodeIndex, Entity* element);

};

#endif
