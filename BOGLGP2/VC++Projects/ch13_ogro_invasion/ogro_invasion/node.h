#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "entity.h"
#include <vector>

class Node
{
private:
	Node* myParent; //because recusrion sucks
	Node* myChildren[4];
	std::vector<Entity*> *myListOfEntitys;

	bool myListInit;
	bool myChildrenInit;
	int myID;
	float myNodeWidth;
	Vector3 myCenter;

	float myMinX;
	float myMinZ;
	float myMaxX;
	float myMaxZ;

public:
	Node::Node();
	Node::Node(float width);

	Node* Node::getChild(int index);
	Node* Node::getParent();
	std::vector<Entity*> Node::getLOE();
	float Node::getNodeWidth();
	float Node::getMinX();
	float Node::getMinZ();
	float Node::getMaxX();
	float Node::getMaxZ();

	int Node::getID();
	Vector3 Node::getCenter();
	bool Node::getChildrenInit();
	bool Node::getIsLeafNode();

	void Node::setID(int id);
	void Node::setCenter(Vector3 center);
	void Node::setNodeWidth(float amt);
	void Node::setParent(Node* parent);
	void Node::setChildrenInit(bool amt);
	void Node::setMinX(float minX);
	void Node::setMinZ(float minZ);
	void Node::setMaxX(float maxX);
	void Node::setMaxZ(float maxZ);

	void Node::addEntityToNodeList(Entity* element);
	void Node::initChild(int nodeIndex);
	void Node::emptyListOfEntitys();

};

#endif
