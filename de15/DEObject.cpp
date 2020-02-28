// DEObject.cpp: implementation of the CDEObject class.
//
//////////////////////////////////////////////////////////////////////

#include "DEObject.h"
#include "icons.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDEObject::CDEObject()
{
	m_size = CSize(32,32);
}

CDEObject::~CDEObject()
{

}

void CDEObject::Display()
{

}

CRect CDEObject::GetRect()
{
	CRect rect(CPoint(0,0),m_size);
	rect.OffsetRect(GetDrawPoint());
	return rect;
}

CPoint CDEObject::GetCenterPoint()
{
	return GetRect().CenterPoint();
}



void CDEObject::GotoPos(CPoint p)
{
	m_pos.x = p.x;
	m_pos.y = p.y;
}

void CDEObject::SetIcon(int i)
{
	m_icon = i;
}
