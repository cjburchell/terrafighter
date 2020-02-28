#include "StdAfx.h"
#include "deapp.h"
#include <dxutil.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDEApp::~CDEApp()
{
}

//-----------------------------------------------------------------------------
// Name: CDEApp()
// Desc: Constructor
//-----------------------------------------------------------------------------
CDEApp::CDEApp()
{
   m_bUseDepthBuffer        = TRUE;

   m_dwMinDepthBits    = 16;
   m_dwMinStencilBits  = 4;
   m_bShowCursorWhenFullscreen = TRUE;

   m_dwCreationWidth   = 640;
   m_dwCreationHeight  = 480;

   m_bWindowed = FALSE;
}




//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CDEApp::OneTimeSceneInit()
{
   sys_verify_hr(m_Input.CreateDevice(m_hWnd));
   srand( (unsigned)time( NULL ) );

   m_curs = new CCursor("cursor.bmp",&m_2dDraw);


   m_map =new CGameMap(&m_2dDraw,m_curs,&m_Player);

	if(m_map==NULL)
	{
		sys_msg("Map not Created");
		return E_FAIL;
	}

   if(!m_Hud.Create(&m_2dDraw,NULL))
	{
		sys_msg("Hud not created");
		return E_FAIL;
	}
	m_WinMgr.AddChild(&m_Hud);

	m_map->setmonst(&m_monst);

   m_Player.spells->current= -1;
	int offsety = m_2dDraw.GetScreenHight()/2;
	int offsetx = m_2dDraw.GetScreenWidth()/2;
	m_map->SetMapPos(m_Player.GetDrawPoint().x-offsetx,m_Player.GetDrawPoint().y-offsety,offsetx,offsety);
	

	ShowCursor(FALSE);

   return S_OK;
}


//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CDEApp::FrameMove()
{
   sys_verify_hr_return_hr( m_Input.Update());

   if(m_Input.IsKeyDown(DIK_K) && (m_Input.IsKeyDown(DIK_LALT) || m_Input.IsKeyDown(DIK_RALT)))
   {
      static int shot = 0;
      CString ScreenFile;
      ScreenFile.Format("Shot%d.bmp",shot);
      shot++;

      TakeScreenShot(ScreenFile);
   }


   m_map->UpdateTime();
	if(!m_Player.GetInput(&m_Input,m_map,&m_2dDraw))
	{
		return E_FAIL;
	}

	m_Player.AninIcon(m_map->GetElapsedTime());
	if(m_Player.monster==TRUE)
		m_map->m_pmonst->move(&m_Player,m_map,&m_2dDraw);
		

   return S_OK;
}

HRESULT CDEApp::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                        D3DFORMAT Format )
{
   // Make sure device supports point lights
   if( (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING ) ||
      (dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING ) )
   {
      if(pCaps->MaxTextureWidth>=640 && 
         pCaps->MaxTextureHeight>=512 && 
         pCaps->MaxTextureBlendStages>=2 &&
         pCaps->MaxPrimitiveCount>=65535 &&
         pCaps->MaxVertexIndex>=65535)
         return S_OK;
   }
   return E_FAIL;
}

//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CDEApp::FinalCleanup()
{
   CoUninitialize();
   return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CDEApp::DeleteDeviceObjects()
{
   return S_OK;
}
//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT CDEApp::InvalidateDeviceObjects()
{
   return S_OK;
}

HRESULT CDEApp::Render()
{
   m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
            D3DCOLOR_XRGB(0,255,0)//m_BGColour
            , 1.0f, 0L );
   // Begin the scene
   if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
   {
      m_2dDraw.Start(m_pd3dDevice);
      m_map->draw(&m_2dDraw);

      // display all windows
      GetWindowMgr()->Display(m_pd3dDevice, &m_2dDraw);
	   m_curs->draw(&m_2dDraw);
      m_2dDraw.End();

      m_pd3dDevice->EndScene();
   }

   return S_OK;
}

HRESULT CDEApp::RestoreDeviceObjects()
{
   
   return S_OK;
}

HRESULT CDEApp::InitDeviceObjects()
{
   // Initialize the font's internal texture

   // Load an object to cast the shadow 

   m_2dDraw.SetScreenSize(m_dwCreationHeight, m_dwCreationWidth);

   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
   //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
   //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
   //m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
   m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
   m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

   m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
   m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,      TRUE );
   m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );






   return S_OK;
}



BOOL CDEApp::LookForMSG()
{
   BOOL bGotMsg = FALSE;
   while( WM_QUIT != m_msg.message && !bGotMsg )
   {
      // Use PeekMessage() if the app is active, so we can use idle time to
      // render the scene. Else, use GetMessage() to avoid eating CPU time.
      if( m_bActive )
         bGotMsg = PeekMessage( &m_msg, NULL, 0U, 0U, PM_REMOVE );
      else
         bGotMsg = GetMessage( &m_msg, NULL, 0U, 0U );

      if( bGotMsg )
      {
         // Translate and dispatch the message
         TranslateMessage( &m_msg );
         DispatchMessage( &m_msg );
      }
   }
   return bGotMsg;
}



