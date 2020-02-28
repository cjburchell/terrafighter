//-----------------------------------------------------------------------------
// File: DXUtil.cpp
//
// Desc: Shortcut macros and functions for using DX objects
//
//
// Copyright (c) 1997-2001 Microsoft Corporation. All rights reserved
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "DXUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GAME_REG_KEY        TEXT("Software\\Redpoint\\Terrafighter")




//-----------------------------------------------------------------------------
// Name: DXUtil::ReadStringRegKey()
// Desc: Helper function to read a registry key string
//-----------------------------------------------------------------------------
HRESULT DXUtil::ReadStringRegKey(const TCHAR* strRegName, TCHAR* strValue, 
                                 DWORD dwLength, const TCHAR* strDefault )
{
   HKEY hKey;
   if(RegCreateKeyEx( HKEY_CURRENT_USER, GAME_REG_KEY, 0, NULL,
      REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, 
      &hKey, NULL ) != ERROR_SUCCESS) 
      return E_FAIL;

   DWORD dwType;

   if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
      (BYTE*)strValue, &dwLength ) )
   {
      _tcscpy( strValue, strDefault );
   }

   RegCloseKey( hKey );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DXUtil::WriteStringRegKey()
// Desc: Helper function to write a registry key string
//-----------------------------------------------------------------------------
HRESULT DXUtil::WriteStringRegKey(const TCHAR* strRegName,
                                  const TCHAR* strValue )
{
   HRESULT hr = S_OK;
   HKEY hKey;
   if(RegCreateKeyEx( HKEY_CURRENT_USER, GAME_REG_KEY, 0, NULL,
      REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, 
      &hKey, NULL ) != ERROR_SUCCESS) 
      return E_FAIL;

   if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_SZ, 
      (BYTE*)strValue, 
      (_tcslen(strValue)+1)*sizeof(TCHAR) ) )
      hr =  E_FAIL;

   RegCloseKey( hKey );
   return hr;
}



//-----------------------------------------------------------------------------
// Name: DXUtil::ReadIntRegKey()
// Desc: Helper function to read a registry key int
//-----------------------------------------------------------------------------
HRESULT DXUtil::ReadIntRegKey(const TCHAR* strRegName, DWORD* pdwValue, 
                              DWORD dwDefault )
{
   HKEY hKey;
   if(RegCreateKeyEx( HKEY_CURRENT_USER, GAME_REG_KEY, 0, NULL,
      REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, 
      &hKey, NULL ) != ERROR_SUCCESS) 
      return E_FAIL;


   DWORD dwType;
   DWORD dwLength = sizeof(DWORD);

   if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
      (BYTE*)pdwValue, &dwLength ) )
   {
      *pdwValue = dwDefault;
   }

   RegCloseKey( hKey );

   return S_OK;
}


//-----------------------------------------------------------------------------
// Name: DXUtil::WriteIntRegKey()
// Desc: Helper function to write a registry key int
//-----------------------------------------------------------------------------
HRESULT DXUtil::WriteIntRegKey(const TCHAR* strRegName, DWORD dwValue )
{
   HRESULT hr = S_OK;
   HKEY hKey;
   if(RegCreateKeyEx( HKEY_CURRENT_USER, GAME_REG_KEY, 0, NULL,
      REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, 
      &hKey, NULL ) != ERROR_SUCCESS) 
      return E_FAIL;

   if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
      (BYTE*)&dwValue, sizeof(DWORD) ) )
      hr =  E_FAIL;

   RegCloseKey( hKey );
   return hr;
}





