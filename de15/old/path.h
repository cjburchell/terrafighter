#ifndef __PATH_H
#define __PATH_H


class CGameMap;

#define MAX_CHILD_NODES 8
class CNode {
public:
	CNode()
	{
		f=0;
		h=0;
		g=0;
		tmpg=0;
		//x=0;
		//y=0;
		p = CPoint(0,0);
		Parent = NULL;
		NextNode = NULL;
		for(int i=0;i<MAX_CHILD_NODES;i++)
			Child[i] = NULL;
	}
    long f,h;
    UINT g,tmpg;
    //int x,y;
    //int NodeNum;
    CPoint p;
    CNode *Parent;
    CNode *Child[MAX_CHILD_NODES];       /* a node may have upto 8+(NULL) children. */
    CNode *NextNode;       /* for filing purposes */
};

class CPathStack {
public:
	CPathStack()
	{
		NodePtr = NULL;
		NextStackPtr = NULL;
	}
    CNode *NodePtr;
    CPathStack *NextStackPtr;
};



class CPath
{
public:
	void DisplayPath(CDisplay* pDisplay, CGameMap* pMap);
	BOOL GetNextPos(CPoint p, CPoint &newpos);
	
	CPath();
	virtual  ~CPath();
	void CreatePath(CPoint src, CPoint dest, CGameMap* pMap);

private:
	void CleanUpPath();
// Algorithm stuff
    BOOL FreeTile(CPoint,CGameMap* pMap);
    CNode* FindPath(CPoint s, CPoint d,CGameMap* pMap);
    CNode* ReturnBestNode(void);
    void GenerateSuccessors( CNode *BestNode, CPoint d,CGameMap* pMap);
    void GenerateSucc( CNode *BestNode, CPoint p, CPoint d);
    CNode *CheckOPEN(CPoint p);
    CNode *CheckCLOSED(CPoint p);


    void Insert( CNode *Successor);
    void PropagateDown(CNode *Old);
    void Push(CNode *Node);
    CNode *Pop(void);


private:
	// Algorithm stuff
    CNode *OPEN;
    CNode *CLOSED;
    CPathStack *Stack;

	CNode* m_pBestNode;

	long lPropagateDown;
	long lPropLoop;
	long lGenerateSuccessors;
	long lGenerateSucc;
	long lCheckOpen;
	long lOpenLoop;
	long lCheckClosed;
	long lClosedLoop;
	long lInsert;
	long lInsertLoop;
	long lPush;
	long lPop;
	long lReturnBestNode;
};

#endif

