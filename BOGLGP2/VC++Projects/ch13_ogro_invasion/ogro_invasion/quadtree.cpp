#include "quadtree.h"

QuadTree::QuadTree(float worldWidth)
{
	myRoot = new Node(worldWidth);
	myDepth = 5;
	myWorldWidth = worldWidth;
}

Node* QuadTree::getRoot()
{
	return myRoot;
}


void QuadTree::BuildQuadTree()
{
	Node* currNode = myRoot;
	float w2 = myWorldWidth / 2;

	std::vector<Entity*> entitys = currNode->getLOE();
	Vector3 currNodeCenter = currNode->getCenter();
	Entity* temp;
	Vector3 tempPos;

	for(int j = 0; j < myDepth; j++)
	{
		for(int i = 0; i < entitys.size(); i++)
		{
			temp = entitys.at(i);
			tempPos = temp->getPosition();

			if(tempPos.x <= currNodeCenter.x && tempPos.z >= currNodeCenter.z)//I
			{
				currNode->addEntityToChildNode(0, temp);
			}
			else if(tempPos.x > currNodeCenter.x && tempPos.z >= currNodeCenter.z)//II
			{
				currNode->addEntityToChildNode(1, temp);
			}
			else if(tempPos.x > currNodeCenter.x && tempPos.z < currNodeCenter.z)//III
			{
				currNode->addEntityToChildNode(2, temp);
			}
			else if(tempPos.x <= currNodeCenter.x && tempPos.z < currNodeCenter.z)//IV
			{
				currNode->addEntityToChildNode(3, temp);
			}
		}

		currNode = currNode->getChild(j % 4);
	}
}