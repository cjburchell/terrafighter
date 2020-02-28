//-----------------------------------------------------------------------------
// File: D3DFile.cpp
//
// Desc: Support code for loading DirectX .X files.
//
// Copyright (c) 1997-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT

#include "stdafx.h"
#include <initguid.h>
#include <rmxfguid.h>
#include <rmxftmpl.h>

#include "D3DFile.h"
#include "D3DUtil.h"
#include "DXUtil.h"

#include <list>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::list<MeshCash::CMeshCash> MeshCash::m_meshCashList;



Xfile::File* MeshCash::createMesh( IDirect3DDevice9* device, const TCHAR* filename, DWORD fvf, CZipArchive* zip)
{
   Xfile::File* result = NULL;

   // find a cashed Mesh
   for(std::list<CMeshCash>::iterator i = m_meshCashList.begin();i!=m_meshCashList.end();i++)
   {
      DWORD tempfvf = i->m_fvf;
      if(i->m_filename == filename && fvf == i->m_fvf)
      {
         i->m_ref_count++;
         return i->m_mesh;
      }
   }
   sys_msg(filename);

   result = new Xfile::File;

   sys_verify_hr_return(result->create( device, filename, zip),NULL);

   sys_verify_hr(result->setFVF( device, D3DFVF_D3DVERTEX ));
   sys_verify_hr(result->restoreDeviceObjects(device));

   CMeshCash Cash;
   Cash.m_filename = filename;
   Cash.m_mesh = result;
   Cash.m_ref_count = 1;
   Cash.m_fvf = fvf;
   m_meshCashList.push_back(Cash);
   //sys_printf(__LOCATION__, 0,"DEBUG: Creating  %s\n",Cash.m_filename.c_str());


   return result;
}



void MeshCash::releaseMesh(Xfile::File* mesh)
{
   if(mesh == NULL)
      return;

   for(std::list<CMeshCash>::iterator i = m_meshCashList.begin();i!=m_meshCashList.end();i++)
   {
      if(i->m_mesh == mesh)
      {
         i->m_ref_count--;
         if(i->m_ref_count == 0)
         {
            CMeshCash Cash = *i;
            //sys_printf(__LOCATION__,0,"Deleteing %s\n",i->m_filename);
            i->m_mesh->destroy();
            SAFE_DELETE(i->m_mesh);
            m_meshCashList.remove(Cash);
         }
         return;
      }
   }

   /*if(mesh)
   {
   mesh->InvalidateDeviceObjects();
   mesh->Destroy();
   SAFE_DELETE(mesh);
   }*/
}

void MeshCash::restoreDeviceObjects(IDirect3DDevice9* device)
{
   for(std::list<CMeshCash>::iterator i = m_meshCashList.begin();i!=m_meshCashList.end();i++)
   {
      i->m_mesh->restoreDeviceObjects(device);
   }
}

