// map1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// map1 window
#ifndef __map_1_H
#define __map_1_H
#include "types.h"
#include "map.h"
#include "icons.h"
#include "mouse.h"
#include "player.h"
#include "Monster.h"


class map1 : public map
{
// Construction
public:
	void move(UINT nChar);
	map1(CDisplay* pDD, HWND hWin,cursor* p_curs,CPlayer* lp_player);
	void startfight();
	void endfight();
	void shearch();
	void fkey(int pos1,int pos2);
	BOOL CheckPosMonst(CMonster* monst);
	void getnewmap(int map);
	BOOL chkpos();
	void outside();
protected:
	void ship();
	int dore(int pos1,int pos2);
	CSurface* MoveSurface;
// Implementation
public:
	~map1();
};
#endif
/////////////////////////////////////////////////////////////////////////////
