// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sprite.h"
#include "icons.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite() : CDEObject()
{
	m_bPlayer = FALSE;
	m_move_state = STOPPED;
	m_Angle = 0;
}

CSprite::~CSprite()
{
}

void CSprite::MoveBack()
{
	m_pos.x = m_lastpos.x;
	m_pos.y = m_lastpos.y;
}

void CSprite::GotoPos(CPoint p)
{
	m_lastpos = p;
	CDEObject::GotoPos(p);
}

BOOL CSprite::IsPlayer()
{
	return m_bPlayer;
}

CPoint CSprite::GetLastCenterPoint()
{
	CRect rect(CPoint(0,0),m_size);
	rect.OffsetRect(m_lastpos);
	return rect.CenterPoint();
}

double CSprite::GetAngle()
{
	return m_Angle;
}

CString CSprite::GetStateStr()
{
	switch(m_move_state)
	{
	case MOVING:
		return "MOVING";
	case SEARCHING:
		return "SEARCHING";
	case STOPPED:
		return "STOPPED";
	case SEEKMONSTER:
		return "SEEKMONSTER";
	case USEOBJECT:
		return "USEOBJECT";
	}
	return "";
}
