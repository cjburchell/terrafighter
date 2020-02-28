// MapEditView.cpp : implementation of the CMapEditView class
//

#include "stdafx.h"
#include "MapEdit.h"
#include "MainFrm.h"
#include "MapEditDoc.h"
#include "MapEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditView

IMPLEMENT_DYNCREATE(CMapEditView, CScrollView)

BEGIN_MESSAGE_MAP(CMapEditView, CScrollView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CMapEditView)
	ON_COMMAND(ID_VIEW_ITEMS, OnViewItems)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ITEMS, OnUpdateViewItems)
	ON_COMMAND(ID_VIEW_MAPTEXT, OnViewMaptext)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAPTEXT, OnUpdateViewMaptext)
	ON_COMMAND(ID_VIEW_MONSTERS, OnViewMonsters)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MONSTERS, OnUpdateViewMonsters)
	ON_COMMAND(ID_VIEW_STARTPOINT, OnViewStartpoint)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STARTPOINT, OnUpdateViewStartpoint)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_POPUP_SETSTARTLOCATION, OnPopupSetstartlocation)
	ON_COMMAND(ID_EDIT_PAINT, OnEditPaint)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PAINT, OnUpdateEditPaint)
	ON_COMMAND(ID_FILE_SAVETILELIST, OnFileSavetilelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditView construction/destruction

CMapEditView::CMapEditView()
{
    m_bStartPoint = TRUE;
    m_bMonsters = TRUE;
    m_bItems = TRUE;
    m_bText = FALSE;
    m_bPaintMode = FALSE;
}

CMapEditView::~CMapEditView()
{
}

BOOL CMapEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditView drawing

void CMapEditView::OnDraw(CDC* pDC)
{
	CMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    //pDC->SetBkMode(TRANSPARENT);
    //pDC->SetBkColor(RGB(255,0,255));

    CSize ObjCount = pDoc->GetObjCount();

    for(int x=0;x < ObjCount.cx; x++)
    {
        for(int y=0;y<ObjCount.cy;y++)
        {
            CMapObject* pObj =  pDoc->GetMapObj(x,y);
            if(pObj)
            {
                pObj->Display(pDC,&pDoc->m_Icons,m_bText);
            }
        }
    }
    if(m_bItems)
    {
        POSITION pos = pDoc->m_MapObj.GetHeadPosition();
	    // for each monster thats alive
	    while(pos)
	    {
		    CDEStatic* pObj = pDoc->m_MapObj.GetNext(pos);
		    if(pObj)
		    {
			    pObj->Display(pDC);
		    }
	    }
    }

    if(m_bMonsters)
    {
        POSITION pos = pDoc->m_pMonst.GetHeadPosition();
	    // for each monster thats alive
	    while(pos)
	    {
		    CMonster* pMonst = pDoc->m_pMonst.GetNext(pos);
		    if(pMonst)
		    {
			    pMonst->Display(pDC);
		    }
	    }
    }

    if(m_bStartPoint)
    {
        CPoint StartPos = pDoc->GetStartPos();
        if(pDC->RectVisible(CRect(StartPos,CSize(32,32))))
        {
            pDoc->m_Icons.DrawIcon(48,pDC,StartPos);
        }
    }

    CRect rec(m_Cursor,CSize(32,32));
    if(pDC->RectVisible(rec))
    {
        CBrush brush(RGB(255,255,255));
        pDC->FrameRect(rec,&brush);
        //m_Icons.DrawIcon(58,pDC,m_Cursor);
    }
}

void CMapEditView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

    CMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSize sizeTotal = pDoc->GetSize();
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditView diagnostics

#ifdef _DEBUG
void CMapEditView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMapEditView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMapEditDoc* CMapEditView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditDoc)));
	return (CMapEditDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapEditView message handlers

void CMapEditView::OnViewItems() 
{
	m_bItems = !m_bItems;
    Invalidate();
}

void CMapEditView::OnUpdateViewItems(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_bItems);
	
}

void CMapEditView::OnViewMaptext() 
{
    m_bText = !m_bText;
    Invalidate();	
}

