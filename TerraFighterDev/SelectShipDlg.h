// SelectShipDlg.h: interface for the CSelectShipDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTSHIPDLG_H__4752E857_6515_4E8A_AA1F_BB21547B5CAE__INCLUDED_)
#define AFX_SELECTSHIPDLG_H__4752E857_6515_4E8A_AA1F_BB21547B5CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinDialog.h"
#include "WinArrowButton.h"
#include "WinStatic.h"
#include "sprite.h"
#include "WinBitmapCtrl.h"
#include "WinEditBox.h"

class CSelectShipDlg : public CWinDialog  
{
public:
   CWinBaceObj* CreateControl(int id,int classId,const TCHAR* Class,DWORD Style);
   BOOL Create(C2DDraw *pDisplay,const CPoint& p);
   BOOL ValidButtonID(int Id);
   CSelectShipDlg(CD3DArcBall* pArcBall,CSpriteDataEx* pSpriteData, DWORD MaxSpriteData,BOOL bAllShips);
   virtual ~CSelectShipDlg();
   void SetStats(CSpriteDataEx* pData);
   virtual void OnButtonPressed(int ButtonID);

   virtual void OnMouseOver(BOOL bIsOver, const CPoint& p);

   BOOL IsValidShip(int type){return !m_SpriteData[type].m_IsBuilding && (m_SpriteData[type].m_bCanBeUser || m_bAllShips);}

   int m_ShipType;
   CWinEditBox m_PlayerName;
private:
   void UpdateDisplay();
   void UpdateControls();
   CWinButton m_start;
   CWinArrowButton m_next;
   CWinArrowButton m_prev;
   CWinButton m_back;
   CWinStatic m_Stats;
   CWinBitmapCtrl m_bmp;
   CD3DArcBall* m_pArcBall;

   CSurface   m_ShipDisplay;

   BOOL m_bAllShips;



   CSpriteDataEx* m_SpriteData;
   DWORD m_MaxSpriteData;

};

#endif // !defined(AFX_SELECTSHIPDLG_H__4752E857_6515_4E8A_AA1F_BB21547B5CAE__INCLUDED_)
