//-----------------------------------------------------------------------------
// File: DXUtil.h
//
// Desc: Helper functions and typing shortcuts for DirectX programming.
//
// Copyright (c) 1997-2001 Microsoft Corporation. All rights reserved
//-----------------------------------------------------------------------------
#ifndef DXUTIL_H
#define DXUTIL_H

#include <afx.h>

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }



//-----------------------------------------------------------------------------
// Name: ArrayListType
// Desc: Indicates how data should be stored in a CArrayList
//-----------------------------------------------------------------------------
enum ArrayListType
{
   AL_VALUE,       // entry data is copied into the list
   AL_REFERENCE,   // entry pointers are copied into the list
};


//-----------------------------------------------------------------------------
// Name: CArrayList
// Desc: A growable array
//-----------------------------------------------------------------------------
class CArrayList
{
protected:
   ArrayListType m_ArrayListType;
   void* m_pData;
   UINT m_BytesPerEntry;
   UINT m_NumEntries;
   UINT m_NumEntriesAllocated;

public:
   CArrayList( ArrayListType Type, UINT BytesPerEntry = 0 );
   ~CArrayList( void );
   HRESULT Add( void* pEntry );
   void Remove( UINT Entry );
   void* GetPtr( UINT Entry );
   UINT Count( void ) { return m_NumEntries; }
   bool Contains( void* pEntryData );
   void Clear( void ) { m_NumEntries = 0; }
};

namespace DXUtil
{
   //-----------------------------------------------------------------------------
   // Name: DXUtil_Read*RegKey() and DXUtil_Write*RegKey()
   // Desc: Helper functions to read/write a string registry key 
   //-----------------------------------------------------------------------------
   HRESULT WriteStringRegKey(const TCHAR* strRegName, const TCHAR* strValue );
   HRESULT WriteIntRegKey(const TCHAR* strRegName, DWORD dwValue );
   HRESULT WriteGuidRegKey(const TCHAR* strRegName, GUID guidValue );
   HRESULT WriteBoolRegKey(const TCHAR* strRegName, BOOL bValue );

   HRESULT ReadStringRegKey(const TCHAR* strRegName, TCHAR* strValue, DWORD dwLength, const TCHAR* strDefault );
   HRESULT ReadIntRegKey(const TCHAR* strRegName, DWORD* pdwValue, DWORD dwDefault = 0);
   HRESULT ReadGuidRegKey(const TCHAR* strRegName, GUID* pGuidValue, GUID& guidDefault );
   HRESULT ReadBoolRegKey(const TCHAR* strRegName, BOOL* pbValue, BOOL bDefault  = FALSE);




   //-----------------------------------------------------------------------------
   // Name: Timer()
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
   enum TIMER_COMMAND 
   {  
      TIMER_RESET, 
      TIMER_START, 
      TIMER_STOP, 
      TIMER_ADVANCE,                 
      TIMER_GETABSOLUTETIME, 
      TIMER_GETAPPTIME, 
      TIMER_GETELAPSEDTIME 
   };

   FLOAT __stdcall Timer( TIMER_COMMAND command );




   //-----------------------------------------------------------------------------
   // UNICODE support for converting between CHAR, TCHAR, and WCHAR strings
   //-----------------------------------------------------------------------------
   VOID ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar = -1 );
   VOID ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar = -1 );
   VOID ConvertGenericStringToAnsi( CHAR* strDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
   VOID ConvertGenericStringToWide( WCHAR* wstrDestination, const TCHAR* tstrSource, int cchDestChar = -1 );
   VOID ConvertAnsiStringToGeneric( TCHAR* tstrDestination, const CHAR* strSource, int cchDestChar = -1 );
   VOID ConvertWideStringToGeneric( TCHAR* tstrDestination, const WCHAR* wstrSource, int cchDestChar = -1 );




   //-----------------------------------------------------------------------------
   // GUID to String converting 
   //-----------------------------------------------------------------------------
   VOID ConvertGUIDToString( const GUID* pGuidIn, TCHAR* strOut );
   BOOL ConvertStringToGUID( const TCHAR* strIn, GUID* pGuidOut );




   //-----------------------------------------------------------------------------
   // Debug printing support
   //-----------------------------------------------------------------------------
   HRESULT _DbgOut(const TCHAR*, DWORD, HRESULT,const TCHAR* , ...);


#define __LOCATION__ __FILE__,__LINE__

#define sys_printf DXUtil::_DbgOut
#define sys_msg_hr( str, hr ) sys_printf( __LOCATION__, hr, (str) )
#define sys_msg( str ) sys_printf( __LOCATION__, 0, (str) )
#define sys_msg1( str, val ) sys_printf( __LOCATION__, 0, (str), (val) )
#define sys_hr( hr ) sys_printf( __LOCATION__, hr, "ERROR" )

#define sys_verify( exp ) if(!(exp)){ sys_printf( __LOCATION__, 0, (#exp) );}
#define sys_verify_hr( exp ) {HRESULT __hr = (exp); if(FAILED(__hr)){ sys_printf( __LOCATION__, __hr, (#exp) );} }
#define sys_verify_return( exp , val) if(!(exp)){ sys_printf( __LOCATION__, 0, (#exp) ); return (val);}
#define sys_verify_void( exp ) if(!(exp)){ sys_printf( __LOCATION__, 0, (#exp) ); return;}
#define sys_verify_hr_return_hr( exp ) {HRESULT __hr = (exp); if(FAILED(__hr)){ sys_printf( __LOCATION__, __hr, (#exp) ); return __hr;} }
#define sys_verify_hr_return( exp, val ) {HRESULT __hr = (exp); if(FAILED(__hr)){ sys_printf( __LOCATION__, __hr, (#exp) ); return val;} }

}




#endif // DXUTIL_H
