//-----------------------------------------------------------------------------
// File: D3DFile.h
//
// Desc: Support code for loading DirectX .X files.
//
// Copyright (c) 1997-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef D3DFILE_H
#define D3DFILE_H
#include "ZipArchive.h"
#include <atlcomcli.h>
#include <vector>
#include <list>
#include <string>

namespace Xfile
{
   //-----------------------------------------------------------------------------
   // Name: class Mesh
   // Desc: Class for loading and rendering file-based meshes
   //-----------------------------------------------------------------------------
   class Mesh
   {
   public:
      std::string         m_name;

      

      DWORD               m_num_materials; // Materials for the mesh
      D3DMATERIAL9*       m_materials;
      CComPtr<IDirect3DTexture9>* m_textures;
      bool                m_use_materials;

   public:
      // Rendering
      HRESULT render( IDirect3DDevice9* device, 
         bool drawOpaqueSubsets = true,
         bool drawAlphaSubsets = true,
         bool useTexture = true);

      // Rendering options
      VOID    useMeshMaterials( bool use ) { m_use_materials = use; }
      HRESULT setFVF( IDirect3DDevice9* device, DWORD fvf );

      // Initializing
      HRESULT restoreDeviceObjects( IDirect3DDevice9* device );
      HRESULT deleteDeviceObjects();

      // Creation/destruction
      HRESULT create( IDirect3DDevice9* device, const TCHAR* filename , CZipArchive* zip);
      HRESULT create( IDirect3DDevice9* device, ID3DXFileData* filedata, CZipArchive* zip );
      HRESULT destroy();

      void computeBoundingSphere(D3DXVECTOR3* center, FLOAT* radius);

      Mesh( const TCHAR* name = _T("CD3DFile_Mesh") );
      virtual ~Mesh();

   public:
      CComPtr<ID3DXMesh>  m_sysMemMesh;    // SysMem mesh, lives through resize
      CComPtr<ID3DXMesh>  m_localMesh;     // Local mesh, rebuilt on resize
   };

   // X File formation rotate key
   struct SRotateKeyXFile
   {
      DWORD			time;
      DWORD			floats;	
      float			w;
      float			x;
      float			y;
      float			z;
   };

   struct SScaleKeyXFile
   {
      DWORD	time;
      DWORD	floats;	
      D3DXVECTOR3	scale;	
   };


   struct SPositionKeyXFile
   {
      DWORD	time;
      DWORD	floats;	
      D3DXVECTOR3	pos;	
   };

   struct SMatrixKeyXFile
   {
      DWORD	time;
      DWORD	floats;	
      D3DXMATRIX	mat;	
   };


   struct SRotateKey
   {
      DWORD			time;
      D3DXQUATERNION	quatRotate;	
   };

   struct SPositionKey
   {
      DWORD	time;
      D3DXVECTOR3	pos;	
   };

   #pragma warning(disable:4324)

   struct SScaleKey
   {
      DWORD	time;
      D3DXVECTOR3	scale;	
   };

   struct SMatrixKey
   {
      DWORD	time;
      D3DXMATRIXA16	mat;	
   };

   struct SCenterPoints
   {
      D3DXVECTOR3 center;
      FLOAT radius;
   };

   //-----------------------------------------------------------------------------
   // Name: class Frame
   // Desc: Class for loading and rendering file-based meshes
   //-----------------------------------------------------------------------------
   class Frame
   {
   public:
      std::string      m_name;
      D3DXMATRIX m_mat;
      D3DXMATRIX m_matOrig;
      Mesh*  m_mesh;
      std::string      m_mesh_name;

      Frame* m_next;
      Frame* m_child;

      // animation information
      SPositionKey *m_positionKeys;
      UINT m_positionKeys_count;
      SRotateKey *m_rotateKeys;
      UINT m_rotateKeys_count;
      SScaleKey *m_scaleKeys;
      UINT m_scaleKeys_count;
      SMatrixKey *m_matrixKeys;
      UINT m_matrixKeys_count;

      Frame *m_frameAnimNext;
      Frame *m_frameToAnimate;

      bool m_animationFrame;

   public:

