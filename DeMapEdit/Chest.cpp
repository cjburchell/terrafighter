// Chest.cpp: implementation of the CChest class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Chest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChest::CChest() :CDEStatic()
{
	m_bDestroyAfterUse = FALSE;
}

CChest::~CChest()
{

}

BOOL CChest::Create(CPoint p, CSize s, BOOL bOpen, BOOL bKey, BOOL bDestroyAfterUse)
{
	m_bDestroyAfterUse = bDestroyAfterUse;
	m_bIsOpen = bOpen;

    
    if(m_pBitmap)
		delete m_pBitmap;
    m_pBitmap = NULL;

    HBITMAP hbitmap = (HBITMAP)LoadImage(::AfxGetInstanceHandle(),m_bIsOpen?"ochest.bmp":"chest.bmp",IMAGE_BITMAP,s.cx,s.cy,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
    if(hbitmap)
    {
        m_pBitmap = new CBitmap();
        m_pBitmap->Attach(hbitmap);
    }
	
	if(!bKey)
	{
        m_Type = CHEST_ITEM;
	}
	else
		m_Type = CHEST_KEY;
	return CDEStatic::Create(p,s);
}
