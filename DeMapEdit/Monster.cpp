// Monster.cpp: implementation of the CMonster class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Monster.h"
#include "icons.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonster::CMonster() : CSprite()
{
	m_pBitmap=NULL;
	m_MoveNumb=0;
	m_Speed=1;
}

CMonster::~CMonster()
{
	if(m_pBitmap)
		delete m_pBitmap;
}



void CMonster::Display(CDC* pDC)
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

BOOL CMonster::Create(
		     int hp1,
			 int constpoint,
			 int ha,
			 UINT xp,
			 CString bmp_file,
			 CString name,
			 UINT gold,
			 CSize size,
			 CPoint pos,
			 BOOL movement,
			 TYPE_NPC npctype)
{
	m_gold = gold;
	m_bCanMove = movement;
	m_ha = ha;
	m_name = name;
	m_npctype = npctype;
	m_xp = xp;
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_size = size;
	m_hp=0;
    for(int i=0;i<hp1;i++)
		m_hp=m_hp+(rand()%8)+1;
	m_hp=constpoint+m_hp;
	if(m_hp<=0)
		m_hp=1;

    if(m_pBitmap)
		delete m_pBitmap;
    m_pBitmap = NULL;

    HBITMAP hbitmap = (HBITMAP)LoadImage(::AfxGetInstanceHandle(),bmp_file,IMAGE_BITMAP,m_size.cx,m_size.cy,LR_CREATEDIBSECTION|LR_LOADFROMFILE);
    if(hbitmap)
    {
        m_pBitmap = new CBitmap();
        m_pBitmap->Attach(hbitmap);
    }

	if(m_pos.x==0)
	{
		m_pos.x=rand()%((17-(m_size.cx>>ICONSIZEB))<<ICONSIZEB)+1;
		m_pos.y=rand()%((12-(m_size.cy>>ICONSIZEB))<<ICONSIZEB)+1;
	}

	m_lastpos=GetDrawPoint();
	m_MoveNumb=0;
	m_Speed=1;
	return TRUE;
}
