// MapObject.cpp: implementation of the CMapObject class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MapObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapObject::CMapObject(char type) : CDEStatic()
{
	m_MapType = type;
	m_bShown = TRUE;
}

CMapObject::~CMapObject()
{
}

char CMapObject::GetMapType(BOOL bRealType)
{
	if(m_bShown || bRealType)
		return m_MapType;
	else
		return 'D';
}

void CMapObject::SetType(char type, int icon,CString Text)
{
	m_MapType=type;
	m_icon = icon;
    m_Text = Text;
}

BOOL CMapObject::Create(CPoint p, int icon, CString Text)
{
	SetType(m_MapType,icon, Text);
	return CDEStatic::Create(p,CSize(32,32));
}

void CMapObject::Display(CDC* pDC, CIcons* pIcons, BOOL bText)
{
	if(!IsShown())
		return;

    if(pDC->RectVisible(CRect(GetDrawPoint(),GetSize())))
    {
        pIcons->DrawIcon(m_icon,pDC,GetDrawPoint());
        CString str;
        str.Format("%d",m_MapType);
        if(bText)
            pDC->TextOut(GetDrawPoint().x,GetDrawPoint().y,str);
    }
}

void CMapObject::ShowEdit()
{
    AfxMessageBox(m_Text);
}
