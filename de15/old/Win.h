#ifndef __WIN_H
#define __WIN_H
#include "types.h"
#include "player.h"
#include "ddutil.h"
#define WD_STAT 0
#define WD_HEAL 1
#define WD_KEY 3
#define WD_NOTHING 4
#define WD_SAVE 5
#define WD_DEFALT (-1)



#ifdef __DEBUG
	void Dismon(TYPE_MONSTER* monst,int pos1=1,int pos2=1);
#endif
class mywin
{
	friend void mywindow(int type,CPlayer* player,CDisplay* pDirDraw,CGameMap* m_map,int pos1=1,int pos2=1);
    friend void mywindow(int type,CPlayer* player,CDisplay* pDirDraw,CGameMap* m_map,CString string,long var = WD_DEFALT);
	
public:
	mywin(int type,CPlayer* player,CDisplay* inpDirDraw,CGameMap* inm_map,int pos1=1,int pos2=1);
	~mywin();
	
protected:
	void sayraw(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay,const char* string);
	void say(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay,const char* format ...);
	void DisStat(CPlayer* player,CDisplay* pDisplay,int pos1=1,int pos2=1);
	BOOL heal(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay,int pos1=1,int pos2=1);
	BOOL buykey(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay,int pos1=1,int pos2=1);
	BOOL savegame(CPlayer* player,CGameMap* pMap,CDisplay* pDisplay);

};
#endif