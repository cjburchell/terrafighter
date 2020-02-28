// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__C5166BAE_0B21_4D56_83F3_6F845BB442CC__INCLUDED_)
#define AFX_SPRITE_H__C5166BAE_0B21_4D56_83F3_6F845BB442CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DEObject.h"

enum e_SpriteState{
	MOVING  = 0,
	SEARCHING = 1,
	STOPPED=2,
	SEEKMONSTER =3,
	USEOBJECT = 4,
	RUNAWAY = 5
};

#define RANGE 640
#define HRANGE 320


class CSprite : public CDEObject  
{
public:
	int GetNextPos();
	CString GetStateStr();
	double GetAngle();
	CPoint GetLastCenterPoint();
	BOOL IsPlayer();
	virtual void GotoPos(CPoint p);
	void MoveBack();
	CSprite();
	virtual ~CSprite();
protected:
	double m_Angle;
	e_SpriteState m_move_state;
	float m_Speed;
	BOOL m_bPlayer;
	CPoint m_lastpos;
	CPoint m_GotoPos;
	CPoint m_NextPos;


};

#endif // !defined(AFX_SPRITE_H__C5166BAE_0B21_4D56_83F3_6F845BB442CC__INCLUDED_)
