#if !defined(AFX_PALETTEBAR_H__48AB58FC_9E31_436A_A9A2_679C0CBA539F__INCLUDED_)
#define AFX_PALETTEBAR_H__48AB58FC_9E31_436A_A9A2_679C0CBA539F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaletteBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaletteBar dialog

class CPaletteBar : public CDialog
{
// Construction
public:
	int GetCurrentType();
	CPaletteBar(CWnd* pParentWnd = NULL);   // standard constructor
    virtual ~CPaletteBar();

// Dialog Data
	//{{AFX_DATA(CPaletteBar)
	enum { IDD = CG_IDD_PALETTE };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CImageList* m_pImageList;

	// Generated message map functions
	//{{AFX_MSG(CPaletteBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTEBAR_H__48AB58FC_9E31_436A_A9A2_679C0CBA539F__INCLUDED_)
