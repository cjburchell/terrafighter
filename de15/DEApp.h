#pragma once

#include <d3dapp.h>
#include <windowmgr.h>
#include <input.h>
#include "map.h"
#include "Monster.h"
#include "hud.h"

class CDEApp : public CD3DApplication  
{
public:
   CDEApp(void);
   ~CDEApp(void);

   HRESULT OneTimeSceneInit();
   HRESULT FrameMove();
   HRESULT ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                        D3DFORMAT Format );
   HRESULT FinalCleanup();
   HRESULT DeleteDeviceObjects();
   HRESULT InvalidateDeviceObjects();
   HRESULT Render();
   HRESULT RestoreDeviceObjects();
   HRESULT InitDeviceObjects();

   BOOL LookForMSG();
   LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

   void TakeScreenShot(const char* file_name);
   void DisplayProgress(CString str, FLOAT pcentDone);

   CWindowMgr* GetWindowMgr(){return &m_WinMgr;};
   CInput* GetInput(){return &m_Input;};
   C2DDraw* GetDisplay(){return &m_2dDraw;};
   CWinBaceObj* GetHud(){ return  &m_Hud;};
   IGameMap* GetGameMap(){ return m_map;};


private:
   CWindowMgr m_WinMgr;

   C2DDraw m_2dDraw;
   CGameMap* m_map;
   CMonsters m_monst;
   CCursor* m_curs;
   CPlayer m_Player;
   CInput m_Input;
   CHud m_Hud;

   //global Flags
   BOOL GameOver;
   BOOL AcApp;

};



CDEApp* GetApp();
CWindowMgr* GetWindowMgr();
CInput* GetInputObj();


