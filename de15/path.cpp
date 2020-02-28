////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// STL A* Search implementation
// (C)2001 Justin Heyes-Jones
//
// Finding a path on a simple grid maze
// This shows how to do shortest path finding using A*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "stlastar.h"

#include <iostream>
#include "path.h"
#include "player.h"
#include "map.h"
#include "deapp.h"


using namespace std;

#define NEXTSTEP 1

// Global data

// The world map
// map helper functions



BOOL GetMap( CPoint p)
{
	//CPoint p = CPoint(x,y);
	return GetApp()->GetGameMap()->CheckPosMap(p);
}

MapSearchNode::MapSearchNode( CPoint p , BOOL bMapPos) 
{ 
	m_p=p;
	if(!bMapPos)
	{
		CGameMap::SnapToGrid(m_p);
		m_p.x = m_p.x>>ICONSIZEB;
		m_p.y = m_p.y>>ICONSIZEB;
	}
}

bool MapSearchNode::IsSameState( MapSearchNode &rhs )
{

	// same state in a maze search is simply when (x,y) are the same
	if( (m_p == rhs.m_p))
	{
		return true;
	}
	else
	{
		return false;
	}

}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal. 

float MapSearchNode::GoalDistanceEstimate( MapSearchNode &nodeGoal )
{
	float xd = float( ( (float)m_p.x - (float)nodeGoal.m_p.x ) );
	float yd = float( ( (float)m_p.y - (float)nodeGoal.m_p.y) );

	return ((xd*xd) + (yd*yd));

}

bool MapSearchNode::IsGoal( MapSearchNode &nodeGoal )
{

	/*CSize size = nodeGoal.m_p - m_p;
	size.cx = abs(size.cx);
	size.cy = abs(size.cy);
	if(size.cx < SERCHAREA && size.cy < SERCHAREA)
	{
		return true;
	}*/

	if(nodeGoal.m_p == m_p)
	{
		return true;
	}

	return false;
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
void MapSearchNode::GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node )
{

	CPoint parent(-1,-1);

	if( parent_node )
	{
		parent = parent_node->m_p;
	}
	

	MapSearchNode NewNode;

	// push each possible move except allowing the search to go backwards

	//Left 
	CPoint left(m_p.x-NEXTSTEP, m_p.y);
	if( GetMap( left ) && (parent != left)) 
	{
		NewNode = MapSearchNode( left ,TRUE);
		astarsearch->AddSuccessor( NewNode );
	}	

	//Upper
	CPoint upper(m_p.x, m_p.y-NEXTSTEP);
	if( GetMap( upper ) && parent != upper)
	{
		NewNode = MapSearchNode( upper ,TRUE);
		astarsearch->AddSuccessor( NewNode );
	}	

	//Right
	CPoint right(m_p.x+NEXTSTEP, m_p.y);
	if( GetMap( right ) && parent != right)
	{
		NewNode = MapSearchNode( right ,TRUE);
		astarsearch->AddSuccessor( NewNode );
	}	

	//Lower		
	CPoint lower(m_p.x, m_p.y+NEXTSTEP);
	if( GetMap( lower ) && parent != lower)
	{
		NewNode = MapSearchNode( lower,TRUE );
		astarsearch->AddSuccessor( NewNode );
	}	

	//Upper-Left
	CPoint upperleft(m_p.x-NEXTSTEP, m_p.y-NEXTSTEP);
	if( GetMap( upperleft ) && parent != upperleft)
	{
		NewNode = MapSearchNode( upperleft ,TRUE);
		astarsearch->AddSuccessor( NewNode );
	}

	// Upper-Right
	CPoint upperright(m_p.x+NEXTSTEP, m_p.y-NEXTSTEP);
	if( GetMap( upperright ) && parent != upperright)
	{
		NewNode = MapSearchNode( upperright,TRUE );
		astarsearch->AddSuccessor( NewNode );
	}

	//Lower-Right
	CPoint lowerright(m_p.x+NEXTSTEP, m_p.y+NEXTSTEP);
	if( GetMap( lowerright ) && parent != lowerright)
	{
		NewNode = MapSearchNode( lowerright ,TRUE);
		astarsearch->AddSuccessor( NewNode );
	}

	//Lower-Left
	CPoint lowerleft(m_p.x-NEXTSTEP, m_p.y+NEXTSTEP);
	if( GetMap( lowerleft ) && parent != lowerleft)
	{
		NewNode = MapSearchNode( lowerleft,TRUE );
		astarsearch->AddSuccessor( NewNode );
	}

}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is 
// conceptually where we're moving

float MapSearchNode::GetCost( MapSearchNode &successor )
{
	return (float) GetMap(m_p)?1:9;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CPoint MapSearchNode::GetPos()
{
	CPoint p =m_p;
	p.x = p.x<<ICONSIZEB;
	p.y = p.y<<ICONSIZEB;
	CGameMap::SnapToGrid(p);
	return p;
}
