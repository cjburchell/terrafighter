///////////////////////////////////////////////////////////////////////////////
///  vertextypes.h
///
///  PATH: D:\dev2\CommonDev\Include
///
///  CREATED: 02/06/2004 4:37:59 PM by Christiaan Burchell
///
///  PURPOSE:  Vertex Types
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

#ifndef __VERTEXTYPES_H__
#define __VERTEXTYPES_H__
#include "D3DUtil.h"



struct D3DVERTEXUNLIT
{
   D3DXVECTOR3 p;
   D3DCOLOR    color;
   FLOAT       tu, tv;
};

struct MIRRORVERTEX
{
   D3DXVECTOR3 p;
   D3DXVECTOR3 n;
   FLOAT       tu, tv;
   FLOAT       tu1, tv1;
};

struct SHADOWVERTEX
{
   D3DXVECTOR4 p;
   D3DCOLOR    color;
};

struct POINTVERTEX
{
   D3DXVECTOR3 v;
   D3DCOLOR    color;
};

typedef D3DVERTEX TREEVERTEX;

#define D3DFVF_POINTVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_SHADOWVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define D3DFVF_D3DVERTEXUNLIT (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_MIRRORVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2)

#define D3DFVF_TREEVERTEX   D3DFVF_D3DVERTEX
#define D3DFVF_D3DSHIELD    (D3DFVF_XYZ|D3DFVF_DIFFUSE)

/////////////////////////////////////////////////
///
/// NAME: FtoDW
///
/// DESCRIPTION: Helper function to stuff a FLOAT into a DWORD argument
///
/// CREATED:     02/06/2004 4:39:10 PM
///
/// PARAMETERS: 
///             FLOAT f
///
/// RETURN: inline DWORD 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }


template< class T >
inline void Swap( T& a, T& b )//swap the values of a and b
{
   const T temp = a;

   a = b;
   b = temp;
}


template< class T >
class CArray2D
{
public:
   T* pData;//pointer to externally allocated data
   const long Width;//array width

   CArray2D( T* pd, const long w )
      : pData(pd), Width(w)
   {}

   //indexing operator
   //read-only
   const T* operator [] ( const long i ) const		{ return pData + (i*Width); }

   //write-to
   T* operator [] ( const long i )					{ return pData + (i*Width); }
};



/////////////////////////////////////////////////
///
/// NAME: SetColorKey
///
/// DESCRIPTION: Sets the color key of a textrue
///
/// CREATED:     02/06/2004 4:38:40 PM
///
/// PARAMETERS: 
///             DWORD dwColorKey
///             LPDIRECT3DTEXTURE9 pTexture
///
/// RETURN: inline HRESULT 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
inline HRESULT SetColorKey(DWORD dwColorKey,LPDIRECT3DTEXTURE9 pTexture)
{
   D3DCOLOR c = dwColorKey;//D3DCOLOR_XRGB(GetRValue(dwColorKey),GetGValue(dwColorKey),GetBValue(dwColorKey));
   D3DSURFACE_DESC pDesc;
   pTexture->GetLevelDesc(0, &pDesc );

   D3DLOCKED_RECT d3dlr;
   pTexture->LockRect( 0, &d3dlr, 0, 0 );
   DWORD * pDst = (DWORD *)d3dlr.pBits;
   int DPitch = d3dlr.Pitch/4;

   for (DWORD y=0; y<pDesc.Height; ++y)
   {
      for (DWORD x=0; x<pDesc.Width; ++x)
      {
         int index = y*DPitch + x;
         if(pDst[index] != c)
         {
            pDst[index] = 0xff000000 | pDst[index];
         }
         else
         {
            pDst[index] = 0x00ffffff & pDst[index];
         }
      }
   }

   pTexture->UnlockRect (0);
   return S_OK;
}
#endif //__VERTEXTYPES_H__