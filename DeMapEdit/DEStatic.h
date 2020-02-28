// DEStatic.h: interface for the CDEStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESTATIC_H__4659DEF3_4DF1_4978_99B7_FA3296BBE461__INCLUDED_)
#define AFX_DESTATIC_H__4659DEF3_4DF1_4978_99B7_FA3296BBE461__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DEObject.h"

class CDEStatic : public CDEObject  
{
public:
	virtual void Display(CDC* pDC);
	virtual BOOL Create(CPoint p,CSize s);
	CDEStatic();
	virtual ~CDEStatic();
protected:
	BOOL m_CanMoveOver;
    CBitmap* m_pBitmap;
};

#endif // !defined(AFX_DESTATIC_H__4659DEF3_4DF1_4978_99B7_FA3296BBE461__INCLUDED_)