//-----------------------------------------------------------------------------
// Name: DXUtil::ReadBoolRegKey()
// Desc: Helper function to read a registry key BOOL
//-----------------------------------------------------------------------------
HRESULT DXUtil::ReadBoolRegKey(const TCHAR* strRegName, BOOL* pbValue, 
                               BOOL bDefault )
{

   *pbValue = bDefault;

   HKEY hKey;
   if(RegCreateKeyEx( HKEY_CURRENT_USER, GAME_REG_KEY, 0, NULL,
      REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, 
      &hKey, NULL ) != ERROR_SUCCESS) 
      return E_FAIL;

   DWORD dwType;
   DWORD dwLength = sizeof(BOOL);

   if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
      (BYTE*)pbValue, &dwLength ) )
   {
      *pbValue = bDefault;
   }

   RegCloseKey( hKey );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DXUtil::WriteBoolRegKey()
// Desc: Helper function to write a registry key BOOL
//-----------------------------------------------------------------------------
HRESULT DXUtil::WriteBoolRegKey(const TCHAR* strRegName, BOOL bValue )
{
   HRESULT hr = S_OK;
   HKEY hKey;
   if(RegCreateKeyEx( HKEY_CURRENT_USER, GAME_REG_KEY, 0, NULL,
      REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, 
      &hKey, NULL ) != ERROR_SUCCESS) 
      return E_FAIL;

   if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
      (BYTE*)&bValue, sizeof(BOOL) ) )
      hr =  E_FAIL;

   RegCloseKey( hKey );
   return hr;
}




//-----------------------------------------------------------------------------
// Name: DXUtil::ReadGuidRegKey()
// Desc: Helper function to read a registry key guid
//-----------------------------------------------------------------------------
HRESULT DXUtil::ReadGuidRegKey(const TCHAR* strRegName, GUID* pGuidValue, 
                               GUID& guidDefault )
{

   HKEY hKey;
   if(RegCreateKeyEx( HKEY_CURRENT_USER, GAME_REG_KEY, 0, NULL,
      REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, 
      &hKey, NULL ) != ERROR_SUCCESS) 
      return E_FAIL;

   DWORD dwType;
   DWORD dwLength = sizeof(GUID);

   if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
      (LPBYTE) pGuidValue, &dwLength ) )
   {
      *pGuidValue = guidDefault;
   }

   RegCloseKey( hKey );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DXUtil::WriteGuidRegKey()
// Desc: Helper function to write a registry key guid
//-----------------------------------------------------------------------------
HRESULT DXUtil::WriteGuidRegKey(const TCHAR* strRegName, GUID guidValue )
{
   HRESULT hr = S_OK;
   HKEY hKey;
   if(RegCreateKeyEx( HKEY_CURRENT_USER, GAME_REG_KEY, 0, NULL,
      REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, 
      &hKey, NULL ) != ERROR_SUCCESS) 
      return E_FAIL;

   if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_BINARY, 
      (BYTE*)&guidValue, sizeof(GUID) ) )
      hr =  E_FAIL;

   RegCloseKey( hKey );
   return hr;
}




