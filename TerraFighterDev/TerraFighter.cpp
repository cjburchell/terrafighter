#include "stdafx.h"
#include "DXUtil.h"
#include "TerraFighterApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTerraFighterApp g_theApp;

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE  hPreviousInst, LPSTR, int nCmdShow )
{

   CoInitialize(NULL);
   sys_msg("Start App");
   sys_verify_hr(g_theApp.Create(hInst));

   return g_theApp.Run();

}

CWindowMgr* GetWindowMgr()
{
   return g_theApp.GetWindowMgr();
}

CTerraFighterApp* GetApp()
{
   return &g_theApp;
}