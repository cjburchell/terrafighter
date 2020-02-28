#include <afxwin.h>
#include "player.h"
#include "path.h"

#define PATHSERCHSIZE 16
#define RANGESERCH PATHSERCHSIZE

void CPath::CreatePath(CPoint src, CPoint dest, CGameMap* pMap)
{     

	//src.y = src.y/ICONSIZE;
	//src.x = src.x/ICONSIZE;
	//dest.y = dest.y/ICONSIZE;
	//dest.x = dest.x/ICONSIZE;
    m_pBestNode = FindPath(src,dest,pMap);
	if(m_pBestNode)
	{
		CNode* first = m_pBestNode->Parent;
		CNode* next = m_pBestNode;
		if(first)
		{
			while(first->Parent)
			{
				next = first;
				first = first->Parent;
			}
			if(next)
				next->Parent=NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// Algorithm to find shortest path (A*)
/////////////////////////////////////////////////////////////////////////

BOOL CPath::FreeTile(CPoint p, CGameMap* pMap)   // returns 1 if tile is free(nothing on it).
{
    return pMap->CheckPos(p);
    // Check if this location's terrain is impassable to this army.
}

// A* Algorithm
CNode* CPath::FindPath(CPoint s, CPoint d, CGameMap* pMap)
{
    
    
    CNode *Node, *BestNode;
    UINT deltaX, deltaY;
    CleanUpPath();
    
    OPEN= new CNode;
    CLOSED= new CNode;
    // Setup the Stack.
    Stack= new CPathStack;

    Node= new CNode;
    Node->g = 0;    
    deltaX = abs(s.x-d.x);
    deltaY = abs(s.y-d.y);
    Node->h = max(deltaX, deltaY);
    Node->f = Node->g + Node->h;  
    Node->p = s;

    OPEN->NextNode = Node;        // make Open List point to first node 
    for (;;)
    {
        BestNode = ReturnBestNode();
		if(!BestNode)
			break;
        // if we've found the end, break and finish 
		CSize size = d - BestNode->p;
		size.cx = abs(size.cx);
		size.cy = abs(size.cy);
		if(size.cx < RANGESERCH && size.cy < RANGESERCH)
			break;
        GenerateSuccessors(BestNode, d,pMap);
    }
    return (BestNode);
}

CNode* CPath::ReturnBestNode(void)
{
    lReturnBestNode += 1;
    CNode *tmp;

    if (OPEN->NextNode == NULL)
    {
        //AfxMessageBox("ERROR: no more nodes on OPEN\n");
        //ASSERT(FALSE);
        return NULL;
    }

// Pick Node with lowest f, in this case it's the first node in list
//  because we sort the OPEN list wrt lowest f. Call it BESTNODE. 

    tmp = OPEN->NextNode;   // point to first node on OPEN
    OPEN->NextNode = tmp->NextNode;    // Make OPEN point to nextnode or NULL.

// Next take BESTNODE (or temp in this case) and put it on CLOSED

    tmp->NextNode = CLOSED->NextNode;
    CLOSED->NextNode = tmp;

    return(tmp);
}


void CPath::GenerateSuccessors(CNode *BestNode, CPoint d,CGameMap* pMap)
{
    lGenerateSuccessors += 1;

		    // Upper-Left
	CPoint p;
    if (FreeTile(p = CPoint(BestNode->p.x - PATHSERCHSIZE,BestNode->p.y - PATHSERCHSIZE),pMap))
        GenerateSucc(BestNode, p, d);
		    // Upper
    if (FreeTile(p = CPoint(BestNode->p.x, BestNode->p.y - PATHSERCHSIZE),pMap))
        GenerateSucc(BestNode, p, d);
		    // Upper-Right
    if (FreeTile(p = CPoint(BestNode->p.x + PATHSERCHSIZE, BestNode->p.y - PATHSERCHSIZE),pMap))
        GenerateSucc(BestNode, p, d);
		    // Right
    if (FreeTile(p = CPoint(BestNode->p.x + PATHSERCHSIZE, BestNode->p.y),pMap))
        GenerateSucc(BestNode, p, d);
		    // Lower-Right
    if (FreeTile(p = CPoint(BestNode->p.x + PATHSERCHSIZE, BestNode->p.y + PATHSERCHSIZE),pMap))
        GenerateSucc(BestNode, p, d);
		    // Lower
    if (FreeTile(p = CPoint(BestNode->p.x , BestNode->p.y + PATHSERCHSIZE),pMap))
        GenerateSucc(BestNode, p, d);
		    // Lower-Left
    if (FreeTile(p = CPoint(BestNode->p.x - PATHSERCHSIZE, BestNode->p.y + PATHSERCHSIZE),pMap))
        GenerateSucc(BestNode, p, d);
		    // Left
    if (FreeTile(p = CPoint(BestNode->p.x - PATHSERCHSIZE, BestNode->p.y),pMap))
        GenerateSucc(BestNode, p, d);
}

void CPath::GenerateSucc(CNode *BestNode, CPoint p, CPoint d)
{
    lGenerateSucc += 1;
    UINT g;  // total path cost - as stored in the linked lists.
    int c = 0;
    UINT nTerrainCost;   // Movement cost of this terrain.
    UINT deltaX, deltaY;
    CNode *Old, *Successor;

    // g(Successor)=g(BestNode)+cost of getting from BestNode to Successor 
    nTerrainCost = 1; //m_nMovementPenalty[m_pMap->GetLocation(x, y)->GetTerrainType()];

    g = BestNode->g + nTerrainCost;    

    if ((Old = CheckOPEN(p)) != NULL) // if equal to NULL then not in OPEN list, else it returns the Node in Old 
    {
    	for(c = 0; c < MAX_CHILD_NODES; c++)
    	    if(BestNode->Child[c] == NULL) // Add Old to the list of BestNode's Children (or Successors). 
    	        break;
    	BestNode->Child[c] = Old;

    	if (g < Old->g)  // if our new g value is < Old's then reset Old's parent to point to BestNode
    	{
    	    Old->Parent = BestNode;
    	    Old->g = g;
    	    Old->f = g + Old->h;
    	}
    }
    else if ((Old = CheckCLOSED(p)) != NULL) // if equal to NULL then not in OPEN list, else it returns the Node in Old
    {
        for(c = 0; c < MAX_CHILD_NODES; c++)
	        if (BestNode->Child[c] == NULL) // Add Old to the list of BestNode's Children (or Successors).
	            break;
	    BestNode->Child[c] = Old;

    	if (g < Old->g)  // if our new g value is < Old's then reset Old's parent to point to BestNode
    	{
    	    Old->Parent = BestNode;
    	    Old->g = g;
    	    Old->f = g + Old->h;
    	    PropagateDown(Old);       // Since we changed the g value of Old, we need
    					 // to propagate this new value downwards, i.e.
    					 // do a Depth-First traversal of the tree! 
    	}
    }
    else
    {
        Successor = new CNode;
        Successor->Parent = BestNode;
        Successor->g = g;    	
        deltaX = abs(p.x-d.x);
        deltaY = abs(p.y-d.y);
        Successor->h = max(deltaX, deltaY);
        Successor->f = g + Successor->h;
        Successor->p = p;

        Insert(Successor);     // Insert Successor on OPEN list wrt f
        for(c = 0; c < MAX_CHILD_NODES; c++)
            if (BestNode->Child[c] == NULL) // Add Old to the list of BestNode's Children (or Successors).
    	  break;
        BestNode->Child[c] = Successor;
    }
}

CNode* CPath::CheckOPEN(CPoint p)
{
    lCheckOpen += 1;
    CNode *tmp;

    tmp = OPEN->NextNode;

    while (tmp != NULL)
    {
    	lOpenLoop += 1;
    	if (tmp->p == p)
    	    return (tmp);
    	else
    	    tmp = tmp->NextNode;
    }
    return (NULL);
}

CNode* CPath::CheckCLOSED(CPoint p)
{
    lCheckClosed += 1;
    CNode *tmp;

    tmp = CLOSED->NextNode;

    while (tmp != NULL)
    {
    	lClosedLoop += 1;
    	if (tmp->p==p)
    	    return (tmp);
    	else
    	    tmp = tmp->NextNode;
    }
    return (NULL);
}

void CPath::Insert(CNode *Successor)
{
    lInsert += 1;
    CNode *tmp1, *tmp2;
    long f;

    if (OPEN->NextNode == NULL)
    {
	    OPEN->NextNode = Successor;
	    return;
    }

    // insert into OPEN successor wrt f
    f = Successor->f;
    tmp1 = OPEN;
    tmp2 = OPEN->NextNode;

    while ((tmp2 != NULL) && (tmp2->f < f))
    {
        lInsertLoop += 1;
        tmp1 = tmp2;
        tmp2 = tmp2->NextNode;
    }
	Successor->NextNode = tmp2;
	tmp1->NextNode = Successor;
}

void CPath::PropagateDown(CNode *Old)
{
    lPropagateDown += 1;
    int     c;
    UINT    g;
    UINT    nTerrainCost;
    CNode *Child, *Father;    

    g = Old->g;            // alias.
    for(c = 0; c < MAX_CHILD_NODES; c++)
    {
        Child = Old->Child[c];  // Create alias for faster access.
        if (Child == NULL)
            break;
        nTerrainCost = 1;// m_nMovementPenalty[m_pMap->GetLocation(Child->x, Child->y)->GetTerrainType()];        
        if (g + nTerrainCost < Child->g)
        {
            Child->g = g + nTerrainCost;
            Child->f = Child->g + Child->h;
            Child->Parent = Old;          // reset parent to new path.
            Push(Child);                  // Now the Child's branch need to be
        }     // checked out. Remember the new cost must be propagated down.
    }

    while (Stack->NextStackPtr != NULL)
    {
    	lPropLoop += 1;
    	Father = Pop();
    	for(c = 0; c < MAX_CHILD_NODES; c++)
    	{
    	    Child = Father->Child[c];
            if (Child == NULL)       // we may stop the propagation 2 ways: either
    	        break;
            nTerrainCost = 1;//m_nMovementPenalty[m_pMap->GetLocation(Child->x, Child->y)->GetTerrainType()];    	    
    	    if (Father->g + nTerrainCost < Child->g) // there are no children, or that the g value of
    	    {                           // the child is equal or better than the cost we're propagating
                        Child->g = Father->g + nTerrainCost;
                        Child->f = Child->g + Child->h;
    	        Child->Parent = Father;
    	        Push(Child);
    	    }
    	}
    }
}

///////////////////////////////////////////////////////////////////////////
// STACK Functions
///////////////////////////////////////////////////////////////////////////

void CPath::Push(CNode *Node)
{
    lPush += 1;
    CPathStack *tmp;

    //tmp = (struct STACK *) calloc(1,sizeof(struct STACK));
    tmp = new CPathStack;
    if (!tmp)
        ASSERT(FALSE);

    tmp->NodePtr = Node;
    tmp->NextStackPtr = Stack->NextStackPtr;
    Stack->NextStackPtr = tmp;
}

CNode* CPath::Pop()
{
    lPop += 1;
    CNode *tmp;
    CPathStack *tmpSTK;

    tmpSTK = Stack->NextStackPtr;
    tmp = tmpSTK->NodePtr;

    Stack->NextStackPtr = tmpSTK->NextStackPtr;
    delete tmpSTK;
    return (tmp);
}


CPath::~CPath()
{
	CleanUpPath();
}

CPath::CPath()
{
	OPEN = NULL;
	CLOSED = NULL;
	Stack = NULL;
	m_pBestNode = NULL;

}

void CPath::CleanUpPath()
{
	lPropagateDown=lPropLoop=lGenerateSuccessors=lGenerateSucc=lCheckOpen=lOpenLoop = 0;
    lCheckClosed=lClosedLoop=lInsert=lInsertLoop=lPush=lPop=lReturnBestNode = 0;
	m_pBestNode = NULL;
	CNode* Node;
	if(OPEN)
	{
		Node=OPEN->NextNode;
		while (Node != NULL)
		{
			CNode* Next = Node->NextNode;
			delete Node;
			Node=Next;
		}
		delete OPEN;
	}
	if(CLOSED)
	{
		Node=CLOSED->NextNode;
		while (Node != NULL)
		{
			CNode* Next = Node->NextNode;
			delete Node;
			Node=Next;
		}
		delete CLOSED;
	}
	if(Stack)
		delete Stack;
}

BOOL CPath::GetNextPos(CPoint p, CPoint &newpos)
{
	//Draw the path on the main map
	//p.y = p.y/ICONSIZE;
	//p.x = p.x/ICONSIZE;
	if(m_pBestNode)
	{
		CNode* first = m_pBestNode->Parent;
		CNode* next = m_pBestNode;
		if(first)
		{
			while(first->Parent)
			{
				next = first;
				first = first->Parent;
			}

			CSize size = p - first->p;
			size.cx = abs(size.cx);
			size.cy = abs(size.cy);
			if(size.cx < RANGESERCH && size.cy < RANGESERCH)
			{
				next->Parent = NULL;
			}

			if(first)
			{
				newpos = first->p;
				//newpos.y = newpos.y*ICONSIZE;
				//newpos.x = newpos.x*ICONSIZE;
				//newpos.Offset(ICONSIZE/2,ICONSIZE/2);
				return TRUE;
			}
			
		}
	}
	return FALSE;
}

void CPath::DisplayPath(CDisplay* pDisplay, CGameMap* pMap)
{
	CNode* path = m_pBestNode;
	if(m_pBestNode)
	{
		CPoint lastpos = path->p;
		//lastpos.y = lastpos.y*ICONSIZE;
		//lastpos.x = lastpos.x*ICONSIZE;
		//lastpos.Offset(ICONSIZE/2,ICONSIZE/2);
		while (path->Parent != NULL)
		{
			path=path->Parent;
			CPoint pos = path->p;
			//pos.y = pos.y*ICONSIZE;
			//pos.x = pos.x*ICONSIZE;
			//pos.Offset(ICONSIZE/2,ICONSIZE/2);
			pDisplay->DrawLine(pMap->MapToScreen(lastpos),pMap->MapToScreen(pos),RGB(255,0,0));
			lastpos = pos;
		}
	}
}