//-----------------------------------------------------------------------------
// Name: DXUtil::Timer()
// Desc: Performs timer opertations. Use the following commands:
//          TIMER_RESET           - to reset the timer
//          TIMER_START           - to start the timer
//          TIMER_STOP            - to stop (or pause) the timer
//          TIMER_ADVANCE         - to advance the timer by 0.1 seconds
//          TIMER_GETABSOLUTETIME - to get the absolute system time
//          TIMER_GETAPPTIME      - to get the current time
//          TIMER_GETELAPSEDTIME  - to get the time that elapsed between 
//                                  TIMER_GETELAPSEDTIME calls
//-----------------------------------------------------------------------------
FLOAT __stdcall DXUtil::Timer( TIMER_COMMAND command )
{
   static BOOL     m_bTimerInitialized = FALSE;
   static BOOL     m_bUsingQPF         = FALSE;
   static BOOL     m_bTimerStopped     = TRUE;
   static LONGLONG m_llQPFTicksPerSec  = 0;

   // Initialize the timer
   if( FALSE == m_bTimerInitialized )
   {
      m_bTimerInitialized = TRUE;

      // Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
      // not supported, we will timeGetTime() which returns milliseconds.
      LARGE_INTEGER qwTicksPerSec;
      m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
      if( m_bUsingQPF )
         m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
   }

   if( m_bUsingQPF )
   {
      static LONGLONG m_llStopTime        = 0;
      static LONGLONG m_llLastElapsedTime = 0;
      static LONGLONG m_llBaseTime        = 0;
      double fTime;
      double fElapsedTime;
      LARGE_INTEGER qwTime;

      // Get either the current time or the stop time, depending
      // on whether we're stopped and what command was sent
      if( m_llStopTime != 0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
         qwTime.QuadPart = m_llStopTime;
      else
         QueryPerformanceCounter( &qwTime );

      // Return the elapsed time
      if( command == TIMER_GETELAPSEDTIME )
      {
         fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
         m_llLastElapsedTime = qwTime.QuadPart;
         return (FLOAT) fElapsedTime;
      }

      // Return the current time
      if( command == TIMER_GETAPPTIME )
      {
         double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
         return (FLOAT) fAppTime;
      }

      // Reset the timer
      if( command == TIMER_RESET )
      {
         m_llBaseTime        = qwTime.QuadPart;
         m_llLastElapsedTime = qwTime.QuadPart;
         m_llStopTime        = 0;
         m_bTimerStopped     = FALSE;
         return 0.0f;
      }

      // Start the timer
      if( command == TIMER_START )
      {
         if( m_bTimerStopped )
            m_llBaseTime += qwTime.QuadPart - m_llStopTime;
         m_llStopTime = 0;
         m_llLastElapsedTime = qwTime.QuadPart;
         m_bTimerStopped = FALSE;
         return 0.0f;
      }

      // Stop the timer
      if( command == TIMER_STOP )
      {
         m_llStopTime = qwTime.QuadPart;
         m_llLastElapsedTime = qwTime.QuadPart;
         m_bTimerStopped = TRUE;
         return 0.0f;
      }

      // Advance the timer by 1/10th second
      if( command == TIMER_ADVANCE )
      {
         m_llStopTime += m_llQPFTicksPerSec/10;
         return 0.0f;
      }

      if( command == TIMER_GETABSOLUTETIME )
      {
         fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
         return (FLOAT) fTime;
      }

      return -1.0f; // Invalid command specified
   }
   else
   {
      // Get the time using timeGetTime()
      static double m_fLastElapsedTime  = 0.0;
      static double m_fBaseTime         = 0.0;
      static double m_fStopTime         = 0.0;
      double fTime;
      double fElapsedTime;

      // Get either the current time or the stop time, depending
      // on whether we're stopped and what command was sent
      if( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
         fTime = m_fStopTime;
      else
         fTime = timeGetTime() * 0.001;

      // Return the elapsed time
      if( command == TIMER_GETELAPSEDTIME )
      {   
         fElapsedTime = (double) (fTime - m_fLastElapsedTime);
         m_fLastElapsedTime = fTime;
         return (FLOAT) fElapsedTime;
      }

      // Return the current time
      if( command == TIMER_GETAPPTIME )
      {
         return (FLOAT) (fTime - m_fBaseTime);
      }

      // Reset the timer
      if( command == TIMER_RESET )
      {
         m_fBaseTime         = fTime;
         m_fLastElapsedTime  = fTime;
         m_fStopTime         = 0;
         m_bTimerStopped     = FALSE;
         return 0.0f;
      }

      // Start the timer
      if( command == TIMER_START )
      {
         if( m_bTimerStopped )
            m_fBaseTime += fTime - m_fStopTime;
         m_fStopTime = 0.0f;
         m_fLastElapsedTime  = fTime;
         m_bTimerStopped = FALSE;
         return 0.0f;
      }

      // Stop the timer
      if( command == TIMER_STOP )
      {
         m_fStopTime = fTime;
         m_fLastElapsedTime  = fTime;
         m_bTimerStopped = TRUE;
         return 0.0f;
      }

      // Advance the timer by 1/10th second
      if( command == TIMER_ADVANCE )
      {
         m_fStopTime += 0.1f;
         return 0.0f;
      }

      if( command == TIMER_GETABSOLUTETIME )
      {
         return (FLOAT) fTime;
      }

      return -1.0f; // Invalid command specified
   }
}




//-----------------------------------------------------------------------------
// Name: DXUtil::ConvertAnsiStringToWide()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       WCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID DXUtil::ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, 
                                     int cchDestChar )
{
   if( wstrDestination==NULL || strSource==NULL )
      return;

   if( cchDestChar == -1 )
      cchDestChar = strlen(strSource)+1;

   MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
      wstrDestination, cchDestChar-1 );

   wstrDestination[cchDestChar-1] = 0;
}




