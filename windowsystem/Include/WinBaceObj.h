///////////////////////////////////////////////////////////////////////////////
///  WinBaceObj.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 9:56:44 AM by Christiaan Burchell
///
///  PURPOSE:  The Base Window Object
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
///       4. The author permission is required to use this software in commercial 
///              applications 
///
///  LAST CHANGED: $Date$
///
///  REVISION HISTORY:
///  $Log$
/// 

// WinBaceObj.h: interface for the CWinBaceObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINBACEOBJ_H__C28C28D9_2BF9_43C1_8B43_A02F3ED6FC7C__INCLUDED_)
#define AFX_WINBACEOBJ_H__C28C28D9_2BF9_43C1_8B43_A02F3ED6FC7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning( disable : 4786 )
#include <list>

#include "2ddraw.h"
#include "commonres.h"


// Input: a ptr.  Output: the ptr aligned to a DWORD.
#define ALIGN_DWORD( type, p ) ( (type)(((DWORD)p+3) & ~3 ))

PWORD GetDlgEmbeddedString( PWORD pwsz, PSTR pszBuffer, unsigned cbLen );

#define WIN_USE_CTRL(id,ctrl) case id: return &ctrl;


class CWinBaceObj  
{
public:
   virtual void OnCharDown(char c, DWORD keycode);
   virtual BOOL CharPressed(char c, DWORD keycode);
   virtual BOOL MouseOver(const CPoint& p);
   virtual BOOL MouseUp(const CPoint& p,int& Id,BOOL bForceCheck = FALSE);
   virtual int OnMouseUp(const CPoint& p);
   virtual void OnMouseOver(BOOL bIsOver,const CPoint& p);
   virtual int OnMouseDown();
   virtual BOOL MouseDown(const CPoint& p, int& Id,BOOL bForceCheck = FALSE);
   virtual ~CWinBaceObj();
   virtual void OnButtonPressed(int ButtonID);
   virtual void Display(C2DDraw *pDisplay);
   virtual BOOL Create(C2DDraw *pDisplay, CWinBaceObj* pParent,const CPoint& p, const CSize& s, const TCHAR* str);
   virtual CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);


   static LPDLGITEMTEMPLATE LoadCtrl( LPDLGITEMTEMPLATE pDlgItemTemplate, CWinBaceObj* pParent,C2DDraw *pDisplay);
   CSize GetSize() const;
   CRect GetRect() const;
   const CString& GetText() const;
   void SetColors(D3DCOLOR forg, D3DCOLOR back);
   CWinBaceObj* GetParent();
   void RemoveChild(CWinBaceObj* pWin);
   const CPoint& GetPos() const;
   void SetPos(const CPoint& p);
   CPoint GetDrawPoint() const;
   CRect GetDrawRect() const;
   void SetText(const TCHAR* str);
   void SetSize(const CSize& s);
   void AddChild(CWinBaceObj* pWnd);
   CWinBaceObj();
   void SetDinamicCreation(BOOL bDi){m_bDinamic = bDi;}
   void SetID(int Id){m_ID = Id;}
   int GetID()const {return m_ID;}
   C2DDraw* GetDisplay() const {return m_pDisplay;}
   BOOL IsShown() const {return m_bShowWindow;}
   void ShowWindow(BOOL bShow = TRUE){m_bShowWindow = bShow;}
   CWinBaceObj* GetControl(int id);
   BOOL IsActive() const {return m_bActive;}
   void EnableWin(BOOL bEnable){m_bEnable = bEnable;};
   BOOL IsEnabled() const {return m_bEnable;}
   void DrawBorder(C2DDraw *pDisplay,const CRect& rect);
   void SetShowBackground(BOOL bBG = TRUE){m_bShowBackground = bBG;}
   BOOL ShowBackground() const {return m_bShowBackground;}


protected:
   D3DCOLOR m_ForgoundColor;
   D3DCOLOR m_BackgroundColor;
   D3DCOLOR m_HighlightColor;
   D3DCOLOR m_TitleBarColor;
   D3DCOLOR m_BorderColor;
   D3DCOLOR m_BorderColor2;
   D3DCOLOR m_BorderColor3;
   D3DCOLOR m_EditAreaColor;
   D3DCOLOR m_DisabledColor;
   D3DCOLOR m_SelectColor;

   CWinBaceObj* m_pParent;

private:
   BOOL m_bShowBackground;
   BOOL m_bEnable;
   BOOL m_bActive;
   BOOL m_bShowWindow;
   BOOL m_MouseWasOver;
   BOOL m_bCreated;
   CString m_text;
   CPoint m_pos;
   std::list <CWinBaceObj*> m_Childen;
   CSize m_size;
   BOOL m_bDinamic;
   int m_ID;

   C2DDraw* m_pDisplay;
};

#endif // !defined(AFX_WINBACEOBJ_H__C28C28D9_2BF9_43C1_8B43_A02F3ED6FC7C__INCLUDED_)
