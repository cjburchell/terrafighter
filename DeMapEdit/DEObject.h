// DEObject.h: interface for the CDEObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEOBJECT_H__128F6496_6915_4ACA_BFB7_6C7FBB0FB15E__INCLUDED_)
#define AFX_DEOBJECT_H__128F6496_6915_4ACA_BFB7_6C7FBB0FB15E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxwin.h>
#include <d3d.h>

class CDEObject  
{
public:
	void SetIcon(int i);
	virtual void GotoPos(CPoint p);
	CPoint GetCenterPoint();
	virtual CRect GetRect();
	virtual void Display();
	CDEObject();
	virtual ~CDEObject();

	CPoint GetDrawPoint()
	{
		CPoint p(static_cast<int>(m_pos.x),static_cast<int>(m_pos.y));
		return p;
	}

	CSize GetSize()
	{
		return m_size;
	}

	int GetIcon()
	{
		return m_icon;
	}

protected:
	int m_icon;
	CSize m_size;
	D3DVECTOR m_pos; //postion on the map

};

#endif // !defined(AFX_DEOBJECT_H__128F6496_6915_4ACA_BFB7_6C7FBB0FB15E__INCLUDED_)