//-----------------------------------------------------------------------------
// Name: DXUtil::ConvertWideStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       CHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID DXUtil::ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, 
                                     int cchDestChar )
{
   if( strDestination==NULL || wstrSource==NULL )
      return;

   if( cchDestChar == -1 )
      cchDestChar = wcslen(wstrSource)+1;

   WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
      cchDestChar-1, NULL, NULL );

   strDestination[cchDestChar-1] = 0;
}




//-----------------------------------------------------------------------------
// Name: DXUtil::ConvertGenericStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       CHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID DXUtil::ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
   if( strDestination==NULL || tstrSource==NULL || cchDestChar == 0 )
      return;

#ifdef _UNICODE
   DXUtil::ConvertWideStringToAnsi( strDestination, tstrSource, cchDestChar );
#else
   if( cchDestChar == -1 )
   {
      strcpy( strDestination, tstrSource );
   }
   else
   {
      strncpy( strDestination, tstrSource, cchDestChar );
      strDestination[cchDestChar-1] = '\0';
   }
#endif
}




//-----------------------------------------------------------------------------
// Name: DXUtil::ConvertGenericStringToWide()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       WCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID DXUtil::ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, 
                                        int cchDestChar )
{
   if( wstrDestination==NULL || tstrSource==NULL || cchDestChar == 0 )
      return;

#ifdef _UNICODE
   if( cchDestChar == -1 )
   {
      wcscpy( wstrDestination, tstrSource );
   }
   else
   {
      wcsncpy( wstrDestination, tstrSource, cchDestChar );
      wstrDestination[cchDestChar-1] = L'\0';
   }
#else
   DXUtil::ConvertAnsiStringToWide( wstrDestination, tstrSource, cchDestChar );
#endif
}




//-----------------------------------------------------------------------------
// Name: DXUtil::ConvertAnsiStringToGeneric()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       TCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID DXUtil::ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, 
                                        int cchDestChar )
{
   if( tstrDestination==NULL || strSource==NULL || cchDestChar == 0 )
      return;

#ifdef _UNICODE
   DXUtil::ConvertAnsiStringToWide( tstrDestination, strSource, cchDestChar );
#else
   if( cchDestChar == -1 )
   {
      strcpy( tstrDestination, strSource );
   }
   else
   {
      strncpy( tstrDestination, strSource, cchDestChar );
      tstrDestination[cchDestChar-1] = '\0';
   }
#endif
}




//-----------------------------------------------------------------------------
// Name: DXUtil::ConvertAnsiStringToGeneric()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       TCHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
VOID DXUtil::ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, 
                                        int cchDestChar )
{
   if( tstrDestination==NULL || wstrSource==NULL || cchDestChar == 0 )
      return;

#ifdef _UNICODE
   if( cchDestChar == -1 )
   {
      wcscpy( tstrDestination, wstrSource );
   }
   else
   {
      wcsncpy( tstrDestination, wstrSource, cchDestChar );
      tstrDestination[cchDestChar-1] = L'\0';
   }
#else
   DXUtil::ConvertWideStringToAnsi( tstrDestination, wstrSource, cchDestChar );
#endif
}




