#ifndef __MOUSE_H
#define __MOUSE_H
#include <2DDraw.h>
#include <surface.h>

class CCursor
{
public:
	void ShowSel(int x,int y, CRect r, C2DDraw* pDisplay, int size);
	CPoint GetPos();
	CCursor(char* bmppath,C2DDraw* pDD);
	void setpos(int xpos,int ypos);
	void showcursor(C2DDraw* pDisplay);
	~CCursor();
protected:
	CPoint m_point;
	CSurface* bmp;
	CSurface* m_pSelBmp[3];
};
#endif