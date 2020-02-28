///////////////////////////////////////////////////////////////////////////////
///  TerraEd.cpp
///
///  PATH: D:\dev2\TerraEd
///
///  CREATED: 03/06/2004 9:06:54 AM by Christiaan Burchell
///
///  PURPOSE:  Defines the class behaviors for the application.
///
///  COPYRIGHT NOTICE:   Copyright (C) 2004 Redpoint Games 
///
///   This software is provided 'as-is', without any express or implied 
///   warranty.  In no event will the author be held liable for any damages 
///   arising from the use of this software. 
/// 
///   Permission is granted to anyone to use this software for any purpose,
///   excluding commercial applications, and to alter it and redistribute it
///   freely, subject to the following restrictions:
/// 
///   1. The origin of this software must not be misrepresented; you must not
///      claim that you wrote the original software. If you use this software
///      in a product, an acknowledgment in the product documentation would be
///      appreciated but is not required.
///   2. Altered source versions must be plainly marked as such, and must not be
///      misrepresented as being the original software.
///   3. This notice may not be removed or altered from any source distribution.
///   4. The author permission is required to use this software in commercial 
///      applications 
///
///  LAST CHANGED: $Date$
///
///  REVISION HISTORY:
///  $Log$
///

#include "stdafx.h"
#include "TerraEd.h"

#include "ChildFrm.h"
#include "TerraEdDoc.h"
#include "TerraEdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerraEdApp

BEGIN_MESSAGE_MAP(CTerraEdApp, CWinApp)
   //{{AFX_MSG_MAP(CTerraEdApp)
   ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
   // NOTE - the ClassWizard will add and remove mapping macros here.
   //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG_MAP
   // Standard file based document commands
   ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
   ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerraEdApp construction



/////////////////////////////////////////////////
///
/// NAME: CTerraEdApp
///
/// CLASS: CTerraEdApp
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 9:05:50 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CTerraEdApp::CTerraEdApp()
{
   // TODO: add construction code here,
   // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTerraEdApp object

CTerraEdApp theApp;


/////////////////////////////////////////////////
///
/// NAME: InitInstance
///
/// CLASS: CTerraEdApp
///
/// DESCRIPTION: CTerraEdApp initialization
///
/// CREATED:     03/06/2004 9:05:59 AM
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CTerraEdApp::InitInstance()
{
   AfxEnableControlContainer();

   ::CoInitialize(NULL);

   // Change the registry key under which our settings are stored.
   // TODO: You should modify this string to be something appropriate
   // such as the name of your company or organization.
   SetRegistryKey(_T("Local AppWizard-Generated Applications"));

   LoadStdProfileSettings();  // Load standard INI file options (including MRU)

   // Register the application's document templates.  Document templates
   //  serve as the connection between documents, frame windows and views.

   CSingleDocTemplate* pDocTemplate;
   pDocTemplate = new CSingleDocTemplate(
      IDR_MAINFRAME,
      RUNTIME_CLASS(CTerraEdDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CTerraEdView));
   AddDocTemplate(pDocTemplate);

   // create main MDI Frame window
   //CMainFrame* pMainFrame = new CMainFrame;
   //if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
   //	return FALSE;
   //m_pMainWnd = pMainFrame;

   // Parse command line for standard shell commands, DDE, file open
   CCommandLineInfo cmdInfo;
   ParseCommandLine(cmdInfo);

   if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
   {
      CString Path = GetProfileString(_T("Settings"),_T("LastOpenFile"),_T("TerraFighter.dat")); // get the last open file

      if(!Path.IsEmpty())
         pDocTemplate->OpenDocumentFile(Path);
      else
         pDocTemplate->OpenDocumentFile(_T("TerraFighter.dat"));
   }
   else
   {
      // Dispatch commands specified on the command line
      if (!ProcessShellCommand(cmdInfo))
         return FALSE;
   }

   // The main window has been initialized, so show and update it.
   m_pMainWnd->ShowWindow(m_nCmdShow);
   m_pMainWnd->UpdateWindow();

   return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
   CAboutDlg();

   // Dialog Data
   //{{AFX_DATA(CAboutDlg)
   enum { IDD = IDD_ABOUTBOX };
   //}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CAboutDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

   // Implementation
protected:
   //{{AFX_MSG(CAboutDlg)
   // No message handlers
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////
///
/// NAME: CAboutDlg
///
/// CLASS: CAboutDlg
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     03/06/2004 9:06:19 AM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
   //{{AFX_DATA_INIT(CAboutDlg)
   //}}AFX_DATA_INIT
}



/////////////////////////////////////////////////
///
/// NAME: DoDataExchange
///
/// CLASS: CAboutDlg
///
/// DESCRIPTION: Data Exchange for the dialog
///
/// CREATED:     03/06/2004 9:06:21 AM
///
/// PARAMETERS: 
///             CDataExchange* pDX
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CAboutDlg)
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
   //{{AFX_MSG_MAP(CAboutDlg)
   // No message handlers
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////
///
/// NAME: OnAppAbout
///
/// CLASS: CTerraEdApp
///
/// DESCRIPTION: App command to run the dialog
///
/// CREATED:     03/06/2004 9:06:27 AM
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CTerraEdApp::OnAppAbout()
{
   CAboutDlg aboutDlg;
   aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTerraEdApp message handlers

