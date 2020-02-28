// MapObject.h: interface for the CMapObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPOBJECT_H__64B1979D_5226_43E0_A533_843A87BAC555__INCLUDED_)
#define AFX_MAPOBJECT_H__64B1979D_5226_43E0_A533_843A87BAC555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DEStatic.h"
#include "icons.h"

class CMapObject : public CDEStatic  
{
public:
	void ShowEdit();
	virtual void Display(CDC* pDC, CIcons* pIcons,BOOL bText);
	virtual BOOL Create(CPoint p,int icon, CString Text);
	void SetType(char type,int icon, CString Text);
	char GetMapType(BOOL bRealType = FALSE);
	CMapObject(char type);
	virtual ~CMapObject();

	BOOL IsShown()
	{
		return m_bShown;
	}
	void Show(BOOL bShown)
	{
		m_bShown = bShown;
	}
protected:
    CString m_Text;
	char m_MapType;
	char m_MoveValue;
	BOOL m_bShown;

};

#endif // !defined(AFX_MAPOBJECT_H__64B1979D_5226_43E0_A533_843A87BAC555__INCLUDED_)