void CMapEditView::OnUpdateViewMaptext(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_bText);	
}

void CMapEditView::OnViewMonsters() 
{
	m_bMonsters = !m_bMonsters;
    Invalidate();
}

void CMapEditView::OnUpdateViewMonsters(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_bMonsters);	
	
}

void CMapEditView::OnViewStartpoint() 
{
	m_bStartPoint = !m_bStartPoint;
    Invalidate();
}

void CMapEditView::OnUpdateViewStartpoint(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_bStartPoint);	
	
}

void CMapEditView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    CPoint OldPoint = m_Cursor;
    m_Cursor = point;
    CClientDC dc(this);
    OnPrepareDC(&dc);
    dc.DPtoLP(&m_Cursor);
    dc.LPtoDP(&OldPoint);
    m_Cursor.x = (m_Cursor.x/32)*32;
    m_Cursor.y = (m_Cursor.y/32)*32;
    point = m_Cursor;
    dc.LPtoDP(&point);
    // do somthing with the single click
    if(m_bPaintMode)
    {
        CMapEditDoc* pDoc = GetDocument();
	    ASSERT_VALID(pDoc);
        CPoint mappoint(m_Cursor.x/32,m_Cursor.y/32);
        CMapObject* pObj =  pDoc->GetMapObj(mappoint.x,mappoint.y);
        if(pObj)
        {
            int Type = static_cast<CMainFrame*>(AfxGetMainWnd())->m_wndPalette.GetCurrentType();
            if(Type != -1)
            {
                pObj->SetType(Type,pDoc->m_Icons.GetImage(Type,pDoc->GetType()),pDoc->m_Icons.GetItemText(Type));
            }
        }
    }


    InvalidateRect(CRect(OldPoint,CSize(32,32)));
    InvalidateRect(CRect(point,CSize(32,32)));	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CMapEditView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    CMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    CClientDC dc(this);
    OnPrepareDC(&dc);
    dc.DPtoLP(&point);
    CPoint mappoint(point.x/32,point.y/32);
    point.x = mappoint.x*32;
    point.y = mappoint.y*32;
    // do somthing with the double click

    CMapObject* pObj =  pDoc->GetMapObj(mappoint.x,mappoint.y);
    if(pObj)
    {
        pObj->ShowEdit();
    }

	CScrollView::OnLButtonDblClk(nFlags, point);
}

#include "resource.h"
void CMapEditView::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}
        else
        {
            CPoint NewPoint = point;
            ScreenToClient(&NewPoint);

            CPoint OldPoint = m_Cursor;
            m_Cursor = NewPoint;

            CClientDC dc(this);
            OnPrepareDC(&dc);
            dc.DPtoLP(&m_Cursor);
            dc.LPtoDP(&OldPoint);
            m_Cursor.x = (m_Cursor.x/32)*32;
            m_Cursor.y = (m_Cursor.y/32)*32;
            NewPoint = m_Cursor;
            dc.LPtoDP(&NewPoint);
            // do somthing with the single click


            InvalidateRect(CRect(OldPoint,CSize(32,32)));
            InvalidateRect(CRect(NewPoint,CSize(32,32)));
        }

		CMenu menu;
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_MAP_EDIT_VIEW));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

void CMapEditView::OnPopupSetstartlocation() 
{
    CMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    CPoint OldPoint = pDoc->GetStartPos();

    pDoc->SetStartPos(m_Cursor);
    CPoint NewPoint = m_Cursor;

    CClientDC dc(this);
    OnPrepareDC(&dc);
    dc.LPtoDP(&NewPoint);
    dc.LPtoDP(&OldPoint);

    InvalidateRect(CRect(OldPoint,CSize(32,32)));
    InvalidateRect(CRect(NewPoint,CSize(32,32)));
}

void CMapEditView::OnEditPaint() 
{
	m_bPaintMode = !m_bPaintMode;
	
}

void CMapEditView::OnUpdateEditPaint(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bPaintMode);
}

void CMapEditView::OnFileSavetilelist() 
{
    CMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    pDoc->m_Icons.SaveXML();
}
