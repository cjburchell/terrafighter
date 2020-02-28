// OptionsDlg.h: interface for the COptionsDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONSDLG_H__501AA052_3957_4A64_A990_C74F43707B37__INCLUDED_)
#define AFX_OPTIONSDLG_H__501AA052_3957_4A64_A990_C74F43707B37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinStatic.h"
#include "WinCheckBox.h"

class COptionsDlg : public CWinDialog  
{
public:
   CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);
   BOOL Create(C2DDraw *pDisplay,const CPoint& p);
   BOOL ValidButtonID(int Id);
   COptionsDlg();
   virtual ~COptionsDlg();
   CWinCheckBox m_CheckShadows;
   CWinCheckBox m_CheckWater;
   CWinCheckBox m_CheckMultiTex;
   CWinCheckBox m_CheckLOD;
   CWinCheckBox m_Check3DSound;
};

#endif // !defined(AFX_OPTIONSDLG_H__501AA052_3957_4A64_A990_C74F43707B37__INCLUDED_)
