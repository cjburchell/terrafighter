// Door.cpp: implementation of the CDoor class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Door.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoor::CDoor() : CDEStatic()
{
	m_CanMoveOver = FALSE;
}

CDoor::~CDoor()
{

}

BOOL CDoor::Create(CPoint p, CSize s)
{
    if(m_pBitmap)
		delete m_pBitmap;
    m_pBitmap = NULL;
    HBITMAP hbitmap = (HBITMAP)LoadImage(::AfxGetInstanceHandle(),"door.bmp",IMAGE_BITMAP,s.cx,s.cy,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
    if(hbitmap)
    {
        m_pBitmap = new CBitmap();
        m_pBitmap->Attach(hbitmap);
    }
	return CDEStatic::Create(p,s);
}