LRESULT CDEApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   switch(uMsg)
   {
   case WM_MOUSEMOVE:
      {
         m_curs->setpos(LOWORD(lParam),HIWORD(lParam));
			CPoint p(LOWORD(lParam),HIWORD(lParam));
			if(!m_WinMgr.MouseOver(p))
			{
				if((MK_LBUTTON&wParam)!=0)
				{
					
					p = m_map->ScreenToMap(p);
					CMonster* pMonst = m_map->m_pmonst->GetMonsterAtPos(p);
					if(pMonst)
					{
						m_Player.SeekMonster(pMonst,m_map);
					}
					else
					{
						CDEStatic* pObj = m_map->GetObjAtPos(p);
						if(pObj)
							m_Player.SeekObject(pObj,m_map);
						else
							m_Player.SeekPoint(p,m_map,TRUE);
					}
				
               m_Input.AddButtonPress(VK_ESCAPE);
				}
			}
         break;
      }
   case WM_LBUTTONDOWN:
      {
         CPoint p(LOWORD(lParam),HIWORD(lParam));
				int key;
				if(m_WinMgr.MouseDown(p,key))
					m_Input.AddButtonPress(key);
				else
				{
					p = m_map->ScreenToMap(p);
					CMonster* pMonst = m_map->m_pmonst->GetMonsterAtPos(p);
					if(pMonst)
					{
						m_Player.SeekMonster(pMonst,m_map);
					}
					else
					{
						CDEStatic* pObj = m_map->GetObjAtPos(p);
						if(pObj)
							m_Player.SeekObject(pObj,m_map);
						else
							m_Player.SeekPoint(p,m_map,TRUE);
					}
					m_Input.AddButtonPress(VK_ESCAPE);
				}
      }

   case WM_RBUTTONDOWN:
		{
			CPoint p(LOWORD(lParam),HIWORD(lParam));
			p = m_map->ScreenToMap(p);
			m_Player.UseSpell(m_map, &m_2dDraw, p);
		}
		break;

   case WM_LBUTTONUP:
      {
         CPoint p(LOWORD(lParam),HIWORD(lParam));
         int Id;
         if(m_WinMgr.MouseUp(p,Id))
            m_Input.AddButtonPress(Id);
         break;
      }

   case WM_KEYDOWN:
		m_Input.AddButtonPress(wParam);
		return 0;

   case WM_CHAR:
      {
         char c = wParam;
         m_WinMgr.CharPressed(c,lParam);
      }
      break;
   }
   return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}


void CDEApp::TakeScreenShot(const char* file_name)
{
   //now we copy the front buffer into our surface
   LPDIRECT3DSURFACE9 pBackBuffer;
   m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

   //now write our screen shot to a bitmap file
   //the last 2 params are NULL because we want the entire front buffer and no palette
   D3DXSaveSurfaceToFile(file_name, D3DXIFF_BMP, pBackBuffer, NULL, NULL);
   pBackBuffer->Release();
}


#define BAR_SIZE 200.0f
#define BAR_HIGHT 20.0f
#define COLOUR_TO D3DCOLOR_XRGB(0,0,0)

#define TOPBAR_FLAG GR_TR|GR_TL
#define BOTTOMBAR_FLAG GR_BR|GR_BL

void CDEApp::DisplayProgress(CString str, FLOAT pcentDone)
{
   m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
      0xff000000, 1.0f, 0L );


   if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
   {
      m_2dDraw.Start(m_pd3dDevice);

      CPoint Pos((m_2dDraw.GetScreenWidth()- 200) / 2,(m_2dDraw.GetScreenHight()-(BAR_HIGHT+15))/2);

      CPoint TextPoint(0,0);
      TextPoint.Offset(Pos);
      m_2dDraw.OutTextc(0,D3DCOLOR_XRGB(255,255,255),TextPoint.x,TextPoint.y,TRUE,"Loading.... %s %2.0f%%",str,pcentDone);


      FLOAT BarLen = pcentDone/100.0f * BAR_SIZE;
      // Draw Shield Bar
      CRect rec(CPoint(0, 15),CSize(BarLen,BAR_HIGHT/2.0f));
      rec.OffsetRect(Pos);
      m_2dDraw.DrawGradientRect(rec,D3DCOLOR_XRGB(0,0,255),COLOUR_TO,TOPBAR_FLAG);

      CRect rec6(CPoint(0, 15+(BAR_HIGHT/2.0f)),CSize(BarLen,BAR_HIGHT/2.0f));
      rec6.OffsetRect(Pos);
      m_2dDraw.DrawGradientRect(rec6,D3DCOLOR_XRGB(0,0,255),COLOUR_TO,BOTTOMBAR_FLAG);

      // Draw Rect around Shield Bar
      CRect rec1(CPoint(0, 15),CSize(BAR_SIZE,BAR_HIGHT));
      rec1.OffsetRect(Pos);
      m_2dDraw.DrawRect(rec1,D3DCOLOR_XRGB(0,0,255));


      m_2dDraw.End();
      m_pd3dDevice->EndScene();
   }
   m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}


