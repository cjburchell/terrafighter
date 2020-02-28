#include "mouse.h"
#include "dxutil.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CCursor::CCursor(char* bmppath,C2DDraw* pDD)
{
	bmp=NULL;
	sys_verify_hr(pDD->CreateSurfaceFromBitmap( &bmp, bmppath,32,32));
	
	if(bmp!=NULL)
	{
		bmp->SetColorKey(RGB(255,0,255));
	}

	sys_verify_hr(pDD->CreateSurfaceFromBitmap( &m_pSelBmp[0], "select.bmp",32,32));

	if(m_pSelBmp[0]!=NULL)
	{
		m_pSelBmp[0]->SetColorKey(RGB(255,0,255));
	}

	sys_verify_hr(pDD->CreateSurfaceFromBitmap( &m_pSelBmp[1], "select1.bmp",64,64));

	if(m_pSelBmp[1]!=NULL)
	{
		m_pSelBmp[1]->SetColorKey(RGB(255,0,255));
	}

	sys_verify_hr(pDD->CreateSurfaceFromBitmap( &m_pSelBmp[2], "select2.bmp",96,96));
	
	if(m_pSelBmp[2] != NULL)
	{
		m_pSelBmp[2]->SetColorKey(RGB(255,0,255));
	}


}
void CCursor::setpos(int xpos,int ypos)
{
	m_point.x=xpos;
	m_point.y=ypos;
}
void CCursor::showcursor(C2DDraw* pDisplay)
{
	int h = 32;
	int w = 32;
   int maxh =  pDisplay->GetScreenHight();
   int maxw =  pDisplay->GetScreenWidth();

	int offsety = 0;
	int offsetx = 0;
	if(m_point.x < 0)
	{
		w = w + m_point.x;
		offsetx = -m_point.x;
		m_point.x = 0;
	}
	else if(m_point.x+w > maxw)
	{
		w = maxw - m_point.x;
	}

	if(m_point.y < 0)
	{
		h = h + m_point.y;
		offsety = -m_point.y;
		m_point.y = 0;
	}
	else if(m_point.y+h > maxh)
	{
		h = maxh - m_point.y;
	}

	CPoint rectPoint(0,0);
	CSize  size(w,h);
	CRect rect(rectPoint,size);
	rect.OffsetRect(offsetx,offsety);
	if(bmp)
		pDisplay->Blt(m_point.x,m_point.y,bmp,&rect);

}
CCursor::~CCursor()
{
	SAFE_DELETE(bmp);
	SAFE_DELETE(m_pSelBmp[0]);
	SAFE_DELETE(m_pSelBmp[1]);
	SAFE_DELETE(m_pSelBmp[2]);
}

CPoint CCursor::GetPos()
{
	return m_point;
}

void CCursor::ShowSel(int x, int y, CRect r, C2DDraw *pDisplay, int size)
{
	if(m_pSelBmp)
		pDisplay->Blt(x,y,m_pSelBmp[size-1],&r);
}
