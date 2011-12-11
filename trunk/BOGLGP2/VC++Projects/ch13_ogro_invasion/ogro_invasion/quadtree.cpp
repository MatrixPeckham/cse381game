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

void QuadTree::InitChildren(Node* currNode)
{
	if(currNode->getChildrenInit() == false)
	{
		currNode->initChild(0);
		currNode->initChild(1);
		currNode->initChild(2);
		currNode->initChild(3);

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

	if((int)currNode->getNodeWidth() < 10 )
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

				t0.insert(t0.end(), t1.begin(), t1.end());
				t0.insert(t0.end(), t2.begin(), t2.end());
				t0.insert(t0.end(), t3.begin(), t3.end());

				return t0;
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
void QuadTree::recAddEntity(Entity* item, Node* currNode)
{
	if(currNode->getIsLeafNode())
	{
		currNode->addEntityToNodeList(item);
		item->setContainingNode(currNode);
	}
	else
	{
		if( item->getPosition().x< currNode->getChild(0)->getMaxX() &&
		    item->getPosition().x> currNode->getChild(0)->getMinX() &&
		    item->getPosition().z< currNode->getChild(0)->getMaxZ() &&
		    item->getPosition().z> currNode->getChild(0)->getMinZ())
		{
			QuadTree::recAddEntity(item, currNode->getChild(0));
		}
		
		if( item->getPosition().x< currNode->getChild(1)->getMaxX() &&
		    item->getPosition().x> currNode->getChild(1)->getMinX() &&
		    item->getPosition().z< currNode->getChild(1)->getMaxZ() &&
		    item->getPosition().z> currNode->getChild(1)->getMinZ())
		{
			QuadTree::recAddEntity(item, currNode->getChild(1));
		}

		if( item->getPosition().x< currNode->getChild(2)->getMaxX() &&
		    item->getPosition().x> currNode->getChild(2)->getMinX() &&
		    item->getPosition().z< currNode->getChild(2)->getMaxZ() &&
		    item->getPosition().z> currNode->getChild(2)->getMinZ())
		{
			QuadTree::recAddEntity(item, currNode->getChild(2));
		}

		if( item->getPosition().x< currNode->getChild(3)->getMaxX() &&
		    item->getPosition().x> currNode->getChild(3)->getMinX() &&
		    item->getPosition().z< currNode->getChild(3)->getMaxZ() &&
		    item->getPosition().z> currNode->getChild(3)->getMinZ())
		{
			QuadTree::recAddEntity(item, currNode->getChild(3));
		}
	}
}


void QuadTree::AddEntity(Entity* item)
{
	QuadTree::recAddEntity(item, myRoot);
}

std::vector<Entity*> QuadTree::getPotentiallyVisible(Frustum *frust)
{
	return QuadTree::recPotentiallyVisible(myRoot, frust);
}


void QuadTree::BuildQuadTree()
{
	myRoot->setNodeWidth(65);
	recBuildTree(NULL, myRoot);
}

void QuadTree::UpdateEntity(Entity* ent){
	Vector3 pos = ent->getPosition();
	Node* n = ent->getContainingNode();
	if( pos.x>n->getMaxX()||
		pos.x<n->getMinX()||
		pos.z>n->getMaxZ()||
		pos.z<n->getMinZ()){
			n->removeEntityFromNodeList(ent);
			recAddEntity(ent,getRoot());
	}
}
