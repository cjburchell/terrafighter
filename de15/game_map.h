
#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include <afx.h>
#include <2DDraw.h>

class IGameMap
{
public:
  /* virtual void SetMapVal(CPoint pos, char val) = 0;
   virtual BOOL IsInFog(CPoint p) = 0;
   virtual CPoint MapToScreen(CPoint p) = 0;
	virtual CPoint ScreenToMap(CPoint p) = 0;
   virtual BOOL CheckPos(CPoint cp) = 0;
   virtual double FindValueOfPath(double angle, CPoint dest, CPoint src) = 0;
   virtual float GetElapsedTime() = 0;
   virtual BOOL CreateChest(CPoint p, int gold) = 0;
   virtual BOOL chkpos() = 0;
   virtual void startfight() = 0;
   virtual BOOL LoadFogMap(CString filename) = 0;
   virtual void endfight() = 0;
   virtual void GoToPos(CPoint point) = 0;
	virtual void FadeOut(int time) = 0;
	virtual void FadeIn(int time) = 0;

	virtual void draw(C2DDraw* pDirDraw) = 0;
	virtual int mapitem(unsigned char value) = 0;
	virtual void savemap(CString map) = 0;
	virtual void deselect() = 0;
	virtual void getnewmap(int map,C2DDraw* pDisplay) = 0;
	virtual void outside() = 0;

   virtual BOOL CheckPosMap(CPoint cp) = 0;
	virtual void DeleteMapObj() = 0;
	virtual BOOL SaveFogMap(CString filename) = 0;
	virtual void ClearFog(CPoint p) = 0;
	virtual BOOL FindPath(double& angle,CPoint& dest, CPoint src) = 0;
	virtual void ResetTime() = 0;
	virtual void UpdateTime() = 0;
	virtual unsigned char GetBkgrdItem(CPoint pos) = 0;
	virtual void SetMapPos(int x,int y,int  playerx, int playery) = 0;
   virtual CSize GetSize() = 0;*/
};  

#endif