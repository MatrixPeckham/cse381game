#include "quadtree.h"

QuadTree::QuadTree(float worldWidth)
{
	myRoot = new Node();
	myDepth = 5;
	myWorldWidth = worldWidth;
}

Node* QuadTree::getRoot()
{
	return myRoot;
}

void QuadTree::BuildQuadTree()
{
	float w2 = myWorldWidth / 2;
	std::vector<Entity*> entitys = myRoot->getLOE();
	Node* currNode = myRoot;
	Entity* temp;
	Vector3 tempPos;

	for(int i = 0; i < entitys.size(); i++)
	{
		temp = entitys.at(i);
		tempPos = temp->getPosition();

		if(tempPos.x <= w2 && tempPos.y >= w2)
		{
			currNode->addEntityToChildNode(0, temp);
		}
		else if(tempPos.x > w2 && tempPos.y >= w2)
		{
			currNode->addEntityToChildNode(1, temp);
		}
		else if(tempPos.x > w2 && tempPos.y < w2)
		{
			currNode->addEntityToChildNode(2, temp);
		}
		else if(tempPos.x <= w2 && tempPos.y < w2)
		{
			currNode->addEntityToChildNode(3, temp);
		}
	}
}