#include "quadtree.h"

QuadTree::QuadTree(float worldWidth)
{
	myRoot = new Node(worldWidth);
	myWorldWidth = worldWidth;
}

Node* QuadTree::getRoot()
{
	return myRoot;
}

float QuadTree::getWorldWidth()
{
	return myWorldWidth;
}

std::vector<Entity*> QuadTree::Concat(std::vector<Entity*> v1, std::vector<Entity*> v2)
{
	std::vector<Entity*> mergedVec;

	for(int i = 0; i < v1.size(); i++)
	{
		mergedVec.push_back(v1.at(i));
	}

	for(int i = 0; i < v2.size(); i++)
	{
		mergedVec.push_back(v2.at(i));
	}

	return mergedVec;
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

		currNode->emptyListOfEntitys();
		currNode->setChildrenInit(true);
	}
}


void QuadTree::recBuildTree(Node* parent, Node* currNode)
{
	if(parent == NULL && currNode == NULL)
	{
		return;
	}
	else if(parent!= NULL && currNode == NULL)
	{
		Vector3 temp(0.0f, 0.0f, 0.0f);
		currNode = new Node(parent->getNodeWidth() / 2.0f);
		//currNode->setID(leafIndex);
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
		InitChildren(currNode);
		return;
	}
	else
	{
		InitChildren(currNode);
		recBuildTree(currNode, currNode->getChild(0));

		InitChildren(currNode);
		recBuildTree(currNode, currNode->getChild(1));

		InitChildren(currNode);
		recBuildTree(currNode, currNode->getChild(2));

		InitChildren(currNode);
		recBuildTree(currNode, currNode->getChild(3));

	}
}

std::vector<Entity*> QuadTree::recPotentiallyVisible(Node* curNode, Frustum *frust)
{
	if(curNode != NULL)
	{
		//if node is visible
		if(frust->BoxInFrustum(curNode->getMinX(),
							   curNode->getMinZ(),
							   curNode->getMaxX(),
							   curNode->getMaxZ()))
		{
			if(curNode->getIsLeafNode())
			{
				return curNode->getLOE();
			} 
			else 
			{
				std::vector<Entity*> t0 = recPotentiallyVisible(curNode->getChild(0), frust);
				std::vector<Entity*> t1 = recPotentiallyVisible(curNode->getChild(1), frust);
				std::vector<Entity*> t2 = recPotentiallyVisible(curNode->getChild(2), frust);
				std::vector<Entity*> t3 = recPotentiallyVisible(curNode->getChild(3), frust);

				myConcatList = Concat(t0, t1);
				myConcatList = Concat(myConcatList, t2);
				myConcatList = Concat(myConcatList, t3);

				return myConcatList;
			}
		} 
		else
		{
			//not visible return nothing
			myConcatList.clear();
			return myConcatList;
		}
	}

	myConcatList.clear();
	return myConcatList;
}

std::vector<Entity*> QuadTree::getPotentiallyVisible(Frustum *frust)
{
	return QuadTree::recPotentiallyVisible(myRoot, frust);
}


void QuadTree::BuildQuadTree()
{
	myRoot->setNodeWidth(65);
	recBuildTree(myRoot, myRoot);
}