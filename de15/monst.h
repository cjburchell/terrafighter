// monst.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// monst window
#ifndef __monst_h
#define __monst_h
#include <afx.h>
#include "monster.h"

#define NUMB_TOWN_NPC 20
#define F_NORTH  0x01
#define F_SOUTH  0x02
#define F_EAST   0x04
#define F_WEST   0x08
#define MAX_RANDOM_MONST 25


class CMonsters
{
public:
	CMonster* GetMonsterAtPos(CPoint p);
	void KillMonster(CMonster* pMonster,CPlayer* pPlayer);
	int GetNumb();
	CMonsters();
	void move(CPlayer* player,IGameMap* pMap,C2DDraw* pDisplay);
	void display(IGameMap *pMap,C2DDraw* pDisplay,CCursor* pCursor);
	int checkpos(CPlayer* player,C2DDraw* pDisplay,IGameMap* pMap);
	void start(CPlayer* player,IDirect3DDevice9* pDisplay,UINT type,UINT numb,BOOL IsRand);
	void end();
	~CMonsters();
	CMonster* OtherMonstCheck(CMonster* pMonster);
	BOOL CreateMonst(CSprite* owner,CPlayer* pPlayer,IGameMap* pMap,IDirect3DDevice9* pDisplay,TYPE_NPC type,CPoint pos,double angle = 0,BOOL bUsePoint = FALSE, CPoint SeekPoint = CPoint(0,0));
protected:
	CList<CMonster*,CMonster*> m_pMonst;
	CMonster* getmoster(CPlayer* player,IDirect3DDevice9* pDisplay,int set,BOOL IsRand,int numb = 0,CSprite* pOwner = NULL);
	BOOL movechek(int numb,int spaces,CPlayer* player);
};
	
#endif

/////////////////////////////////////////////////////////////////////////////