#define DEBUG_FILE_NAME "debug.txt"
class CDebugFile
{
   FILE* m_hfile;
public:
   CDebugFile()
   {
      m_hfile = NULL;
      //m_hfile = fopen(DEBUG_FILE_NAME,"a");
      Print("App Start\n");
   }

   ~CDebugFile()
   {
      if(m_hfile)
         fclose(m_hfile);
   }

   void Flush()
   {
      if(m_hfile )
      {
         ::fflush(m_hfile);
      }
   }

   void Print(const TCHAR* buffer)
   {
      if(m_hfile )
      {
         fprintf(m_hfile,buffer);
      }

      OutputDebugString(buffer);
   }
};

#define DEBUG_BUFFER_SIZE 512
//-----------------------------------------------------------------------------
// Name: _DbgOut()
// Desc: Outputs a message to the debug stream
//-----------------------------------------------------------------------------
HRESULT DXUtil::_DbgOut(const TCHAR* strFile, DWORD dwLine, HRESULT hr,const TCHAR* strMsg ... )
{
   static CDebugFile DebugFile;

   TCHAR buffer[DEBUG_BUFFER_SIZE];

   wsprintf( buffer, _T("%s(%ld): "), strFile, dwLine );
   DebugFile.Print( buffer );


   va_list args;
   va_start(args, strMsg);
   _vsntprintf( buffer, DEBUG_BUFFER_SIZE, strMsg, args );
   va_end(args);

   DebugFile.Print( buffer );

   if( hr )
   {
      wsprintf( buffer, _T("(%s hr=%08lx)"),DXGetErrorString9(hr),hr );
      DebugFile.Print( buffer );
   }
   DebugFile.Print( "\n" );

   DebugFile.Flush();
   return hr;
}




//-----------------------------------------------------------------------------
// Name: DXUtil::ConvertStringToGUID()
// Desc: Converts a string to a GUID
//-----------------------------------------------------------------------------
BOOL DXUtil::ConvertStringToGUID( const TCHAR* strIn, GUID* pGuidOut )
{
   UINT aiTmp[10];

   if( _stscanf( strIn, TEXT("{%8X-%4X-%4X-%2X%2X-%2X%2X%2X%2X%2X%2X}"),
      &pGuidOut->Data1, 
      &aiTmp[0], &aiTmp[1], 
      &aiTmp[2], &aiTmp[3],
      &aiTmp[4], &aiTmp[5],
      &aiTmp[6], &aiTmp[7],
      &aiTmp[8], &aiTmp[9] ) != 11 )
   {
      ZeroMemory( pGuidOut, sizeof(GUID) );
      return FALSE;
   }
   else
   {
      pGuidOut->Data2       = (USHORT) aiTmp[0];
      pGuidOut->Data3       = (USHORT) aiTmp[1];
      pGuidOut->Data4[0]    = (BYTE) aiTmp[2];
      pGuidOut->Data4[1]    = (BYTE) aiTmp[3];
      pGuidOut->Data4[2]    = (BYTE) aiTmp[4];
      pGuidOut->Data4[3]    = (BYTE) aiTmp[5];
      pGuidOut->Data4[4]    = (BYTE) aiTmp[6];
      pGuidOut->Data4[5]    = (BYTE) aiTmp[7];
      pGuidOut->Data4[6]    = (BYTE) aiTmp[8];
      pGuidOut->Data4[7]    = (BYTE) aiTmp[9];
      return TRUE;
   }
}




//-----------------------------------------------------------------------------
// Name: DXUtil::ConvertGUIDToString()
// Desc: Converts a GUID to a string 
//-----------------------------------------------------------------------------
VOID DXUtil::ConvertGUIDToString( const GUID* pGuidIn, TCHAR* strOut )
{
   _stprintf( strOut, TEXT("{%0.8X-%0.4X-%0.4X-%0.2X%0.2X-%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X}"),
      pGuidIn->Data1, pGuidIn->Data2, pGuidIn->Data3,
      pGuidIn->Data4[0], pGuidIn->Data4[1],
      pGuidIn->Data4[2], pGuidIn->Data4[3],
      pGuidIn->Data4[4], pGuidIn->Data4[5],
      pGuidIn->Data4[6], pGuidIn->Data4[7] );
}

