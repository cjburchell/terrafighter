// DEStatic.cpp: implementation of the CDEStatic class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DEStatic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDEStatic::CDEStatic() : CDEObject()
{
	m_CanMoveOver = TRUE;
    m_pBitmap = NULL;
}

CDEStatic::~CDEStatic()
{
    if(m_pBitmap)
        delete m_pBitmap;
}

BOOL CDEStatic::Create(CPoint p, CSize s)
{
	GotoPos(p);
	m_size = s;
	return TRUE;
}

void CDEStatic::Display(CDC* pDC)
{
    if(m_pBitmap)
    {
        if(pDC->RectVisible(CRect(GetDrawPoint(),GetSize())))
        {
            BITMAP bmpInfo;
            m_pBitmap->GetBitmap(&bmpInfo);

            CDC dcMemory;
            dcMemory.CreateCompatibleDC(pDC);

            // Select the bitmap into the in-memory DC
            CBitmap* pOldBitmap = dcMemory.SelectObject(m_pBitmap);

            pDC->BitBlt(GetDrawPoint().x, GetDrawPoint().y, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 
            0, 0, SRCCOPY);

            dcMemory.SelectObject(pOldBitmap);
        }
    }
}
