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

Vector3 QuadTree::getCenterOfNode(int depth, int quadrant)
{
	//Since we look town down im not concerned with y
	Vector3 temp(0.0f, 0.0f, 0.0f);
	float w2 = myWorldWidth / 2.0f;

	for(int i = 0; i < depth; i++)
	{
		if(quadrant == 1)//Top Left(I)
		{
			temp.x = -w2 / 2.0f;
			temp.z = w2 / 2.0f;

			w2 = w2 / 2.0f;
		}
		else if(quadrant == 2)//Top Right(II)
		{

		}
		else if(quadrant == 3)//Bottom RIght(III)
		{

		}
		else if(quadrant == 4)//Bottom Left(IV)
		{

		}

	}

	return temp;
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

		if(tempPos.x <= 0 && tempPos.z >= 0)//I
		{
			currNode->addEntityToChildNode(0, temp);
		}
		else if(tempPos.x > 0 && tempPos.z >= 0)//II
		{
			currNode->addEntityToChildNode(1, temp);
		}
		else if(tempPos.x > 0 && tempPos.z < w2)//III
		{
			currNode->addEntityToChildNode(2, temp);
		}
		else if(tempPos.x <= w2 && tempPos.z < w2)//IV
		{
			currNode->addEntityToChildNode(3, temp);
		}
	}
}