// PaletteBar.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "PaletteBar.h"
#include "icons.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaletteBar dialog


CPaletteBar::CPaletteBar(CWnd* pParentWnd)
: CDialog(CPaletteBar::IDD, pParentWnd)
{
	//{{AFX_DATA_INIT(CPaletteBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_pImageList = NULL;
}

CPaletteBar::~CPaletteBar()
{
    if(m_pImageList)
        delete m_pImageList;
}


void CPaletteBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaletteBar)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaletteBar, CDialog)
	//{{AFX_MSG_MAP(CPaletteBar)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteBar message handlers

void CPaletteBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
    if(IsWindow(m_list))
	    m_list.MoveWindow(0,0,cx,cy);
	
}

BOOL CPaletteBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CRect rec;
    GetClientRect(&rec);
    m_list.MoveWindow(&rec);

    CIcons Icons;
    if(!Icons.Create())
        return FALSE;

    m_pImageList = Icons.CreateImageList();
    m_list.SetImageList(m_pImageList,LVSIL_NORMAL);
    m_list.SetImageList(m_pImageList,LVSIL_SMALL);
    m_list.InsertColumn(0,"stuff",LVCFMT_LEFT,rec.Size().cy);

    for(int i=0;i<256;i++)
    {
        CString Text = Icons.GetItemText(i);
        if(!Text.IsEmpty())
        {
            CString ItemTxt;
            ItemTxt.Format("%d '%c' %s",i,i,Text);
            int iItem = m_list.InsertItem(i,ItemTxt,Icons.GetImage(i,0));
            m_list.SetItemData(iItem,i);
        }
    }
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CPaletteBar::GetCurrentType()
{
    int retval = -1;
    POSITION pos = m_list.GetFirstSelectedItemPosition();
    if(pos)
    {
        int iItem = m_list.GetNextSelectedItem(pos);
        return m_list.GetItemData(iItem);
    }

    return retval;
}