//-----------------------------------------------------------------------------
// Name: CArrayList constructor
// Desc: 
//-----------------------------------------------------------------------------
CArrayList::CArrayList( ArrayListType Type, UINT BytesPerEntry )
{
   if( Type == AL_REFERENCE )
      BytesPerEntry = sizeof(void*);
   m_ArrayListType = Type;
   m_pData = NULL;
   m_BytesPerEntry = BytesPerEntry;
   m_NumEntries = 0;
   m_NumEntriesAllocated = 0;
}



//-----------------------------------------------------------------------------
// Name: CArrayList destructor
// Desc: 
//-----------------------------------------------------------------------------
CArrayList::~CArrayList( void )
{
   if( m_pData != NULL )
      delete[] m_pData;
}




//-----------------------------------------------------------------------------
// Name: CArrayList::Add
// Desc: Adds pEntry to the list.
//-----------------------------------------------------------------------------
HRESULT CArrayList::Add( void* pEntry )
{
   if( m_BytesPerEntry == 0 )
      return E_FAIL;
   if( m_pData == NULL || m_NumEntries + 1 > m_NumEntriesAllocated )
   {
      void* pDataNew;
      UINT NumEntriesAllocatedNew;
      if( m_NumEntriesAllocated == 0 )
         NumEntriesAllocatedNew = 16;
      else
         NumEntriesAllocatedNew = m_NumEntriesAllocated * 2;
      pDataNew = new BYTE[NumEntriesAllocatedNew * m_BytesPerEntry];
      if( pDataNew == NULL )
         return E_OUTOFMEMORY;
      if( m_pData != NULL )
      {
         CopyMemory( pDataNew, m_pData, m_NumEntries * m_BytesPerEntry );
         delete[] m_pData;
      }
      m_pData = pDataNew;
      m_NumEntriesAllocated = NumEntriesAllocatedNew;
   }

   if( m_ArrayListType == AL_VALUE )
      CopyMemory( (BYTE*)m_pData + (m_NumEntries * m_BytesPerEntry), pEntry, m_BytesPerEntry );
   else
      *(((void**)m_pData) + m_NumEntries) = pEntry;
   m_NumEntries++;

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CArrayList::Remove
// Desc: Remove the item at Entry in the list, and collapse the array. 
//-----------------------------------------------------------------------------
void CArrayList::Remove( UINT Entry )
{
   // Decrement count
   m_NumEntries--;

   // Find the entry address
   BYTE* pData = (BYTE*)m_pData + (Entry * m_BytesPerEntry);

   // Collapse the array
   MoveMemory( pData, pData + m_BytesPerEntry, ( m_NumEntries - Entry ) * m_BytesPerEntry );
}




//-----------------------------------------------------------------------------
// Name: CArrayList::GetPtr
// Desc: Returns a pointer to the Entry'th entry in the list.
//-----------------------------------------------------------------------------
void* CArrayList::GetPtr( UINT Entry )
{
   if( m_ArrayListType == AL_VALUE )
      return (BYTE*)m_pData + (Entry * m_BytesPerEntry);
   else
      return *(((void**)m_pData) + Entry);
}




//-----------------------------------------------------------------------------
// Name: CArrayList::Contains
// Desc: Returns whether the list contains an entry identical to the 
//       specified entry data.
//-----------------------------------------------------------------------------
bool CArrayList::Contains( void* pEntryData )
{
   for( UINT iEntry = 0; iEntry < m_NumEntries; iEntry++ )
   {
      if( m_ArrayListType == AL_VALUE )
      {
         if( memcmp( GetPtr(iEntry), pEntryData, m_BytesPerEntry ) == 0 )
            return true;
      }
      else
      {
         if( GetPtr(iEntry) == pEntryData )
            return true;
      }
   }
   return false;
}

