#include "quadtree.h"

QuadTree::QuadTree(float worldWidth)
{
	myRoot = new Node(worldWidth);
	myDepth = 16;
	myWorldWidth = worldWidth;
}

Node* QuadTree::getRoot()
{
	return myRoot;
}
void QuadTree::InitChildren(Node* currNode)
{
	Vector3 currNodeCenter = currNode->getCenter();
	Entity* temp;
	Vector3 tempPos(0.0f, 0.0f, 0.0f);

	if(currNode->getChildrenInit() == false)
	{
		for(int i = 0; i < currNode->getLOE().size(); i++)
		{
			temp = currNode->getLOE().at(i);
			currNode->getLOE().pop_back();

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
		
		currNode->setChildrenInit(true);
	}
}

void QuadTree::recBuildTree(Node* parent, Node* currNode, int leafIndex)
{
	if(parent == NULL && currNode == NULL)
	{
		return;
	}
	else if(parent!= NULL && currNode == NULL)
	{
		Vector3 temp(0.0f, 0.0f, 0.0f);
		currNode = new Node(parent->getNodeWidth() / 2.0f);
		currNode->setID(leafIndex);
		currNode->setParent(parent);

		//calculate center for each child
		if(currNode->getID() == 0)
		{
			temp.x = -currNode->getNodeWidth() / 2.0f;
			temp.z = currNode->getNodeWidth() / 2.0f;
			
			currNode->setCenter(temp);					     
		}
		else if(currNode->getID() == 1)
		{
			temp.x = currNode->getNodeWidth() / 2.0f;
			temp.z = currNode->getNodeWidth() / 2.0f;
			
			currNode->setCenter(temp);					     
		}
		else if(currNode->getID() == 2)
		{
			temp.x = currNode->getNodeWidth() / 2.0f;
			temp.z = -currNode->getNodeWidth() / 2.0f;
			
			currNode->setCenter(temp);					     
		}
		else if(currNode->getID() == 3)
		{
			temp.x = -currNode->getNodeWidth() / 2.0f;
			temp.z = -currNode->getNodeWidth() / 2.0f;
			
			currNode->setCenter(temp);					     
		}
	}

	if((int)currNode->getNodeWidth() == 0 || currNode->getTempNode() >= 4)
	{
		//parent->setTempNode(parent->getTempNode() + 1);
		//recBuildTree(parent->getParent(), parent);
		InitChildren(currNode);
		return;
	}
	else
	{
		InitChildren(currNode);
		recBuildTree(currNode, currNode->getChild(0), 0);

		InitChildren(currNode);
		recBuildTree(currNode, currNode->getChild(1), 1);

		InitChildren(currNode);
		recBuildTree(currNode, currNode->getChild(2), 2);

		InitChildren(currNode);
		recBuildTree(currNode, currNode->getChild(3), 3);
		
	}
}

void QuadTree::BuildQuadTree()
{
	myRoot->setNodeWidth(65);
	recBuildTree(myRoot, myRoot,0);
	//recBuildTree(myRoot, myRoot,1);
	//recBuildTree(myRoot, myRoot,2);
	//recBuildTree(myRoot, myRoot,3);
}