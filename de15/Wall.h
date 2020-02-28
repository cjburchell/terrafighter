// Wall.h: interface for the CWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WALL_H__6254FADE_9EE2_4E61_AA82_A38897BA69A4__INCLUDED_)
#define AFX_WALL_H__6254FADE_9EE2_4E61_AA82_A38897BA69A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapObject.h"

class CWall : public CMapObject  
{
public:
	CWall(char type);
	virtual ~CWall();

};

#endif // !defined(AFX_WALL_H__6254FADE_9EE2_4E61_AA82_A38897BA69A4__INCLUDED_)
