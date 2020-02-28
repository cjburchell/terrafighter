// Bush.h: interface for the CBush class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUSH_H__320BDEC8_DD53_4E53_9DA1_2C709A2C48AB__INCLUDED_)
#define AFX_BUSH_H__320BDEC8_DD53_4E53_9DA1_2C709A2C48AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vegetation.h"

class CBush : public CVegetation  
{
public:
   CBush(FLOAT fWidth, FLOAT fHeight, const char* TextureFile);
   virtual ~CBush();
   virtual HRESULT RenderObj(LPDIRECT3DDEVICE9 pd3dDevice,CFOVClipper* pClipper);
   void SetObjData(CObjPos* pObjData);

private:
   DWORD m_NumbSubItems;
   CSubObj m_SubItems[MAX_SUB_ITEMS];

   DWORD m_LastTick;

};

#endif // !defined(AFX_BUSH_H__320BDEC8_DD53_4E53_9DA1_2C709A2C48AB__INCLUDED_)
