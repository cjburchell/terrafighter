#include "stdafx.h"
#include <DXUtil.h>
#include "DEApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEApp g_theApp;

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE  hPreviousInst, LPSTR, int nCmdShow )
{

   CoInitialize(NULL);
   sys_msg("Start App");
   if FAILED(g_theApp.Create(hInst))
      sys_msg("Create Failed");

   return g_theApp.Run();

}

CWindowMgr* GetWindowMgr()
{
   return g_theApp.GetWindowMgr();
}

CDEApp* GetApp()
{
   return &g_theApp;
}

CInput* GetInputObj()
{
   return g_theApp.GetInput();
}


/*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define __3D_DISPLAY

void OnKeyDown(WPARAM wParam);
void OnDestroy();
void OnCharDown(WPARAM wParam);
void OnActiveApp(WPARAM wParam);
void DelGolb();
LRESULT APIENTRY WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam);

BOOL ShowBitmap(CString bmp, int time);

//Global Classes
CDisplay* g_pDisplay;
CGameMap* g_map;
CMonsters* g_pmonst;
CCursor* g_curs;
CPlayer* g_pPlayer;
CInput2* g_pInput;
CHud* g_pHud;
CDEWndowMgr g_WndowMgr;

//global Flags
BOOL GameOver=FALSE;
BOOL AcApp=TRUE;


//common globals
HWND hWindow;
MSG msg;
///////////////////////////////////////////////////////////
// WinMain
///////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE  hPreviousInst, LPSTR, int nCmdShow )
{

    CoInitialize(NULL);

    WNDCLASS wc;
    HWND     hWnd;
	g_pDisplay=NULL;
	g_pPlayer= NULL;
	g_map=NULL;
	g_pmonst=NULL;
	g_curs=NULL;
	g_pHud = NULL;


	#ifdef __3D_DISPLAY
		g_pDisplay = new C3DDisplay();
	#else
		g_pDisplay = new C2DDisplay();
	#endif

    HRESULT hr = g_pDisplay->GetVideoMode(hInst);
	if(FAILED(hr))
	{
		DelGolb();
		return FALSE;
	}	


	if(g_pDisplay->IsWindowed())
	{
		// Register the Window Class
		wc.lpszClassName = TEXT("de15");
		wc.lpfnWndProc   = WndProc;
		wc.style         = CS_VREDRAW | CS_HREDRAW;
		wc.hInstance     = hInst;
		wc.hIcon         = LoadIcon( hInst, MAKEINTRESOURCE(IDI_ICON1) );
		wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
		wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		wc.lpszMenuName  = NULL;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;

		if( RegisterClass( &wc ) == 0 )
			return E_FAIL;

		// Calculate the proper size for the window given a client of 640x480
		DWORD dwFrameWidth    = GetSystemMetrics( SM_CXSIZEFRAME );
		DWORD dwFrameHeight   = GetSystemMetrics( SM_CYSIZEFRAME );
		DWORD dwMenuHeight    = GetSystemMetrics( SM_CYMENU );
		DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );
		DWORD dwWindowWidth   = g_pDisplay->m_dwWidth  + dwFrameWidth * 2;
		DWORD dwWindowHeight  = g_pDisplay->m_dwHeight + dwFrameHeight * 2 + 
								dwMenuHeight + dwCaptionHeight;

		// Create and show the main window
		DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
		hWnd = CreateWindowEx( 0, TEXT("de15"), TEXT("Dungeone Escape v1.5"),
							   dwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
  							   dwWindowWidth, dwWindowHeight, NULL, NULL, hInst, NULL );
    
	}
	else
	{
		// Register the Window Class
		wc.lpszClassName = TEXT("de15");
		wc.lpfnWndProc   = WndProc;
		wc.style         = CS_VREDRAW | CS_HREDRAW;
		wc.hInstance     = hInst;
		wc.hIcon         = LoadIcon( hInst, MAKEINTRESOURCE(IDI_ICON1) );
		wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
		wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		wc.lpszMenuName  = NULL;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		
		if( RegisterClass( &wc ) == 0 )
			return E_FAIL;

		// Create a window of the previously defined class.
		hWnd = CreateWindowEx( 0, TEXT("de15"), TEXT("Dungeone Escape v1.5"),
							   WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
  							   CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL );

	}

	if(hWnd == NULL)
	{
		DelGolb();
		return FALSE;
	}
    // Display the window on the screen.
    ShowWindow(hWnd, nCmdShow);

    // Force the window to repaint itself.
    UpdateWindow(hWnd);
	// Intilize valriables
	hWindow=hWnd;
	
	joySetCapture(hWnd, JOYSTICKID1, NULL, FALSE);
   
	//set up the clases
	if( FAILED(g_pDisplay->Create(hWnd) ) )
	{
		MessageBox(hWnd,"Can't start graphics","ERROR",MB_ICONERROR|MB_OK);
		DelGolb();
		return 0;
	}
	
	g_pInput = new CInput2();

	if((g_curs=new CCursor("cursor.bmp",g_pDisplay,hWnd))==NULL)
	{
		g_pDisplay->FlipToGDISurface();
		MessageBox(hWnd,"mouse cursor not created","ERROR",MB_ICONERROR|MB_OK);
		DelGolb();
		return 0;
	}

	


	srand( (unsigned)time( NULL ) );


	g_pPlayer = new CPlayer;

	g_map =new CGameMap(g_pDisplay,hWnd,g_curs,g_pPlayer,&g_WndowMgr);
	if(g_map==NULL)
	{
		g_pDisplay->FlipToGDISurface();
		MessageBox(hWnd,"Map not Created","ERROR",MB_ICONERROR|MB_OK);
		DelGolb();
		return 0;
	}
	if((g_pmonst = new CMonsters())==NULL)
	{
		g_pDisplay->FlipToGDISurface();
		MessageBox(hWnd,"monsters not Created","ERROR",MB_ICONERROR|MB_OK);
		DelGolb();
		return 0;
	}
	g_map->setmonst(g_pmonst);
	ShowCursor(FALSE);


	if(!ShowBitmap("splash.bmp",2000))
	{
		DelGolb();
		return 0;
	}
	
	
	g_pPlayer->name = "";

	g_pHud=new CHud();
	if(!g_pHud->Create(g_pDisplay,NULL))
	{
		g_pDisplay->FlipToGDISurface();
		MessageBox(hWnd,"Hud not created","ERROR",MB_ICONERROR|MB_OK);
		DelGolb();
		return 0;
	}
	g_WndowMgr.AddChild(g_pHud);

	// game title screen
	if(!g_pPlayer->startnew(g_pDisplay,g_pInput,g_map))
	{
		DelGolb();
		return msg.wParam;
	}

	g_pPlayer->spells->current= -1;
	int offsety = g_pDisplay->m_dwHeight/2;
	int offsetx = g_pDisplay->m_dwWidth/2;
	g_map->SetMapPos(g_pPlayer->GetDrawPoint().x-offsetx,g_pPlayer->GetDrawPoint().y-offsety,offsetx,offsety);
	
    // Start the message loop.
    while (!GameOver)
	{
		if(!LookForMSG())
			break;

		if(!GameOver && AcApp)
		{
			if(g_pPlayer->PlayerOK==FALSE)
			{
				g_pPlayer->name = "";
				if(!g_pPlayer->startnew(g_pDisplay,g_pInput,g_map))
				{
					DelGolb();
					return msg.wParam;
				}
				if(g_pPlayer->name == "")
					PostMessage(hWindow,WM_CLOSE,0,0L);
			}
			g_map->UpdateTime();
			if(!g_pPlayer->GetInput(g_pInput,g_map,g_pDisplay))
			{
				DelGolb();
				return msg.wParam;
			}
			g_pPlayer->AninIcon(g_map->GetElapsedTime());
			if(g_pPlayer->monster==TRUE)
				g_map->m_pmonst->move(g_pPlayer,g_map,g_pDisplay);
			g_pDisplay->Clear(RGB(0,0,0));
			g_pDisplay->StartRender();
			g_map->setscr();
			g_pDisplay->Present();

		}
	}
	DelGolb();
    return msg.wParam;
}

///////////////////////////////////////////////////////////
// WndProc()
//
// This is the main window procedure, which is called by
// Windows.
///////////////////////////////////////////////////////////
LRESULT APIENTRY WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    // Handle the messages to which the application
    // must respond.
	//int pad
	UINT joyposx;
	UINT joyposy;
    switch(message)
    {
        case WM_DESTROY:
			OnDestroy();
			GameOver=TRUE;
            PostQuitMessage(0);
            return 0;
		case WM_MOUSEMOVE:
			{
				g_curs->setpos(LOWORD(lParam),HIWORD(lParam));
				CPoint p(LOWORD(lParam),HIWORD(lParam));
				if(!g_WndowMgr.MouseOver(p))
				{
					if((MK_LBUTTON&wParam)!=0)
					{
						
						p = g_map->ScreenToMap(p);
						CMonster* pMonst = g_map->m_pmonst->GetMonsterAtPos(p);
						if(pMonst)
						{
							g_pPlayer->SeekMonster(pMonst,g_map);
						}
						else
						{
							CDEStatic* pObj = g_map->GetObjAtPos(p);
							if(pObj)
								g_pPlayer->SeekObject(pObj,g_map);
							else
								g_pPlayer->SeekPoint(p,g_map,TRUE);
						}
						
						OnKeyDown(VK_ESCAPE);
					}
				}
			}
			return 0;
		case WM_LBUTTONDOWN:
			{
				CPoint p(LOWORD(lParam),HIWORD(lParam));
				int key;
				if(g_WndowMgr.MouseDown(p,key))
					OnKeyDown(key);
				else
				{
					p = g_map->ScreenToMap(p);
					CMonster* pMonst = g_map->m_pmonst->GetMonsterAtPos(p);
					if(pMonst)
					{
						g_pPlayer->SeekMonster(pMonst,g_map);
					}
					else
					{
						CDEStatic* pObj = g_map->GetObjAtPos(p);
						if(pObj)
							g_pPlayer->SeekObject(pObj,g_map);
						else
							g_pPlayer->SeekPoint(p,g_map,TRUE);
					}
					OnKeyDown(VK_ESCAPE);
				}
			}
			return 0;
		case WM_LBUTTONUP:
			{
				CPoint p(LOWORD(lParam),HIWORD(lParam));
				int key;
				if(g_WndowMgr.MouseUp(p,key))
					OnKeyDown(key);
			}
			return 0;
		case WM_RBUTTONDOWN:
			{
				CPoint p(LOWORD(lParam),HIWORD(lParam));
				p = g_map->ScreenToMap(p);
				g_pPlayer->UseSpell(g_map, g_pDisplay,p);
			}
			return 0;
		case WM_KEYDOWN:
			OnKeyDown(wParam);
			return 0;
		case MM_JOY1MOVE :
			joyposx=LOWORD(lParam);
			joyposy=HIWORD(lParam);
			if(joyposy<200)
			{
				OnKeyDown(VK_UP);
			}
			if(joyposx<200)
			{
				OnKeyDown(VK_LEFT);
			}
			if(joyposy>60000)
			{
				OnKeyDown(VK_DOWN);
			}
			if(joyposx>60000)
			{
				OnKeyDown(VK_RIGHT);
			}
			return 0; 
		case MM_JOY1BUTTONDOWN :  // button is down
			if((UINT) wParam & JOY_BUTTON1) 
			{ 
				OnKeyDown(VK_RETURN);
			} 
			if((UINT) wParam & JOY_BUTTON2) 
			{ 
				OnKeyDown('L');
			}
			if((UINT) wParam & JOY_BUTTON3) 
			{ 
				OnKeyDown(VK_ESCAPE);
			} 
			if((UINT) wParam & JOY_BUTTON4) 
			{ 
				OnKeyDown('S');
			} 
			return 0;
		case WM_ACTIVATEAPP:
			OnActiveApp(wParam);
    }

    // Make sure all messages get returned to Windows.
    return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL InitGraphics(HWND hWnd,HINSTANCE hInstance)
{
	if( FAILED(g_pDisplay->Create(hWnd) ) )
	{
		return FALSE;
	}
	return TRUE;
}

void OnKeyDown(WPARAM wParam)
{
	g_pInput->AddKeyPress(wParam);
	
}
void OnActiveApp(WPARAM wParam)
{
	if(wParam)
	{	
		AcApp=TRUE;
	}
	else
	{
		AcApp=FALSE;
	}
}


void DelGolb()
{
	SAFE_DELETE(g_pDisplay);
	SAFE_DELETE(g_pHud);
	if(g_map!=NULL)
		delete g_map;
	if(g_pmonst!=NULL)
		delete g_pmonst;
	if(g_curs!=NULL)
		delete g_curs;
	if(g_pPlayer !=NULL)
		delete g_pPlayer;
	if(g_pInput != NULL)
		delete g_pInput;
}
void OnDestroy()
{
	KillTimer(hWindow,TIMER_1);
	KillTimer(hWindow,TIMER_2);
	KillTimer(hWindow,TIMER_3);
	KillTimer(hWindow,TIMER_4);
}


BOOL PeekInput()
{
	g_pInput->ClearBuffer();
	while (g_pInput->IsEmpty())
	{
		if(!LookForMSG())
			return FALSE;
	}
	g_map->ResetTime();
	return TRUE;
}

BOOL getinput(UINT* pNewChar)
{
	g_pInput->ClearBuffer();
	while (g_pInput->IsEmpty())
	{
		if(!LookForMSG())
			return FALSE;
	}
	*pNewChar = g_pInput->RemoveKeyPress();
	return TRUE;
}

BOOL LookForMSG()
{
	if(PeekMessage(&msg, NULL, NULL, NULL,PM_REMOVE))
	{
		if(msg.message==WM_QUIT)
			return FALSE;
		else
		{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
		}
	}
	return TRUE;

}


BOOL ShowBitmap(CString bmp, int time)
{
	CSurface* pSplash = NULL;
	g_pDisplay->CreateSurfaceFromBitmap(&pSplash,bmp,g_pDisplay->m_dwWidth,g_pDisplay->m_dwHeight);
	if(pSplash)
	{
		// Fade in
		DWORD newtime=timeGetTime();
		DWORD systime = newtime;
		DWORD elapsed;
		

		FLOAT alpha = 255.0f;
		while(alpha>0.0f && GetInputObj()->IsEmpty())
		{
			if(!LookForMSG())
			{
				delete pSplash;
				return FALSE;
			}
			g_pDisplay->Clear(RGB(0,0,0));
			g_pDisplay->StartRender();
			CRect rect(0,0,g_pDisplay->m_dwWidth,g_pDisplay->m_dwHeight);
			g_pDisplay->Blt(0,0,pSplash,&rect);
			g_pDisplay->DrawBlendRect(rect,D3DCOLOR_ARGB(((BYTE)alpha),0,0,0));
			g_pDisplay->Present();
			newtime=timeGetTime();
			elapsed = float(newtime - systime);
			systime=newtime;
			FLOAT inc = ((FLOAT)elapsed/(FLOAT)time)*510.0f;
			alpha-= inc;
		}

		// fade out
		alpha = 0.0f;
		while(alpha<255.0f && GetInputObj()->IsEmpty())
		{
			if(!LookForMSG())
			{
				delete pSplash;
				return FALSE;
			}
			g_pDisplay->Clear(RGB(0,0,0));
			g_pDisplay->StartRender();
			CRect rect(0,0,g_pDisplay->m_dwWidth,g_pDisplay->m_dwHeight);
			g_pDisplay->Blt(0,0,pSplash,&rect);
			g_pDisplay->DrawBlendRect(rect,D3DCOLOR_ARGB(((BYTE)alpha),0,0,0));
			g_pDisplay->Present();
			newtime=timeGetTime();
			elapsed = float(newtime - systime);
			systime=newtime;
			FLOAT inc = ((FLOAT)elapsed/(FLOAT)time)*510.0f;
			alpha+= inc;
		}
		delete pSplash;
	}
	return TRUE;
}
*/