      bool findFramePos(const TCHAR* name, D3DXVECTOR3* pos, const D3DXMATRIX* pmatWorld = NULL);

      void resetMatrix();

      void setTime(int minKey,int maxKey,float globalTime);
      void setKey(int key);

      void addFrame(Frame* frame);

      // Matrix access
      VOID  setMatrix( const D3DXMATRIX& pmat ) { m_mat = pmat; m_matOrig = pmat; }
      const D3DXMATRIX& getMatrix()                   { return m_mat; }

      Mesh*   findMesh( const TCHAR* meshName );
      Frame*  findFrame( const TCHAR* frameName );
      bool        enumMeshes( bool (*EnumMeshCB)(Mesh*,const D3DXMATRIX&,VOID*),const D3DXMATRIX& worldMartix, 
         VOID* pContext );

      HRESULT destroy();
      HRESULT restoreDeviceObjects( IDirect3DDevice9* device );
      HRESULT deleteDeviceObjects();
      HRESULT render( IDirect3DDevice9* device, 
         bool drawOpaqueSubsets = true,
         bool drawAlphaSubsets = true,
         const D3DXMATRIX* worldMartix = NULL,
         bool useTexture = true);

      HRESULT setFVF( IDirect3DDevice9* device, DWORD fvf );
      VOID    useMeshMaterials( bool use );

      void computeBoundingSphere(std::vector<SCenterPoints>& points,const D3DXMATRIX& worldMatrix);

      Frame( const TCHAR* name = _T("CD3DFile_Frame") );
      virtual ~Frame();
   };




   //-----------------------------------------------------------------------------
   // Name: class CD3DFile
   // Desc: Class for loading and rendering file-based meshes
   //-----------------------------------------------------------------------------
   class File : public Frame
   {
      HRESULT loadMesh( IDirect3DDevice9* device, ID3DXFileData* filedata, 
         Frame* parentFrame, CZipArchive* zip );
      HRESULT loadFrame( IDirect3DDevice9* device, ID3DXFileData* filedata, 
         Frame* parentFrame, CZipArchive* zip );

      HRESULT loadAnimation(ID3DXFileData* object,
         IDirect3DDevice9* device,
         Frame* parentFrame,
         CZipArchive* zip);
      HRESULT loadAnimationSet(ID3DXFileData* object,
         IDirect3DDevice9* device,
         Frame* parentFrame,
         CZipArchive* zip);
   public:

      void computeBoundingSphere(D3DXVECTOR3* center, FLOAT* radius);

      void setTime(int MinKey,int MaxKey,float fGlobalTime);
      void setKey(int Key);

      void addAnimationFrame(Frame *frame)
      {
         frame->m_frameAnimNext = m_frameAnimNext;
         m_frameAnimNext = frame;
      }

      HRESULT create( IDirect3DDevice9* device, const TCHAR* filename , CZipArchive* zip);
      //HRESULT createFromResource( IDirect3DDevice9* device, const TCHAR* resource, const TCHAR* type, CZipArchive* zip );

      // For pure devices, specify the world transform. If the world transform is not
      // specified on pure devices, this function will fail.
      HRESULT render( IDirect3DDevice9* device, const D3DXMATRIX* morldMatrix,
         bool bUseTexture = true);

      File() : Frame( _T("CD3DFile_Root") ) 
      {
      }

      virtual ~File(){};
   };
}


class MeshCash
{
private:
   MeshCash();

   class CMeshCash
   {
   public:
      Xfile::File* m_mesh;
      std::string m_filename;
      DWORD m_ref_count;
      DWORD m_fvf;
      bool operator == (CMeshCash Other)
      {
         return m_filename == Other.m_filename && m_fvf == Other.m_fvf;
      }
   };

   static std::list<CMeshCash> m_meshCashList;
public:
   static Xfile::File* createMesh( IDirect3DDevice9* device, const TCHAR* filename, DWORD fvf, CZipArchive* zip);
   static void releaseMesh(Xfile::File* mesh);
   static void restoreDeviceObjects(IDirect3DDevice9* device);
   static void deleteDeviceObjects();
};





#endif



