// SelectShipDlg.cpp: implementation of the CSelectShipDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectShipDlg.h"
#include "Terrafighterapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectShipDlg::CSelectShipDlg(CD3DArcBall* pArcBall,CSpriteDataEx* pSpriteData, DWORD MaxSpriteData,BOOL bAllShips) : CWinDialog()
{
   m_SpriteData = pSpriteData;
   m_MaxSpriteData = MaxSpriteData;
   m_bCanMove = FALSE;
   m_ShipType = 0;
   m_pArcBall = pArcBall;
   m_bAllShips = bAllShips;
}

CSelectShipDlg::~CSelectShipDlg()
{
   m_ShipDisplay.DeleteDeviceObjects();
}


BOOL CSelectShipDlg::ValidButtonID(int Id)
{
   return (Id == IDOK || Id == IDCANCEL);
}

CWinBaceObj* CSelectShipDlg::CreateControl(int id,int classId,const TCHAR* Class,DWORD Style)
{
   switch(id)
   {
      WIN_USE_CTRL(IDC_NEXT,m_next);
      WIN_USE_CTRL(IDC_PREV,m_prev);
      WIN_USE_CTRL(IDC_STATS_TEXT,m_Stats);
      WIN_USE_CTRL(IDC_BITMAP,m_bmp);
      WIN_USE_CTRL(IDC_PLAYER_NAME,m_PlayerName);
   }
   return NULL;
}

BOOL CSelectShipDlg::Create(C2DDraw *pDisplay, const CPoint& p)
{
   LoadDialog(IDD_SELECT_SHIP,pDisplay);
   m_next.SetType(WINARROW_RIGHT);
   m_prev.SetType(WINARROW_LEFT);

   if(!CWinDialog::Create(pDisplay,NULL,p,GetSize(),GetText()))
      return FALSE;

   UpdateControls();

   m_ShipDisplay.m_bColorKey = TRUE;
   m_ShipDisplay.CreateRenderSurface(m_bmp.GetSize().cx,m_bmp.GetSize().cy,pDisplay->GetTempDevice(),GetApp()->GetModeInfo()->DepthStencilFormat);
   m_bmp.SetBitmap(&m_ShipDisplay);

   UpdateDisplay();

   return TRUE;
}

void CSelectShipDlg::SetStats(CSpriteDataEx* pData)
{
   CString stats;
   stats.Format("Turning Rate:\t%2.2f\nDrag:\t%2.2f\nAccelration:\t%2.2f\nSpeed:\t%2.2f\nName:\t%s",
      pData->m_fTurningRate,
      pData->m_Drag,
      pData->m_fAcceleration,
      pData->m_fMaxSpeed,
      pData->m_Name);

   m_Stats.SetText(stats);
}

void CSelectShipDlg::OnButtonPressed(int ButtonID)
{
   switch(ButtonID)
   {
   case IDC_NEXT:
      m_ShipType++;
      if(m_ShipType >= m_MaxSpriteData)
         m_ShipType = 0;

      while(!IsValidShip(m_ShipType))
      {
         m_ShipType++;
         if(m_ShipType >= m_MaxSpriteData)
            m_ShipType = 0;
      }
      UpdateControls();
      UpdateDisplay();
      break;
   case IDC_PREV:
      m_ShipType--;
      if(m_ShipType < 0)
         m_ShipType = m_MaxSpriteData-1;

      while(!IsValidShip(m_ShipType))
      {
         m_ShipType--;
         if(m_ShipType >= m_MaxSpriteData)
            m_ShipType = m_MaxSpriteData-1;
      }
      UpdateControls();
      UpdateDisplay();
      break;
   }
}


void CSelectShipDlg::UpdateControls()
{
   SetStats(&m_SpriteData[m_ShipType]);
}

void CSelectShipDlg::UpdateDisplay()
{
   LPDIRECT3DDEVICE9 pd3dDevice = GetDisplay()->GetTempDevice();

   sys_verify_hr(m_ShipDisplay.BeginScene());

   sys_verify_hr(pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
      0x00000000, 1.0f, 0L ));

   pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      FALSE );



   D3DXMATRIX matProj;
   FLOAT fAspect = m_bmp.GetSize().cx/m_bmp.GetSize().cy;
   D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4.0f, fAspect, 1.0f, FOV_BACK_DISTACNE+128.0f );
   pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );


   CD3DCamera Camera;
   D3DXVECTOR3 vEyePt = D3DXVECTOR3(0,0,-2.0f);
   D3DXVECTOR3 vLookatPt = D3DXVECTOR3(0,0,0);
   D3DXVECTOR3 vUpVec = D3DXVECTOR3(0,1,0);
   Camera.SetViewParams( vEyePt, vLookatPt, vUpVec);
   pd3dDevice->SetTransform( D3DTS_VIEW, &Camera.GetViewMatrix());

   D3DXMATRIX matFinal, matTrans;
   FLOAT Scale = m_SpriteData[m_ShipType].m_fScale;
   D3DXMatrixScaling( &matFinal, Scale, Scale, Scale );

   if(m_SpriteData[m_ShipType].m_yMeshRot!= 0.0f)
   {
      D3DXMATRIX matRotateY;
      D3DXMatrixRotationZ( &matRotateY, m_SpriteData[m_ShipType].m_yMeshRot);
      D3DXMatrixMultiply( &matFinal, &matFinal, &matRotateY );
   }

   if(m_SpriteData[m_ShipType].m_zMeshRot!= 0.0f)
   {
      D3DXMATRIX matRotateZ;
      D3DXMatrixRotationX( &matRotateZ, m_SpriteData[m_ShipType].m_zMeshRot);
      D3DXMatrixMultiply( &matFinal, &matFinal, &matRotateZ );
   }

   D3DXMatrixMultiply( &matFinal, &matFinal, m_pArcBall->GetRotationMatrix() );


   D3DXMatrixTranslation( &matTrans,0, 0, 0 );
   D3DXMatrixMultiply( &matFinal, &matFinal, &matTrans );
   //pd3dDevice->SetTransform( D3DTS_WORLD,      &matFinal ); // mesh file dose this



   pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
   pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
   pd3dDevice->SetTexture( 0, NULL );
   m_SpriteData[m_ShipType].Render(pd3dDevice,&matFinal);
   pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );


   pd3dDevice->SetRenderState( D3DRS_LIGHTING,      FALSE );
   m_ShipDisplay.EndScene();
   m_ShipDisplay.m_bColorKey = TRUE;
}

void CSelectShipDlg::OnMouseOver(BOOL bIsOver, const CPoint& p)
{
   UpdateDisplay();
   CWinDialog::OnMouseOver(bIsOver,p);
}
