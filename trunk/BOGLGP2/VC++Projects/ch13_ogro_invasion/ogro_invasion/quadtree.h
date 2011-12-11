#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED

#ifdef _WIN32
#include <windows.h>
#endif

#include "entity.h"
#include "frustum.h"
#include "node.h"
#include <vector>

class QuadTree
{
private:
	Node* myRoot;
	float myWorldWidth; //Width of the enviorment
	std::vector<Entity*> myConcatList;

	void QuadTree::recBuildTree(Node* parent, Node* currNode);
	void QuadTree::recAddEntity(Entity* item, Node* currNode);
	std::vector<Entity*> QuadTree::recPotentiallyVisible(Node* curNode, Frustum *frust);
	
public:
	QuadTree::QuadTree(float worldWidth);

	Node* QuadTree::getRoot();
	float QuadTree::getWorldWidth();
	std::vector<Entity*> getPotentiallyVisible(Frustum *frust);
	
	void QuadTree::InitChildren(Node* currNode);
	void QuadTree::BuildQuadTree();
	void QuadTree::AddEntity(Entity* item);
	void QuadTree::UpdateEntity(Entity* ent);
	void QuadTree::removeEntity(Entity* ent){if(ent->getContainingNode())ent->getContainingNode()->removeEntityFromNodeList(ent);}
};

#endif
