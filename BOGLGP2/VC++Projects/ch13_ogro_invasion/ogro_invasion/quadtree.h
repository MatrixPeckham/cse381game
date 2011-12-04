#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED

#ifdef _WIN32
#include <windows.h>
#endif

#include "entity.h"
#include "node.h"
#include <list>


class QuadTree
{
private:
	Node* myRoot;
	int myDepth; //How far we will continue to split our enviorment	
	float myWorldWidth; //Width of the enviorment

public:
	QuadTree::QuadTree(float worldWidth);

	Node* QuadTree::getRoot();
	void QuadTree::BuildQuadTree();

};

#endif