void MeshCash::deleteDeviceObjects()
{
   for(std::list<CMeshCash>::iterator i = m_meshCashList.begin();i!=m_meshCashList.end();i++)
   {
      i->m_mesh->destroy();
      SAFE_DELETE(i->m_mesh);
   }
   m_meshCashList.clear();
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
Xfile::Mesh::Mesh( const TCHAR* name )
{
   m_name = name;
   m_sysMemMesh        = NULL;
   m_localMesh         = NULL;
   m_num_materials     = 0L;
   m_materials         = NULL;
   m_textures          = NULL;
   m_use_materials     = true;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
Xfile::Mesh::~Mesh()
{
   destroy();
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Mesh::create( IDirect3DDevice9* device, const TCHAR* filename, CZipArchive* zip)
{
   CComPtr<ID3DXBuffer> adjacencyBuffer = NULL;
   CComPtr<ID3DXBuffer> mtrlBuffer = NULL;
   CComPtr<ID3DXBuffer> effectInstances = NULL;
   HRESULT      hr;

   // Find the path for the file, and convert it to ANSI (for the D3DX API)

   hr = E_FAIL;

   if(zip)
   {
      if(zip->OpenFile(filename))
      {

         CZipFileHeader fh;
         zip->GetFileInfo(fh, zip->FindFile(filename));
         int filesize = fh.m_uUncomprSize;

         BYTE* pBuffer = new BYTE[filesize];
         memset(pBuffer,0,filesize);
         int retval = zip->ReadFile(pBuffer,filesize);
         if( retval > 0)
         {
            // Create the texture using D3DX
            hr = D3DXLoadMeshFromXInMemory( pBuffer,filesize, D3DXMESH_SYSTEMMEM, device, 
               &adjacencyBuffer, &mtrlBuffer, &effectInstances,
               &m_num_materials, &m_sysMemMesh );
         }
         delete [] pBuffer;

         zip->CloseFile();
      }
   }
   else
   {
      // Load the mesh
      if( FAILED( hr = D3DXLoadMeshFromX( filename, D3DXMESH_SYSTEMMEM, device, 
         &adjacencyBuffer, &mtrlBuffer, &effectInstances,
         &m_num_materials, &m_sysMemMesh ) ) )
      {
         return hr;
      }
   }

   if( FAILED( hr  ) )
   {
      return hr;
   }


   // Optimize the mesh for performance
   if( FAILED( hr = m_sysMemMesh->OptimizeInplace(
      D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
      (DWORD*)adjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
   {
      return hr;
   }


   // Get material info for the mesh
   // Get the array of materials out of the buffer
   if( mtrlBuffer && m_num_materials > 0 )
   {
      // Allocate memory for the materials and textures
      D3DXMATERIAL* materials = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
      m_materials = new D3DMATERIAL9[m_num_materials];
      m_textures  = new CComPtr<IDirect3DTexture9>[m_num_materials];

      // Copy each material and create its texture
      for( DWORD i=0; i<m_num_materials; i++ )
      {
         // Copy the material
         m_materials[i]         = materials[i].MatD3D;
         m_materials[i].Ambient = m_materials[i].Diffuse;
         m_textures[i]          = NULL;

         // Create a texture
         if( materials[i].pTextureFilename )
         {
            TCHAR strTexture[MAX_PATH];
            DXUtil::ConvertAnsiStringToGeneric( strTexture, materials[i].pTextureFilename );

            if( FAILED( D3DUtil::CreateTexture( device, strTexture, 
               m_textures[i], zip ) ) )
               m_textures[i] = NULL;
         }
      }
   }
   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Mesh::create( IDirect3DDevice9* device,
                         ID3DXFileData* pFileData , CZipArchive* zip)
{
   CComPtr<ID3DXBuffer> mtrlBuffer = NULL;
   CComPtr<ID3DXBuffer> adjacencyBuffer = NULL;
   CComPtr<ID3DXBuffer> effectInstances = NULL;
   HRESULT      hr;

   // Load the mesh from the DXFILEDATA object
   if( FAILED( hr = D3DXLoadMeshFromXof( pFileData, D3DXMESH_MANAGED, device,
      &adjacencyBuffer, &mtrlBuffer, &effectInstances,
      &m_num_materials, &m_sysMemMesh ) ) )
   {
      return hr;
   }

   // Optimize the mesh for performance
   if( FAILED( hr = m_sysMemMesh->OptimizeInplace(
      D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
      (DWORD*)adjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
   {
      return hr;
   }

   // Get material info for the mesh
   // Get the array of materials out of the buffer
   if( mtrlBuffer && m_num_materials > 0 )
   {
      // Allocate memory for the materials and textures
      D3DXMATERIAL* materials = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
      m_materials = new D3DMATERIAL9[m_num_materials];
      m_textures  = new CComPtr<IDirect3DTexture9>[m_num_materials];

      // Copy each material and create its texture
      for( DWORD i=0; i<m_num_materials; i++ )
      {
         // Copy the material
         m_materials[i]         = materials[i].MatD3D;
         m_materials[i].Ambient = m_materials[i].Diffuse;
         m_textures[i]          = NULL;

         // Create a texture
         if( materials[i].pTextureFilename )
         {
            TCHAR strTexture[MAX_PATH];
            DXUtil::ConvertAnsiStringToGeneric( strTexture, materials[i].pTextureFilename );

            if( FAILED( D3DUtil::CreateTexture( device, strTexture, 
               m_textures[i], zip ) ) )
               m_textures[i] = NULL;
         }
      }
   }

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Mesh::setFVF( IDirect3DDevice9* device, DWORD fvf )
{
   CComPtr<ID3DXMesh> tempSysMemMesh = NULL;
   CComPtr<ID3DXMesh> tempLocalMesh  = NULL;

   if( m_sysMemMesh )
   {
      if( FAILED( m_sysMemMesh->CloneMeshFVF( D3DXMESH_SYSTEMMEM, fvf,
         device, &tempSysMemMesh ) ) )
         return E_FAIL;
   }
   if( m_localMesh )
   {
      if( FAILED( m_localMesh->CloneMeshFVF( 0L, fvf, device,
         &tempLocalMesh ) ) )
      {
         return E_FAIL;
      }
   }

   m_sysMemMesh = NULL;
   m_localMesh = NULL;

   if( tempSysMemMesh ) m_sysMemMesh = tempSysMemMesh;
   if( tempLocalMesh )  m_localMesh  = tempLocalMesh;

   // Compute normals in case the meshes have them
   if( m_sysMemMesh )
      D3DXComputeNormals( m_sysMemMesh, NULL );
   if( m_localMesh )
      D3DXComputeNormals( m_localMesh, NULL );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Mesh::restoreDeviceObjects( IDirect3DDevice9* device )
{
   if( m_sysMemMesh == NULL )
      return E_FAIL;

   // Make a local memory version of the mesh. Note: because we are passing in
   // no flags, the default behavior is to clone into local memory.


   m_localMesh = NULL;

   if( FAILED( m_sysMemMesh->CloneMeshFVF( 0L, m_sysMemMesh->GetFVF(),
      device, &m_localMesh ) ) )
      return E_FAIL;

   return S_OK;

}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Mesh::deleteDeviceObjects()
{
   m_localMesh = NULL;
   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Mesh::destroy()
{
   deleteDeviceObjects();
   for( UINT i=0; i<m_num_materials; i++ )
   {
      D3DUtil::ReleaseTexture( m_textures[i] );
      m_textures[i] = NULL;
   }
   SAFE_DELETE_ARRAY( m_textures );
   SAFE_DELETE_ARRAY( m_materials );

   m_sysMemMesh = NULL;

   m_num_materials = 0L;

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Mesh::render( IDirect3DDevice9* device, bool drawOpaqueSubsets,
                         bool drawAlphaSubsets,bool useTexture )
{
   if( m_localMesh == NULL)
      return E_FAIL;

   // Frist, draw the subsets without alpha
   if( drawOpaqueSubsets )
   {
      for( DWORD i=0; i<m_num_materials; i++ )
      {
         if( m_use_materials )
         {
            if( m_materials[i].Diffuse.a < 1.0f )
               continue;
            device->SetMaterial( &m_materials[i] );
            if(useTexture)
               device->SetTexture( 0, m_textures[i] );
         }
         m_localMesh->DrawSubset( i );
      }
   }

   // Then, draw the subsets with alpha
   if( drawAlphaSubsets && m_use_materials )
   {
      for( DWORD i=0; i<m_num_materials; i++ )
      {
         if( m_materials[i].Diffuse.a == 1.0f )
            continue;

         // Set the material and texture
         device->SetMaterial( &m_materials[i] );
         if(useTexture)
            device->SetTexture( 0, m_textures[i] );
         m_localMesh->DrawSubset( i );
      }
   }

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
Xfile::Frame::Frame( const TCHAR* name )
{
   m_name = name;
   D3DXMatrixIdentity( &m_mat );
   m_mesh  = NULL;

   m_child = NULL;
   m_next  = NULL;

   D3DXMatrixIdentity(&m_matOrig);

   m_positionKeys = NULL;
   m_positionKeys_count = 0;
   m_rotateKeys= NULL;
   m_rotateKeys_count = 0;;
   m_scaleKeys = NULL;
   m_scaleKeys_count = 0;
   m_matrixKeys = NULL;
   m_matrixKeys_count = 0;

   m_frameAnimNext = NULL;
   m_frameToAnimate = NULL;

   m_animationFrame = false;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
Xfile::Frame::~Frame()
{
   SAFE_DELETE( m_child );
   SAFE_DELETE( m_next );
   SAFE_DELETE_ARRAY(m_positionKeys);
   SAFE_DELETE_ARRAY(m_rotateKeys);
   SAFE_DELETE_ARRAY(m_scaleKeys);
   SAFE_DELETE_ARRAY(m_matrixKeys);
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
bool Xfile::Frame::enumMeshes( bool (*EnumMeshCB)(Xfile::Mesh*,const D3DXMATRIX& ,VOID*),
                           const D3DXMATRIX& pmatWorldMatrix,VOID* pContext )
{
   D3DXMATRIX matWorld;
   D3DXMatrixMultiply( &matWorld, &m_mat, &pmatWorldMatrix);
   if( m_mesh )
      EnumMeshCB( m_mesh,matWorld, pContext );
   if( m_child )
      m_child->enumMeshes( EnumMeshCB, matWorld, pContext );
   if( m_next )
      m_next->enumMeshes( EnumMeshCB,pmatWorldMatrix, pContext );

   return true;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
Xfile::Mesh* Xfile::Frame::findMesh( const TCHAR* meshName )
{
   Xfile::Mesh* mesh;

   if( m_mesh )
      if( m_mesh->m_name == meshName )
         return m_mesh;

   if( m_child )
      if( NULL != ( mesh = m_child->findMesh( meshName ) ) )
         return mesh;

   if( m_next )
      if( NULL != ( mesh = m_next->findMesh( meshName ) ) )
         return mesh;

   return NULL;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
Xfile::Frame* Xfile::Frame::findFrame( const TCHAR* frameName )
{
   Xfile::Frame* pFrame;

   if( m_name == frameName  )
      return this;

   if( m_child )
      if( NULL != ( pFrame = m_child->findFrame( frameName ) ) )
         return pFrame;

   if( m_next )
      if( NULL != ( pFrame = m_next->findFrame( frameName ) ) )
         return pFrame;

   return NULL;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Frame::destroy()
{
   if( m_mesh )  m_mesh->destroy();
   if( m_child ) m_child->destroy();
   if( m_next )  m_next->destroy();

   SAFE_DELETE( m_mesh );
   SAFE_DELETE( m_next );
   SAFE_DELETE( m_child );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Frame::restoreDeviceObjects( IDirect3DDevice9* device )
{
   if( m_mesh )  m_mesh->restoreDeviceObjects( device );
   if( m_child ) m_child->restoreDeviceObjects( device );
   if( m_next )  m_next->restoreDeviceObjects( device );
   return S_OK;
}

HRESULT Xfile::Frame::setFVF( IDirect3DDevice9* device, DWORD fvf )
{
   if( m_mesh )  m_mesh->setFVF( device,fvf );
   if( m_child ) m_child->setFVF( device,fvf );
   if( m_next )  m_next->setFVF( device,fvf );
   return S_OK;
}

VOID Xfile::Frame::useMeshMaterials( bool use )
{
   if( m_mesh )  m_mesh->useMeshMaterials( use );
   if( m_child ) m_child->useMeshMaterials( use );
   if( m_next )  m_next->useMeshMaterials( use );
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Frame::deleteDeviceObjects()
{
   if( m_mesh )  m_mesh->deleteDeviceObjects();
   if( m_child ) m_child->deleteDeviceObjects();
   if( m_next )  m_next->deleteDeviceObjects();
   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::Frame::render( IDirect3DDevice9* device, bool drawOpaqueSubsets,
                          bool drawAlphaSubsets, const D3DXMATRIX* worldMatrix, bool useTexture )
{
   // For pure devices, specify the world transform. If the world transform is not
   // specified on pure devices, this function will fail.

   D3DXMATRIX savedWorld, world;

   if ( NULL == worldMatrix )
      device->GetTransform( D3DTS_WORLD, &savedWorld );
   else
      savedWorld = *worldMatrix;

   D3DXMatrixMultiply( &world, &m_mat, &savedWorld );
   device->SetTransform( D3DTS_WORLD, &world );

   if( m_mesh )
      m_mesh->render( device, drawOpaqueSubsets, drawAlphaSubsets, useTexture );

   if( m_child )
      m_child->render( device, drawOpaqueSubsets, drawAlphaSubsets, &world, useTexture );

   device->SetTransform( D3DTS_WORLD, &savedWorld );

   if( m_next )
      m_next->render( device, drawOpaqueSubsets, drawAlphaSubsets, &savedWorld, useTexture );

   return S_OK;
}

HRESULT Xfile::File::loadAnimation(ID3DXFileData* object,
                                IDirect3DDevice9* device,
                                Xfile::Frame* pParentFrame,
                                CZipArchive* zip)
{
   HRESULT hr = S_OK;

   CHAR  strAnsiName[512] = "";
   TCHAR strName[MAX_PATH];
   DWORD dwNameLength;
   object->GetName( NULL, &dwNameLength );
   if( dwNameLength > 0 )
      object->GetName( strAnsiName, &dwNameLength );
   DXUtil::ConvertAnsiStringToGeneric( strName, strAnsiName );


   Frame* pframeCur = new Frame(strName);

   sys_verify_return(pframeCur == NULL, E_OUTOFMEMORY);

   pframeCur->m_animationFrame = true;

   pParentFrame->addFrame(pframeCur);
   addAnimationFrame(pframeCur);

   // Enumerate child objects.
   // Child object can be data, data reference or binary.
   // Use QueryInterface() to find what type of object a child is.

   SIZE_T numb_children;
   object->GetChildren(&numb_children);
   for (SIZE_T i = 0; i<numb_children; i++)
   {
      CComPtr<ID3DXFileData> child;
      object->GetChild(i, &child);

      if ( child->IsReference() )
      {
         GUID type;
         sys_verify_hr_return_hr(child->GetType(&type));

         if( TID_D3DRMFrame == type )
         {
            sys_verify_return(pframeCur->m_frameToAnimate != NULL, E_INVALIDARG);

            DWORD cchName;
            sys_verify_hr_return_hr(child->GetName(NULL, &cchName));
            sys_verify_return(cchName == 0, E_INVALIDARG);

            char *szFrameName = (char*)_alloca(cchName);
            sys_verify_return(szFrameName == NULL, E_OUTOFMEMORY);

            sys_verify_hr_return_hr(child->GetName(szFrameName, &cchName));

            pframeCur->m_frameToAnimate = findFrame(szFrameName);
            sys_verify_return(pframeCur->m_frameToAnimate == NULL, E_INVALIDARG);
         }
      }
      else
      {
         GUID type;
         sys_verify_hr_return_hr(child->GetType(&type));

         if ( TID_D3DRMFrame == type )
         {
            hr = loadFrame(device, child, pframeCur,zip);
            if (FAILED(hr))
               return hr;
         }
         else if ( TID_D3DRMAnimationOptions == type )
         {
            //ParseAnimOptions(pChildData,pParentFrame);
            //i=2;
         }
         else if ( TID_D3DRMAnimationKey == type )
         {
            DWORD dwSize;
            PBYTE pData;
            hr = child->Lock( &dwSize, (LPCVOID*)&pData );
            if (FAILED(hr))
               return hr;

            DWORD dwKeyType = ((DWORD*)pData)[0];
            DWORD cKeys = ((DWORD*)pData)[1];

            if (dwKeyType == 0)
            {
               if (pframeCur->m_rotateKeys != NULL)
               {
                  return E_INVALIDARG;
               }

               pframeCur->m_rotateKeys = new SRotateKey[cKeys];
               if (pframeCur->m_rotateKeys == NULL)
               {
                  return E_OUTOFMEMORY;
               }

               pframeCur->m_rotateKeys_count = cKeys;
               //NOTE x files are w x y z and QUATERNIONS are x y z w

               SRotateKeyXFile* pFileRotateKey =  (SRotateKeyXFile*)(pData + (sizeof(DWORD) * 2));
               for (DWORD iKey = 0;iKey < cKeys; iKey++)
               {
                  pframeCur->m_rotateKeys[iKey].time = pFileRotateKey->time;
                  pframeCur->m_rotateKeys[iKey].quatRotate.x = pFileRotateKey->x;
                  pframeCur->m_rotateKeys[iKey].quatRotate.y = pFileRotateKey->y;
                  pframeCur->m_rotateKeys[iKey].quatRotate.z = pFileRotateKey->z;
                  pframeCur->m_rotateKeys[iKey].quatRotate.w = pFileRotateKey->w;

                  pFileRotateKey += 1;
               }
            }
            else if (dwKeyType == 1)
            {
               if (pframeCur->m_scaleKeys != NULL)
               {
                  return E_INVALIDARG;
               }

               pframeCur->m_scaleKeys = new SScaleKey[cKeys];
               if (pframeCur->m_scaleKeys == NULL)
               {
                  return E_OUTOFMEMORY;
               }

               pframeCur->m_scaleKeys_count = cKeys;

               SScaleKeyXFile* pFileScaleKey =  (SScaleKeyXFile*)(pData + (sizeof(DWORD) * 2));
               for (DWORD iKey = 0;iKey < cKeys; iKey++)
               {
                  pframeCur->m_scaleKeys[iKey].time = pFileScaleKey->time;
                  pframeCur->m_scaleKeys[iKey].scale = pFileScaleKey->scale;

                  pFileScaleKey += 1;
               }
            }
            else if (dwKeyType == 2)
            {
               if (pframeCur->m_positionKeys != NULL)
               {
                  return E_INVALIDARG;
               }

               pframeCur->m_positionKeys = new SPositionKey[cKeys];
               if (pframeCur->m_positionKeys == NULL)
               {
                  return E_OUTOFMEMORY;
               }

               pframeCur->m_positionKeys_count = cKeys;

               SPositionKeyXFile* pFilePosKey =  (SPositionKeyXFile*)(pData + (sizeof(DWORD) * 2));
               for (DWORD iKey = 0;iKey < cKeys; iKey++)
               {
                  pframeCur->m_positionKeys[iKey].time = pFilePosKey->time;
                  pframeCur->m_positionKeys[iKey].pos = pFilePosKey->pos;

                  pFilePosKey += 1;
               }
            }
            else if (dwKeyType == 4)
            {
               if (pframeCur->m_matrixKeys != NULL)
               {
                  return E_INVALIDARG;
               }

               pframeCur->m_matrixKeys = new SMatrixKey[cKeys];
               if (pframeCur->m_matrixKeys == NULL)
               {
                  return E_OUTOFMEMORY;
               }

               pframeCur->m_matrixKeys_count = cKeys;

               SMatrixKeyXFile* pFileMatrixKey =  (SMatrixKeyXFile*)(pData + (sizeof(DWORD) * 2));
               for (DWORD iKey = 0;iKey < cKeys; iKey++)
               {
                  pframeCur->m_matrixKeys[iKey].time = pFileMatrixKey->time;
                  pframeCur->m_matrixKeys[iKey].mat = pFileMatrixKey->mat;

                  pFileMatrixKey += 1;
               }
            }
            else
            {
               return E_INVALIDARG;
            }
         }
      }
   }

   return hr;
}




HRESULT Xfile::File::loadAnimationSet(ID3DXFileData* object,
                                   IDirect3DDevice9* device,
                                   Xfile::Frame* pParentFrame,
                                   CZipArchive* zip)
{
   HRESULT hr = S_OK;

   CHAR  strAnsiName[512] = "";
   TCHAR strName[MAX_PATH];
   DWORD dwNameLength;
   object->GetName( NULL, &dwNameLength );
   if( dwNameLength > 0 )
      object->GetName( strAnsiName, &dwNameLength );
   DXUtil::ConvertAnsiStringToGeneric( strName, strAnsiName );


   Xfile::Frame* pframeCur = new Xfile::Frame(strName);
   if (pframeCur == NULL)
   {
      return E_OUTOFMEMORY;
   }
   pframeCur->m_animationFrame = true;

   pParentFrame->addFrame(pframeCur);

   // Enumerate child objects.
   // Child object can be data, data reference or binary.
   // Use QueryInterface() to find what type of object a child is.

   SIZE_T numb_children;
   object->GetChildren(&numb_children);
   for (SIZE_T i = 0; i<numb_children; i++)
   {
      CComPtr<ID3DXFileData> child;
      object->GetChild(i, &child);

      // Query the child for it's FileData
      CComQIPtr<IDirectXFileData, &IID_IDirectXFileData> childData(child);
      if ( childData )
      {
         const GUID *type;
         hr = childData->GetType(&type);
         if (FAILED(hr))
            return hr;

         if( TID_D3DRMAnimation == *type )
         {
            hr = loadAnimation(child, device, pframeCur,zip);
            if (FAILED(hr))
               return hr;
         }
      }
   }

   return hr;
}



//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::File::loadFrame( IDirect3DDevice9* device,
                            ID3DXFileData* pFileData,
                            Xfile::Frame* pParentFrame,
                            CZipArchive* zip )
{
   // Get the type of the object

   GUID guid;
   sys_verify_hr_return_hr(pFileData->GetType( &guid ) );


   if( guid == TID_D3DRMMesh )
   {
      sys_verify_hr_return_hr(loadMesh( device, pFileData, pParentFrame ,zip));
   }
   if( guid == TID_D3DRMFrameTransformMatrix )
   {
      D3DXMATRIX* pmatMatrix;
      DWORD  cbSize;
      sys_verify_hr_return_hr(pFileData->Lock(&cbSize, (void const**)&pmatMatrix));

      // Update the parent's matrix with the new one
      pParentFrame->setMatrix( *pmatMatrix );

      sys_verify_hr(pFileData->Unlock());
   }
   if (guid == TID_D3DRMAnimationSet)
   {
      loadAnimationSet(pFileData, device, pParentFrame,zip);
   }
   if (guid == TID_D3DRMAnimation)
   {
      loadAnimation(pFileData, device, pParentFrame, zip);
   }
   if( guid == TID_D3DRMFrame )
   {
      // Get the frame name
      CHAR  strAnsiName[512] = "";
      TCHAR strName[MAX_PATH];
      DWORD dwNameLength;
      pFileData->GetName( NULL, &dwNameLength );
      if( dwNameLength > 0 )
         pFileData->GetName( strAnsiName, &dwNameLength );
      DXUtil::ConvertAnsiStringToGeneric( strName, strAnsiName );

      // Create the frame
      Xfile::Frame*  pCurrentFrame = new Xfile::Frame( strName );

      pParentFrame->addFrame(pCurrentFrame);

      // Enumerate child objects

      SIZE_T numb_children;
      pFileData->GetChildren(&numb_children);
      for (SIZE_T i = 0; i<numb_children; i++)
      {
         CComPtr<ID3DXFileData> child;
         sys_verify_hr(pFileData->GetChild(i, &child));
         // Query the child for its FileData
         if( child )
         {
            sys_verify_hr_return_hr(loadFrame( device, child, pCurrentFrame,zip ));
         }
      }
   }

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::File::loadMesh( IDirect3DDevice9* device,
                           ID3DXFileData* pFileData,
                           Xfile::Frame* pParentFrame , CZipArchive* zip)
{
   // Currently only allowing one mesh per frame
   if( pParentFrame->m_mesh )
   {
      sys_msg(pParentFrame->m_mesh->m_name.c_str());
      return E_FAIL;
   }

   // Get the mesh name
   CHAR  strAnsiName[512] = {0};
   TCHAR strName[MAX_PATH];
   DWORD dwNameLength;
   pFileData->GetName( NULL, &dwNameLength );
   if( dwNameLength > 0 )
      pFileData->GetName( strAnsiName, &dwNameLength );
   DXUtil::ConvertAnsiStringToGeneric( strName, strAnsiName );

   // Create the mesh
   pParentFrame->m_mesh = new Xfile::Mesh( strName );
   pParentFrame->m_mesh->create( device, pFileData , zip );

   return S_OK;
}




/*
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::File::createFromResource( IDirect3DDevice9* device, const TCHAR* strResource, const TCHAR* strType, CZipArchive* zip )
{
   HRESULT hr;

   // Create a x file object
   CComPtr<IDirectXFile>           pDXFile;
   if( FAILED( hr = DirectXFileCreate( &pDXFile ) ) )
      return E_FAIL;

   // Register templates for d3drm and patch extensions.
   if( FAILED( hr = pDXFile->RegisterTemplates( (VOID*)D3DRM_XTEMPLATES,
      D3DRM_XTEMPLATE_BYTES ) ) )
   {
      return E_FAIL;
   }

   CHAR strTypeAnsi[MAX_PATH];
   DXUtil::ConvertGenericStringToAnsi( strTypeAnsi, strType );

   DXFILELOADRESOURCE dxlr;
   dxlr.hModule = NULL;
   dxlr.lpName = strResource;
   dxlr.lpType = (TCHAR*) strTypeAnsi;

   // Create enum object
   CComPtr<IDirectXFileEnumObject> pEnumObj;
   hr = pDXFile->CreateEnumObject( (VOID*)&dxlr, DXFILELOAD_FROMRESOURCE, 
      &pEnumObj );
   if( FAILED(hr) )
   {
      return hr;
   }

   // Enumerate top level objects (which are always frames)
   CComPtr<IDirectXFileData>   pFileData;
   while( SUCCEEDED( pEnumObj->GetNextDataObject( &pFileData ) ) )
   {
      CComQIPtr<ID3DXFileData, &IID_ID3DXFileData> pDxFileData(pFileData);
      hr = loadFrame( device, pDxFileData, this, zip );
      if( FAILED(hr) )
      {
         return E_FAIL;
      }
   }

   return S_OK;
}*/




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::File::create( IDirect3DDevice9* device, const TCHAR* filename,CZipArchive* zip )
{
   
   // Create a x file object
   CComPtr<ID3DXFile>           pDXFile;
   sys_verify_hr_return_hr(D3DXFileCreate( &pDXFile ) );

   // Register templates for d3drm and patch extensions.
   
   sys_verify_hr_return_hr(pDXFile->RegisterTemplates( (void*)D3DRM_XTEMPLATES,
      D3DRM_XTEMPLATE_BYTES ) );

   CComPtr<ID3DXFileEnumObject> pEnumObj;
   if(zip)
   {
      // Find the path to the file, and convert it to ANSI (for the D3DXOF API)
      if(zip->OpenFile(filename))
      {

         //if(!zip->ExtractFileTo(filename, "temp.x"))
         //{
         //   return E_FAIL;
         //}
         //sys_verify_hr_return_hr(pDXFile->CreateEnumObject( (VOID*)"temp.x", DXFILELOAD_FROMFILE , 
         //   &pEnumObj ));

         CZipFileHeader fh;
         zip->GetFileInfo(fh, zip->FindFile(filename));
         int filesize = fh.m_uUncomprSize;

         BYTE* pBuffer = new BYTE[filesize];
         memset(pBuffer,0,filesize);
         int retval = zip->ReadFile(pBuffer,filesize);
         if( retval > 0)
         {
            D3DXF_FILELOADMEMORY mem;
            mem.lpMemory = pBuffer;
            mem.dSize = filesize;

            sys_verify_hr(pDXFile->CreateEnumObject( (void*)&mem, D3DXF_FILELOAD_FROMMEMORY, &pEnumObj ));
         }
         delete [] pBuffer;
         zip->CloseFile();
      }
   }
   else
   {
      // Load the mesh
      sys_verify_hr_return_hr(pDXFile->CreateEnumObject( (void*)filename, D3DXF_FILELOAD_FROMFILE , 
         &pEnumObj ));
   }



   // Create enum object
   try
   {
      // Enumerate top level objects (which are always frames)
      SIZE_T numb_children;
      pEnumObj->GetChildren(&numb_children);
      for (SIZE_T i = 0; i<numb_children; i++)
      {
         CComPtr<ID3DXFileData> child;
         sys_verify_hr(pEnumObj->GetChild(i, &child));

         sys_verify_hr_return_hr(loadFrame( device, child, this, zip ));
      }
   }
   catch(...)
   {
      sys_msg("Execption");
      return E_FAIL;
   }

   //if(zip)
   //   DeleteFile("temp.x");

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Xfile::File::render( IDirect3DDevice9* device,const D3DXMATRIX* pmatWorldMatrix, bool bUseTexture )
{

   // For pure devices, specify the world transform. If the world transform is not
   // specified on pure devices, this function will fail.

   // Set up the world transformation
   D3DXMATRIX matWorld;
   D3DXMatrixMultiply( &matWorld, pmatWorldMatrix, &m_mat );
   device->SetTransform( D3DTS_WORLD, &matWorld );

   // Render opaque subsets in the meshes
   if( m_child )
      m_child->render( device, true, false, &matWorld,	bUseTexture );

   // Enable alpha blending
   /*device->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
   device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
   device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );*/

   // Render alpha subsets in the meshes
   if( m_child )
      m_child->render( device, false, true, &matWorld,	bUseTexture );

   // Restore state
   //device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
   //device->SetTransform( D3DTS_WORLD, &matSavedWorld );

   if( m_mesh )
      m_mesh->render( device, false, true,	bUseTexture );

   return S_OK;
}

void Xfile::Mesh::computeBoundingSphere(D3DXVECTOR3* center,FLOAT* radius)
{
   D3DXVECTOR3* vertexs = NULL;
   m_sysMemMesh->LockVertexBuffer(D3DLOCK_READONLY,(void**)&vertexs);
   DWORD numb_vertex = m_sysMemMesh->GetNumVertices();
   sys_verify(m_sysMemMesh->GetFVF() == D3DFVF_D3DVERTEX)

   D3DXComputeBoundingSphere(vertexs, numb_vertex, D3DXGetFVFVertexSize(D3DFVF_D3DVERTEX), center, radius);
   m_sysMemMesh->UnlockVertexBuffer();
}

void Xfile::Frame::computeBoundingSphere(std::vector<SCenterPoints>& points,const D3DXMATRIX& worldMatrix)
{
   D3DXMATRIX world;
   D3DXMatrixMultiply( &world, &worldMatrix, &m_mat );

   if( m_mesh )
   {
      SCenterPoints point;
      m_mesh->computeBoundingSphere(&point.center,&point.radius);
      D3DXVec3TransformCoord(&point.center,&point.center,&world);
      points.push_back(point);
   }
   if( m_child ) m_child->computeBoundingSphere(points,world);
   if( m_next )  m_next->computeBoundingSphere(points,worldMatrix);
}

void Xfile::File::computeBoundingSphere(D3DXVECTOR3* center,FLOAT* radius)
{
   *radius = 0.0f;
   *center = D3DXVECTOR3(0,0,0);
   D3DXMATRIX world;
   D3DXMatrixIdentity(&world);
   std::vector<SCenterPoints> points;
   Xfile::Frame::computeBoundingSphere(points,world);


   D3DXVECTOR3* temp_points = new D3DXVECTOR3[points.size()];

   for(UINT i=0;i<points.size();i++)
   {
      temp_points[i] = points[i].center;
   }

   FLOAT temp_radius;
   // calulates center point
   D3DXComputeBoundingSphere(temp_points, points.size(), D3DXGetFVFVertexSize(D3DFVF_XYZ), center, &temp_radius);
   delete [] temp_points;

   *radius = temp_radius;
   for(i=0;i<points.size();i++)
   {
      D3DXVECTOR3 v = points[i].center - *center;
      FLOAT l = D3DXVec3Length(&v);
      FLOAT r = points[i].radius + l;
      *radius = max(r,*radius);
   }
}





void Xfile::Frame::addFrame(Xfile::Frame* frame)
{
   frame->m_next = m_child;
   m_child = frame;
}

void Xfile::Frame::resetMatrix()
{
   m_mat = m_matOrig;

   if( m_child )
      m_child->resetMatrix();

   if( m_next )
      m_next->resetMatrix();

}

void Xfile::Frame::setTime(int MinKey,int MaxKey,float fGlobalTime)
{


   /*UINT iKey;
   UINT dwp2;
   UINT dwp3;

   D3DXMATRIXA16 matTemp;
   float fTime1;
   float fTime2;
   float fLerpValue;
   D3DXVECTOR3 scale;
   D3DXVECTOR3 pos;


   float fTime;*/

   if (m_matrixKeys )
   {
      float fTime;
      int TotalKeys = m_matrixKeys_count;
      if(MaxKey > TotalKeys)
      {
         MaxKey=TotalKeys;
      }

      if(MinKey == MaxKey && MaxKey == -1)
      {
         fTime = (float)fmod(fGlobalTime, m_matrixKeys[TotalKeys-1].time);
         MaxKey = TotalKeys-1;
         MinKey = 0;
      }
      else
      {
         fTime = (float)fmod(fGlobalTime, m_matrixKeys[MaxKey-1].time-m_matrixKeys[MinKey].time);
         fTime += m_matrixKeys[MinKey].time;
      }



      UINT dwp2 = MinKey;
      UINT dwp3 = MinKey;

      for (UINT iKey = MinKey ;iKey < MaxKey ; iKey++)
      {
         if ((float)m_matrixKeys[iKey].time > fTime)
         {
            dwp3 = iKey;

            if (iKey > 0)
            {
               dwp2= iKey - 1;
            }
            else  // when iKey == 0, then dwp2 == 0
            {
               dwp2 = iKey;
            }

            break;
         }
      }
      float fTime1 = (float)m_matrixKeys[dwp2].time;
      float fTime2 = (float)m_matrixKeys[dwp3].time;

      float fLerpValue = 0;
      if ((fTime2 - fTime1) != 0)
         fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

      if (fLerpValue > 0.5)
      {
         iKey = dwp3;
      }
      else
      {
         iKey = dwp2;
      }

      m_frameToAnimate->m_mat = m_matrixKeys[iKey].mat;
   }
   else
   {
      D3DXMATRIX matResult;
      bool bAnimate = false;

      D3DXMatrixIdentity(&matResult);

      if (m_scaleKeys)
      {
         UINT dwp3;
         UINT dwp2 = dwp3 = 0;

         FLOAT fTime = (float)fmod(fGlobalTime, m_scaleKeys[m_scaleKeys_count-1].time);

         for (UINT iKey = 0 ;iKey < m_scaleKeys_count ; iKey++)
         {
            if ((float)m_scaleKeys[iKey].time > fTime)
            {
               dwp3 = iKey;

               if (iKey > 0)
               {
                  dwp2= iKey - 1;
               }
               else  // when iKey == 0, then dwp2 == 0
               {
                  dwp2 = iKey;
               }

               break;
            }
         }
         FLOAT fTime1 = (float)m_scaleKeys[dwp2].time;
         FLOAT fTime2 = (float)m_scaleKeys[dwp3].time;

         FLOAT fLerpValue = 0;
         if ((fTime2 - fTime1) !=0)
            fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);


         D3DXVECTOR3 scale;
         D3DXMATRIX matTemp;

         D3DXVec3Lerp(&scale,
            &m_scaleKeys[dwp2].scale,
            &m_scaleKeys[dwp3].scale,
            fLerpValue);


         D3DXMatrixScaling(&matTemp, scale.x, scale.y, scale.z);

         D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

         bAnimate = true;
      }

      //check rot keys
      if (m_rotateKeys )
      {
         int i1 = 0;
         int i2 = 0;

         FLOAT fTime = (float)fmod(fGlobalTime, m_rotateKeys[m_rotateKeys_count-1].time);

         for (UINT iKey = 0 ;iKey < m_rotateKeys_count ; iKey++)
         {
            if ((float)m_rotateKeys[iKey].time > fTime)
            {
               i1 = (iKey > 0) ? iKey - 1 : 0;
               i2 = iKey;
               break;
            }
         }

         FLOAT fTime1 = (float)m_rotateKeys[i1].time;
         FLOAT fTime2 = (float)m_rotateKeys[i2].time;

         FLOAT fLerpValue = 0;
         if ((fTime2 - fTime1) !=0)
            fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

         D3DXQUATERNION quat;
#define USE_SQUAD
#ifdef USE_SQUAD
         int i0 = i1 - 1;
         int i3 = i2 + 1;

         if(i0 < 0)
            i0 += m_rotateKeys_count;

         if(i3 >= (INT) m_rotateKeys_count)
            i3 -= m_rotateKeys_count;

         D3DXQUATERNION qA, qB, qC;
         D3DXQuaternionSquadSetup(&qA, &qB, &qC, 
            &m_rotateKeys[i0].quatRotate, &m_rotateKeys[i1].quatRotate, 
            &m_rotateKeys[i2].quatRotate, &m_rotateKeys[i3].quatRotate);

         D3DXQuaternionSquad(&quat, &m_rotateKeys[i1].quatRotate, &qA, &qB, &qC, fLerpValue);
#else
         D3DXQuaternionSlerp(&quat, &m_rotateKeys[i1].quatRotate, &m_rotateKeys[i2].quatRotate, fLerpValue);
#endif

         quat.w = -quat.w;
         D3DXMATRIX matTemp;
         D3DXMatrixRotationQuaternion(&matTemp, &quat);
         D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

         bAnimate = true;
      }

      if (m_positionKeys)
      {
         UINT dwp2 = 0;
         UINT dwp3 = 0;

         FLOAT fTime = (float)fmod(fGlobalTime, m_positionKeys[m_positionKeys_count-1].time);

         for (UINT iKey = 0 ;iKey < m_positionKeys_count ; iKey++)
         {
            if ((float)m_positionKeys[iKey].time > fTime)
            {
               dwp3 = iKey;

               if (iKey > 0)
               {
                  dwp2= iKey - 1;
               }
               else  // when iKey == 0, then dwp2 == 0
               {
                  dwp2 = iKey;
               }

               break;
            }
         }
         FLOAT fTime1 = (float)m_positionKeys[dwp2].time;
         FLOAT fTime2 = (float)m_positionKeys[dwp3].time;

         FLOAT fLerpValue = 0;
         if ((fTime2 - fTime1) !=0)
            fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

         D3DXVECTOR3 pos;

         D3DXVec3Lerp(&pos,
            &m_positionKeys[dwp2].pos,
            &m_positionKeys[dwp3].pos,
            fLerpValue);

         D3DXMATRIX matTemp;

         D3DXMatrixTranslation(&matTemp, pos.x, pos.y, pos.z);

         D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
         bAnimate = true;
      }
      else
      {
         D3DXMATRIX matTemp;
         D3DXMatrixTranslation(&matTemp, m_frameToAnimate->m_matOrig._41, m_frameToAnimate->m_matOrig._42, m_frameToAnimate->m_matOrig._43);

         D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
      }

      if (bAnimate)
      {
         m_frameToAnimate->m_mat = matResult;
      }
   }
}

void Xfile::Frame::setKey(int Key)
{
   if (m_matrixKeys )
   {
      if(Key < 0 || Key >=  m_matrixKeys_count)
         Key = 0;
      m_frameToAnimate->m_mat = m_matrixKeys[Key].mat;
   }
   else
   {
      D3DXMATRIX matResult;
      bool bAnimate = false;

      D3DXMatrixIdentity(&matResult);

      if (m_scaleKeys)
      {
         if(Key < 0 || Key >=  m_scaleKeys_count)
            Key = 0;
         D3DXVECTOR3 scale = m_scaleKeys[Key].scale;
         D3DXMATRIX matTemp;

         D3DXMatrixScaling(&matTemp, scale.x, scale.y, scale.z);

         D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

         bAnimate = true;
      }

      //check rot keys
      if (m_rotateKeys )
      {
         if(Key < 0 || Key >=  m_rotateKeys_count)
            Key = 0;
         D3DXQUATERNION quat = m_rotateKeys[Key].quatRotate;

         quat.w = -quat.w;
         D3DXMATRIX matTemp;
         D3DXMatrixRotationQuaternion(&matTemp, &quat);
         D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

         bAnimate = true;
      }

      if (m_positionKeys)
      {
         if(Key < 0 || Key >=  m_positionKeys_count)
            Key = 0;
         D3DXVECTOR3 pos = m_positionKeys[Key].pos;
         D3DXMATRIX matTemp;
         D3DXMatrixTranslation(&matTemp, pos.x, pos.y, pos.z);
         D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
         bAnimate = true;
      }
      else
      {
         D3DXMATRIX matTemp;
         D3DXMatrixTranslation(&matTemp, m_frameToAnimate->m_matOrig._41, m_frameToAnimate->m_matOrig._42, m_frameToAnimate->m_matOrig._43);
         D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
      }

      if (bAnimate)
      {
         m_frameToAnimate->m_mat = matResult;
      }
   }
}

void Xfile::File::setKey(int Key)
{
   Xfile::Frame* pframeCur = m_frameAnimNext;
   while (pframeCur != NULL)
   {
      pframeCur->setKey(Key);
      pframeCur = pframeCur->m_frameAnimNext;
   }
}

void Xfile::File::setTime(int MinKey,int MaxKey,float fGlobalTime)
{
   Xfile::Frame* pframeCur = m_frameAnimNext;
   while (pframeCur != NULL)
   {
      pframeCur->setTime(MinKey,MaxKey,fGlobalTime);
      pframeCur = pframeCur->m_frameAnimNext;
   }
}

bool Xfile::Frame::findFramePos(const TCHAR* Name, D3DXVECTOR3* pPos, const D3DXMATRIX* pmatWorld)
{
   D3DXMATRIX Current;
   if(pmatWorld != NULL)
      Current = *pmatWorld;
   else
      D3DXMatrixIdentity(&Current);

   D3DXMATRIX matWorld;
   D3DXMatrixMultiply( &matWorld, &m_mat, &Current);

   if( m_name == Name )
   {
      D3DXVec3TransformCoord(pPos,pPos,&matWorld);
      return true;
   }

   if( m_child )
      if( m_child->findFramePos(Name,pPos,&matWorld))
         return true;

   if( m_next )
      if( m_next->findFramePos(Name,pPos,&Current) )
         return true;

   return false;
}

