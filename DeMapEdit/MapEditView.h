// MapEditView.h : interface of the CMapEditView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITVIEW_H__9DD0ED84_7E31_4235_BAFF_DE6263773BD5__INCLUDED_)
#define AFX_MAPEDITVIEW_H__9DD0ED84_7E31_4235_BAFF_DE6263773BD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Icons.h"


class CMapEditView : public CScrollView
{
protected: // create from serialization only
	CMapEditView();
	DECLARE_DYNCREATE(CMapEditView)


// Attributes
public:
	CMapEditDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    BOOL m_bPaintMode;
    BOOL m_bStartPoint;
    BOOL m_bMonsters;
    BOOL m_bItems;
    BOOL m_bText;
    CPoint m_Cursor;

// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CMapEditView)
	afx_msg void OnViewItems();
	afx_msg void OnUpdateViewItems(CCmdUI* pCmdUI);
	afx_msg void OnViewMaptext();
	afx_msg void OnUpdateViewMaptext(CCmdUI* pCmdUI);
	afx_msg void OnViewMonsters();
	afx_msg void OnUpdateViewMonsters(CCmdUI* pCmdUI);
	afx_msg void OnViewStartpoint();
	afx_msg void OnUpdateViewStartpoint(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPopupSetstartlocation();
	afx_msg void OnEditPaint();
	afx_msg void OnUpdateEditPaint(CCmdUI* pCmdUI);
	afx_msg void OnFileSavetilelist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MapEditView.cpp
inline CMapEditDoc* CMapEditView::GetDocument()
   { return (CMapEditDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITVIEW_H__9DD0ED84_7E31_4235_BAFF_DE6263773BD5__INCLUDED_)
