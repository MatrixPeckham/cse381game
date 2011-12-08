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
	int myTempNode; //used to go back up the tree
	float myNodeWidth;
	Vector3 myCenter;

public:
	Node::Node();
	Node::Node(float width);

	Node* Node::getChild(int index);
	Node* Node::getParent();
	std::vector<Entity*> Node::getLOE();
	float Node::getNodeWidth();
	int Node::getID();
	int Node::getTempNode();
	Vector3 Node::getCenter();
	bool Node::getChildrenInit();

	void Node::setID(int id);
	void Node::setCenter(Vector3 center);
	void Node::setTempNode(int amt);
	void Node::setNodeWidth(float amt);
	void Node::setParent(Node* parent);
	void Node::setChildrenInit(bool amt);

	void Node::addEntityToNodeList(Entity* element);
	void Node::addEntityToChildNode(int nodeIndex, Entity* element);
	void Node::emptyListOfEntitys();

};

#endif
