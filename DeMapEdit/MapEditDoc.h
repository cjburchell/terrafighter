// MapEditDoc.h : interface of the CMapEditDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITDOC_H__29221971_AD1A_4122_B788_32CC2110EA8E__INCLUDED_)
#define AFX_MAPEDITDOC_H__29221971_AD1A_4122_B788_32CC2110EA8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapObject.h"
#include "Monster.h"



#define MAX_RANDOM_MONST 25

class CWarps
{
public:
    int x;
    int y;
    int map;
};

#define MAPX  125
#define MAPY  122

class CMapEditDoc : public CDocument
{
protected: // create from serialization only
	CMapEditDoc();
	DECLARE_DYNCREATE(CMapEditDoc)

// Attributes
public:
    CList <CDEStatic*,CDEStatic*> m_MapObj;
    CList<CMonster*,CMonster*> m_pMonst;

// Operations
public:
    CIcons m_Icons;
    CSize GetSize(){return m_Size;};
    CSize GetObjCount(){return m_ObjCount;};
    CMapObject* GetMapObj(UINT x, UINT y)
    {
        if(x<MAPX && y<MAPY)
            return m_pMap[y][x];
        return NULL;
    };
    int GetType(){return m_Type;};
    CPoint GetStartPos(){return m_startpos;};
    void SetStartPos(CPoint pos){m_startpos = pos;};


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    BOOL LoadMonsters(int set);
    CMapObject* m_pMap[MAPY][MAPX];
    CSize m_ObjCount;
    void DeleteMapObj();
    int m_Type;
    CSize m_Size;
    CPoint m_startpos;
    CWarps inout[9];

// Generated message map functions
protected:
	//{{AFX_MSG(CMapEditDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITDOC_H__29221971_AD1A_4122_B788_32CC2110EA8E__INCLUDED_)
