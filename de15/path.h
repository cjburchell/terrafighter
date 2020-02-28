////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// STL A* Search implementation
// (C)2001 Justin Heyes-Jones
//
// Finding a path on a simple grid maze
// This shows how to do shortest path finding using A*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __PATH_H
#define __PATH_H

#include <iostream>
#include "stlastar.h"



using namespace std;

// Definitions

class MapSearchNode
{
private:
	CPoint m_p;
public:
	CPoint GetPos();
	
	MapSearchNode() { m_p = CPoint(0,0);}
	MapSearchNode( CPoint p , BOOL bMapPos = FALSE);
	MapSearchNode( int x, int y ) { m_p=CPoint(x,y);}

	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
	void GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
	float GetCost( MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );


};

#endif