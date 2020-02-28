// Map.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMap window
#ifndef __map_H
#define __map_H

#include "game_map.h"
#include <2DDraw.h>
#include "icons.h"
#include "mouse.h"
#include "monst.h"
#include "MapObject.h"
#include "player.h"

#define MAPX  125
#define MAPY  122
#define MAXMAPS 50
#define MAXEDICONS 80
#define MAKEM 0

class CGameMap : public IGameMap
{
// Construction
public:
	CGameMap(IDirect3DDevice9* device, DWORD width, DWORD hight);
	
// Attributes
public:
// Operations
public:
	int load(CString map, IDirect3DDevice9* pDisplay, CPlayer* player);
	void draw(C2DDraw* pDirDraw, CCursor* pCursor, CPlayer* player, CMonsters* monsters);
	int mapitem(unsigned char value);
	void savemap(CString map);
	//void deselect(C2DDraw* pDirDraw);
	void startfight();
	void endfight();
	BOOL CheckPos(CPoint cp);
	void load_new_map(CPlayer* player, int map, IDirect3DDevice9* device);
	BOOL chkpos(CPlayer* player, CMonsters* monsters, IDirect3DDevice9* device);
	void outside();
	CSize GetSize()
	{
		return m_Size;
	}
	CSize GetMapSize()
	{
		CSize size;
		size.cy = m_Size.cy>>ICONSIZEB;
		size.cx = m_Size.cx>>ICONSIZEB;
		return size;
	}
	
	CPoint m_mappos;
	BOOL m_bMapLoaded;

protected:

	CSize m_Size;
	void ship(CPoint currentpoint);
	void gotopos(CPlayer* player, UINT xpos,UINT ypos);
	CPoint m_playerpos;
	int page;
	char edchar;
	int refnum;
	
	CSurface m_Startbmp;
	CIcons m_icons; 
	int type;
	CPoint m_startpos;

	int inout[9][3];
	
	CMapObject* m_pMap[MAPY][MAPX];
   std::list <CDEStatic*> m_MapObj;

	

	int SCREEN_TW;
	int SCREEN_TH;
// Implementation
public:
	void GoToPos(CPoint point);
	void FadeOut(int time);
	void FadeIn(int time);
	BOOL CreateChest(CPoint p, int gold);
	static void SnapToGrid(CPoint& p)
	{
		p.x = (p.x>>ICONSIZEB)<<ICONSIZEB;
		p.y = (p.y>>ICONSIZEB)<<ICONSIZEB;
		p.x+=16;
		p.y+=16;
	}
	BOOL CheckPosMap(CPoint cp);
	CDEStatic* GetObjAtPos(CPoint p);
	BOOL KillObject(CDEStatic* pObj);
	void DeleteMapObj();
	BOOL LoadFogMap(CString filename);
	BOOL SaveFogMap(CString filename);
	BOOL IsInFog(CPoint p);
	void ClearFog(CPoint p);
	double FindValueOfPath(double angle, CPoint dest, CPoint src);
	BOOL FindPath(double& angle,CPoint& dest, CPoint src);
	CPoint MapToScreen(CPoint p);
	CPoint ScreenToMap(CPoint p);
	void ResetTime();
	float m_elapsed;
	void UpdateTime();
	float GetElapsedTime();
	DWORD m_systime;
	unsigned char GetBkgrdItem(CPoint pos);
	void SetMapVal(CPoint pos, char val);
	void SetMapPos(int x,int y,int  playerx, int playery);
	~CGameMap();
	// Generated message CMap functions

};
#include "player.h"
#endif
/////////////////////////////////////////////////////////////////////////////
