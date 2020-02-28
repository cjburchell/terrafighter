//-----------------------------------------------------------------------------
// File: DSUtil.cpp
//
// Desc: DirectSound framework classes for reading and writing wav files and
//       playing them in DirectSound buffers. Feel free to use this class 
//       as a starting point for adding extra functionality.
//
// Copyright (c) 1999-2001 Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "DSUtil.h"
#include "DXUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




//-----------------------------------------------------------------------------
// Name: CSoundManager::CSoundManager()
// Desc: Constructs the class
//-----------------------------------------------------------------------------
CSoundManager::CSoundManager()
{
   m_pDS = NULL;
   m_NumbBuffers = 0;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::~CSoundManager()
// Desc: Destroys the class
//-----------------------------------------------------------------------------
CSoundManager::~CSoundManager()
{
   SAFE_RELEASE( m_pDS ); 
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::Initialize()
// Desc: Initializes the IDirectSound object and also sets the primary buffer
//       format.  This function must be called before any others.
//-----------------------------------------------------------------------------
HRESULT CSoundManager::Initialize( HWND  hWnd, 
                                  DWORD dwCoopLevel, 
                                  DWORD dwPrimaryChannels, 
                                  DWORD dwPrimaryFreq, 
                                  DWORD dwPrimaryBitRate )
{
   SAFE_RELEASE( m_pDS );

   // Create IDirectSound using the primary sound device
   sys_verify_hr_return_hr(DirectSoundCreate8( NULL, &m_pDS, NULL ) );

   // Set DirectSound coop level 
   sys_verify_hr_return_hr(m_pDS->SetCooperativeLevel( hWnd, dwCoopLevel ) );

   // Set primary buffer format
   sys_verify_hr_return_hr(SetPrimaryBufferFormat( dwPrimaryChannels, dwPrimaryFreq, dwPrimaryBitRate ) );


   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::SetPrimaryBufferFormat()
// Desc: Set primary buffer to a specified format 
//       For example, to set the primary buffer format to 22kHz stereo, 16-bit
//       then:   dwPrimaryChannels = 2
//               dwPrimaryFreq     = 22050, 
//               dwPrimaryBitRate  = 16
//-----------------------------------------------------------------------------
HRESULT CSoundManager::SetPrimaryBufferFormat( DWORD dwPrimaryChannels, 
                                              DWORD dwPrimaryFreq, 
                                              DWORD dwPrimaryBitRate )
{
   CComPtr<IDirectSoundBuffer> pDSBPrimary;

   if( m_pDS == NULL )
      return CO_E_NOTINITIALIZED;

   // Get the primary buffer 
   DSBUFFERDESC dsbd;
   ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
   dsbd.dwSize        = sizeof(DSBUFFERDESC);
   dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
   dsbd.dwBufferBytes = 0;
   dsbd.lpwfxFormat   = NULL;

   sys_verify_hr_return_hr(m_pDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) );

   WAVEFORMATEX wfx;
   ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
   wfx.wFormatTag      = WAVE_FORMAT_PCM; 
   wfx.nChannels       = (WORD) dwPrimaryChannels; 
   wfx.nSamplesPerSec  = dwPrimaryFreq; 
   wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate; 
   wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
   wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

   sys_verify_hr_return_hr(pDSBPrimary->SetFormat(&wfx) );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::Get3DListenerInterface()
// Desc: Returns the 3D listener interface associated with primary buffer.
//-----------------------------------------------------------------------------
HRESULT CSoundManager::Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener )
{
   DSBUFFERDESC        dsbdesc;
   CComPtr<IDirectSoundBuffer> pDSBPrimary;

   if( ppDSListener == NULL )
      return E_INVALIDARG;
   if( m_pDS == NULL )
      return CO_E_NOTINITIALIZED;

   *ppDSListener = NULL;

   // Obtain primary buffer, asking it for 3D control
   ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
   dsbdesc.dwSize = sizeof(DSBUFFERDESC);
   dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
   sys_verify_hr_return_hr(m_pDS->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL ) );

   sys_verify_hr_return_hr( pDSBPrimary->QueryInterface( IID_IDirectSound3DListener, 
      (VOID**)ppDSListener ) );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::Create()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CSoundManager::Create( CSound** ppSound, 
                              LPCTSTR strWaveFileName,
                              CZipArchive* pZip, 
                              DWORD dwCreationFlags, 
                              GUID guid3DAlgorithm,
                              DWORD dwNumBuffers)
{
   //if(dwNumBuffers > 5)
   //	dwNumBuffers = 3;
   HRESULT hr;
   HRESULT hrRet = S_OK;
   DWORD   i;
   LPDIRECTSOUNDBUFFER* apDSBuffer     = NULL;
   DWORD                dwDSBufferSize = NULL;
   CWaveFile*           pWaveFile      = NULL;

   if( m_pDS == NULL )
      return CO_E_NOTINITIALIZED;
   if( strWaveFileName == NULL || ppSound == NULL || dwNumBuffers < 1 )
      return E_INVALIDARG;

   apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
   if( apDSBuffer == NULL )
   {
      hr = E_OUTOFMEMORY;
      goto LFail;
   }

   pWaveFile = new CWaveFile();
   if( pWaveFile == NULL )
   {
      hr = E_OUTOFMEMORY;
      goto LFail;
   }

   pWaveFile->Open( strWaveFileName, NULL, WAVEFILE_READ,pZip);

   if( pWaveFile->GetSize() == 0 )
   {
      // Wave is blank, so don't create it.
      hr = E_FAIL;
      goto LFail;
   }

   // Make the DirectSound buffer the same size as the wav file
   dwDSBufferSize = pWaveFile->GetSize();

   // Create the direct sound buffer, and only request the flags needed
   // since each requires some overhead and limits if the buffer can 
   // be hardware accelerated
   DSBUFFERDESC dsbd;
   ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
   dsbd.dwSize          = sizeof(DSBUFFERDESC);
   dsbd.dwFlags         = dwCreationFlags|DSBCAPS_STATIC;
   dsbd.dwBufferBytes   = dwDSBufferSize;
   dsbd.guid3DAlgorithm = guid3DAlgorithm;
   dsbd.lpwfxFormat     = pWaveFile->m_pwfx;

   // DirectSound is only guarenteed to play PCM data.  Other
   // formats may or may not work depending the sound card driver.
   hr = m_pDS->CreateSoundBuffer( &dsbd, &apDSBuffer[0], NULL );
   m_NumbBuffers++;

   // Be sure to return this error code if it occurs so the
   // callers knows this happened.
   if( hr == DS_NO_VIRTUALIZATION )
      hrRet = DS_NO_VIRTUALIZATION;

   if( FAILED(hr) )
   {
      // DSERR_BUFFERTOOSMALL will be returned if the buffer is
      // less than DSBSIZE_FX_MIN (100ms) and the buffer is created
      // with DSBCAPS_CTRLFX.
      if( hr != DSERR_BUFFERTOOSMALL )
         sys_msg_hr( TEXT("CreateSoundBuffer"), hr );

      goto LFail;
   }

   LPDIRECTSOUNDBUFFER pBuffer;
   pBuffer = apDSBuffer[0];

   for( i=1; i<dwNumBuffers; i++ )
   {
      LPDIRECTSOUNDBUFFER pNewBuffer = NULL;
      hr = m_pDS->DuplicateSoundBuffer( pBuffer, &pNewBuffer );
      m_NumbBuffers++;
      //hr=m_pDS->CreateSoundBuffer( &dsbd, &pNewBuffer, NULL );
      if(FAILED(hr))
      {
         sys_msg_hr( TEXT("DuplicateSoundBuffer"), hr );
         dwNumBuffers = i;
         hr = S_OK;
         break;
      }
      else
      {
         apDSBuffer[i] = pNewBuffer;
      }
   }

   // Create the sound
   *ppSound = new CSound( apDSBuffer, dwDSBufferSize, dwNumBuffers, pWaveFile );

   SAFE_DELETE( apDSBuffer );
   return hrRet;

LFail:
   // Cleanup
   SAFE_DELETE( pWaveFile );
   SAFE_DELETE( apDSBuffer );
   return hr;
}









//-----------------------------------------------------------------------------
// Name: CSoundManager::CreateFromMemory()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CSoundManager::CreateFromMemory( CSound** ppSound, 
                                        BYTE* pbData,
                                        ULONG  ulDataSize,
                                        LPWAVEFORMATEX pwfx,
                                        DWORD dwCreationFlags, 
                                        GUID guid3DAlgorithm,
                                        DWORD dwNumBuffers )
{
   HRESULT hr;
   DWORD   i;
   LPDIRECTSOUNDBUFFER* apDSBuffer     = NULL;
   DWORD                dwDSBufferSize = NULL;
   CWaveFile*           pWaveFile      = NULL;

   if( m_pDS == NULL )
      return CO_E_NOTINITIALIZED;
   if( pbData == NULL || ppSound == NULL || dwNumBuffers < 1 )
      return E_INVALIDARG;

   apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
   if( apDSBuffer == NULL )
   {
      hr = E_OUTOFMEMORY;
      goto LFail;
   }

   pWaveFile = new CWaveFile();
   if( pWaveFile == NULL )
   {
      hr = E_OUTOFMEMORY;
      goto LFail;
   }

   pWaveFile->OpenFromMemory( pbData,ulDataSize, pwfx, WAVEFILE_READ );


   // Make the DirectSound buffer the same size as the wav file
   dwDSBufferSize = ulDataSize;

   // Create the direct sound buffer, and only request the flags needed
   // since each requires some overhead and limits if the buffer can 
   // be hardware accelerated
   DSBUFFERDESC dsbd;
   ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
   dsbd.dwSize          = sizeof(DSBUFFERDESC);
   dsbd.dwFlags         = dwCreationFlags;
   dsbd.dwBufferBytes   = dwDSBufferSize;
   dsbd.guid3DAlgorithm = guid3DAlgorithm;
   dsbd.lpwfxFormat     = pwfx;

   if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbd, &apDSBuffer[0], NULL ) ) )
   {
      sys_msg_hr( TEXT("CreateSoundBuffer"), hr );
      goto LFail;
   }

   for( i=1; i<dwNumBuffers; i++ )
   {
      if( FAILED( hr = m_pDS->DuplicateSoundBuffer( apDSBuffer[0], &apDSBuffer[i] ) ) )
      {
         sys_msg_hr( TEXT("DuplicateSoundBuffer"), hr );
         goto LFail;
      }
   }

   // Create the sound
   *ppSound = new CSound( apDSBuffer, dwDSBufferSize, dwNumBuffers, pWaveFile );

   SAFE_DELETE( apDSBuffer );
   return S_OK;

LFail:
   // Cleanup

   SAFE_DELETE( apDSBuffer );
   return hr;
}





//-----------------------------------------------------------------------------
// Name: CSoundManager::CreateStreaming()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CSoundManager::CreateStreaming( CStreamingSound** ppStreamingSound, 
                                       LPTSTR strWaveFileName,
                                       CZipArchive* pZip,
                                       DWORD dwCreationFlags, 
                                       GUID guid3DAlgorithm,
                                       DWORD dwNotifyCount, 
                                       DWORD dwNotifySize, 
                                       HANDLE hNotifyEvent )
{
   HRESULT hr;

   if( m_pDS == NULL )
      return CO_E_NOTINITIALIZED;
   if( strWaveFileName == NULL || ppStreamingSound == NULL || hNotifyEvent == NULL )
      return E_INVALIDARG;

   LPDIRECTSOUNDBUFFER pDSBuffer      = NULL;
   DWORD               dwDSBufferSize = NULL;
   CWaveFile*          pWaveFile      = NULL;
   DSBPOSITIONNOTIFY*  aPosNotify     = NULL; 
   LPDIRECTSOUNDNOTIFY pDSNotify      = NULL;

   pWaveFile = new CWaveFile();
   pWaveFile->Open( strWaveFileName, NULL, WAVEFILE_READ,pZip );

   // Figure out how big the DSound buffer should be 
   dwDSBufferSize = dwNotifySize * dwNotifyCount;

   // Set up the direct sound buffer.  Request the NOTIFY flag, so
   // that we are notified as the sound buffer plays.  Note, that using this flag
   // may limit the amount of hardware acceleration that can occur. 
   DSBUFFERDESC dsbd;
   ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
   dsbd.dwSize          = sizeof(DSBUFFERDESC);
   dsbd.dwFlags         = dwCreationFlags | 
      DSBCAPS_CTRLPOSITIONNOTIFY | 
      DSBCAPS_GETCURRENTPOSITION2;
   dsbd.dwBufferBytes   = dwDSBufferSize;
   dsbd.guid3DAlgorithm = guid3DAlgorithm;
   dsbd.lpwfxFormat     = pWaveFile->m_pwfx;

   if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbd, &pDSBuffer, NULL ) ) )
   {
      // If wave format isn't then it will return 
      // either DSERR_BADFORMAT or E_INVALIDARG
      if( hr == DSERR_BADFORMAT || hr == E_INVALIDARG )
         return sys_msg_hr( TEXT("CreateSoundBuffer"), hr );

      return sys_msg_hr( TEXT("CreateSoundBuffer"), hr );
   }

   // Create the notification events, so that we know when to fill
   // the buffer as the sound plays. 
   if( FAILED( hr = pDSBuffer->QueryInterface( IID_IDirectSoundNotify, 
      (VOID**)&pDSNotify ) ) )
   {
      SAFE_DELETE( aPosNotify );
      return sys_msg_hr( TEXT("QueryInterface"), hr );
   }

   aPosNotify = new DSBPOSITIONNOTIFY[ dwNotifyCount ];
   if( aPosNotify == NULL )
      return E_OUTOFMEMORY;

   for( DWORD i = 0; i < dwNotifyCount; i++ )
   {
      aPosNotify[i].dwOffset     = (dwNotifySize * i) + dwNotifySize - 1;
      aPosNotify[i].hEventNotify = hNotifyEvent;             
   }

   // Tell DirectSound when to notify us. The notification will come in the from 
   // of signaled events that are handled in WinMain()
   if( FAILED( hr = pDSNotify->SetNotificationPositions( dwNotifyCount, 
      aPosNotify ) ) )
   {
      SAFE_RELEASE( pDSNotify );
      SAFE_DELETE( aPosNotify );
      return sys_msg_hr( TEXT("SetNotificationPositions"), hr );
   }

   SAFE_RELEASE( pDSNotify );
   SAFE_DELETE( aPosNotify );

   // Create the sound
   *ppStreamingSound = new CStreamingSound( pDSBuffer, dwDSBufferSize, pWaveFile, dwNotifySize );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSound::CSound()
// Desc: Constructs the class
//-----------------------------------------------------------------------------
CSound::CSound( LPDIRECTSOUNDBUFFER* apDSBuffer, DWORD dwDSBufferSize, 
               DWORD dwNumBuffers, CWaveFile* pWaveFile )
{
   m_CurrentBuffer = 0;
   DWORD i;


   //sys_printf(__LOCATION__, 0, "Create Sound %s\n",pWaveFile->GetFileName());
   m_apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
   for( i=0; i<dwNumBuffers; i++ )
      m_apDSBuffer[i] = apDSBuffer[i];

   m_dwDSBufferSize = dwDSBufferSize;
   m_dwNumBuffers   = dwNumBuffers;
   m_pWaveFile      = pWaveFile;

   FillBufferWithSound( m_apDSBuffer[0], FALSE );

   // Make DirectSound do pre-processing on sound effects
   for( i=0; i<dwNumBuffers; i++ )
      m_apDSBuffer[i]->SetCurrentPosition(0);
}




//-----------------------------------------------------------------------------
// Name: CSound::~CSound()
// Desc: Destroys the class
//-----------------------------------------------------------------------------
CSound::~CSound()
{
   for( DWORD i=0; i<m_dwNumBuffers; i++ )
   {
      SAFE_RELEASE( m_apDSBuffer[i] ); 
   }

   SAFE_DELETE_ARRAY( m_apDSBuffer ); 
   SAFE_DELETE( m_pWaveFile );
}




//-----------------------------------------------------------------------------
// Name: CSound::FillBufferWithSound()
// Desc: Fills a DirectSound buffer with a sound file 
//-----------------------------------------------------------------------------
HRESULT CSound::FillBufferWithSound( LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger )
{
   HRESULT hr; 
   VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
   DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
   DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

   if( pDSB == NULL )
      return CO_E_NOTINITIALIZED;

   // Make sure we have focus, and we didn't just switch in from
   // an app which had a DirectSound device
   if( FAILED( hr = RestoreBuffer( pDSB, NULL ) ) ) 
      return sys_msg_hr( TEXT("RestoreBuffer"), hr );

   // Lock the buffer down
   if( FAILED( hr = pDSB->Lock( 0, m_dwDSBufferSize, 
      &pDSLockedBuffer, &dwDSLockedBufferSize, 
      NULL, NULL, 0L ) ) )
      return sys_msg_hr( TEXT("Lock"), hr );

   // Reset the wave file to the beginning 
   m_pWaveFile->ResetFile();

   if( FAILED( hr = m_pWaveFile->Read( (BYTE*) pDSLockedBuffer,
      dwDSLockedBufferSize, 
      &dwWavDataRead ) ) )           
      return sys_msg_hr( TEXT("Read"), hr );

   if( dwWavDataRead == 0 )
   {
      // Wav is blank, so just fill with silence
      FillMemory( (BYTE*) pDSLockedBuffer, 
         dwDSLockedBufferSize, 
         (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
   }
   else if( dwWavDataRead < dwDSLockedBufferSize )
   {
      // If the wav file was smaller than the DirectSound buffer, 
      // we need to fill the remainder of the buffer with data 
      if( bRepeatWavIfBufferLarger )
      {       
         // Reset the file and fill the buffer with wav data
         DWORD dwReadSoFar = dwWavDataRead;    // From previous call above.
         while( dwReadSoFar < dwDSLockedBufferSize )
         {  
            // This will keep reading in until the buffer is full 
            // for very short files
            if( FAILED( hr = m_pWaveFile->ResetFile() ) )
               return sys_msg_hr( TEXT("ResetFile"), hr );

            hr = m_pWaveFile->Read( (BYTE*)pDSLockedBuffer + dwReadSoFar,
               dwDSLockedBufferSize - dwReadSoFar,
               &dwWavDataRead );
            if( FAILED(hr) )
               return sys_msg_hr( TEXT("Read"), hr );

            dwReadSoFar += dwWavDataRead;
         } 
      }
      else
      {
         // Don't repeat the wav file, just fill in silence 
         FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead, 
            dwDSLockedBufferSize - dwWavDataRead, 
            (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
      }
   }

   // Unlock the buffer, we don't need it anymore.
   pDSB->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSound::RestoreBuffer()
// Desc: Restores the lost buffer. *pbWasRestored returns TRUE if the buffer was 
//       restored.  It can also be NULL if the information is not needed.
//-----------------------------------------------------------------------------
HRESULT CSound::RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored )
{
   HRESULT hr;

   if( pDSB == NULL )
      return CO_E_NOTINITIALIZED;
   if( pbWasRestored )
      *pbWasRestored = FALSE;

   DWORD dwStatus;
   if( FAILED( hr = pDSB->GetStatus( &dwStatus ) ) )
      return sys_msg_hr( TEXT("GetStatus"), hr );

   if( dwStatus & DSBSTATUS_BUFFERLOST )
   {
      // Since the app could have just been activated, then DirectSound 
      // may not be giving us control yet, so restoring the buffer may fail.  
      // If it does, sleep until DirectSound gives us control.
      do 
      {
         hr = pDSB->Restore();
         if( hr == DSERR_BUFFERLOST )
            Sleep( 10 );
      }
      while( hr != DS_OK );

      if( pbWasRestored != NULL )
         *pbWasRestored = TRUE;

      return S_OK;
   }
   else
   {
      return S_FALSE;
   }
}




//-----------------------------------------------------------------------------
// Name: CSound::GetFreeBuffer()
// Desc: Checks to see if a buffer is playing and returns TRUE if it is.
//-----------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER CSound::GetFreeBuffer()
{
   if( m_apDSBuffer == NULL )
      return FALSE; 

   for( DWORD i=0; i<m_dwNumBuffers; i++ )
   {
      if( m_apDSBuffer[i] )
      {  
         DWORD dwStatus = 0;
         m_apDSBuffer[i]->GetStatus( &dwStatus );
         if ( ( dwStatus & DSBSTATUS_PLAYING ) == 0 )
            break;
      }
   }


   if( i != m_dwNumBuffers )
      m_CurrentBuffer = i;
   else
      m_CurrentBuffer =  rand() % m_dwNumBuffers;

   return m_apDSBuffer[m_CurrentBuffer];
}





//-----------------------------------------------------------------------------
// Name: CSound::GetBuffer()
// Desc: 
//-----------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER CSound::GetBuffer( DWORD dwIndex )
{
   if( m_apDSBuffer == NULL )
      return NULL;
   if( dwIndex >= m_dwNumBuffers )
      return NULL;

   return m_apDSBuffer[dwIndex];
}




//-----------------------------------------------------------------------------
// Name: CSound::Get3DBufferInterface()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CSound::Get3DBufferInterface( DWORD dwIndex, LPDIRECTSOUND3DBUFFER* ppDS3DBuffer )
{
   if( m_apDSBuffer == NULL )
      return CO_E_NOTINITIALIZED;
   if( dwIndex >= m_dwNumBuffers )
      return E_INVALIDARG;

   *ppDS3DBuffer = NULL;

   return m_apDSBuffer[dwIndex]->QueryInterface( IID_IDirectSound3DBuffer, 
      (VOID**)ppDS3DBuffer );
}


//-----------------------------------------------------------------------------
// Name: CSound::Play()
// Desc: Plays the sound using voice management flags.  Pass in DSBPLAY_LOOPING
//       in the dwFlags to loop the sound
//-----------------------------------------------------------------------------
HRESULT CSound::Play( DWORD dwPriority, DWORD dwFlags )
{
   HRESULT hr;
   BOOL    bRestored;

   if( m_apDSBuffer == NULL )
      return CO_E_NOTINITIALIZED;

   LPDIRECTSOUNDBUFFER pDSB = GetFreeBuffer();

   if( pDSB == NULL )
      return sys_msg_hr( TEXT("GetFreeBuffer"), E_FAIL );

   // Restore the buffer if it was lost
   if( FAILED( hr = RestoreBuffer( pDSB, &bRestored ) ) )
      return sys_msg_hr( TEXT("RestoreBuffer"), hr );

   if( bRestored )
   {
      // The buffer was restored, so we need to fill it with new data
      if( FAILED( hr = FillBufferWithSound( pDSB, FALSE ) ) )
         return sys_msg_hr( TEXT("FillBufferWithSound"), hr );

      // Make DirectSound do pre-processing on sound effects
      Reset();
   }

   return pDSB->Play( 0, dwPriority, dwFlags );
}

HRESULT CSound::Play3D(DWORD mode, D3DXVECTOR3 vel, D3DXVECTOR3 pos,FLOAT min , FLOAT max, DWORD dwPriority, DWORD dwFlags )
{
   HRESULT hr;
   BOOL    bRestored;

   if( m_apDSBuffer == NULL )
      return CO_E_NOTINITIALIZED;

   LPDIRECTSOUNDBUFFER pDSB = GetFreeBuffer();

   if( pDSB == NULL )
      return sys_msg_hr( TEXT("GetFreeBuffer"), E_FAIL );

   // Restore the buffer if it was lost
   if( FAILED( hr = RestoreBuffer( pDSB, &bRestored ) ) )
      return sys_msg_hr( TEXT("RestoreBuffer"), hr );

   if( bRestored )
   {
      // The buffer was restored, so we need to fill it with new data
      if( FAILED( hr = FillBufferWithSound( pDSB, FALSE ) ) )
         return sys_msg_hr( TEXT("FillBufferWithSound"), hr );

      // Make DirectSound do pre-processing on sound effects
      Reset();
   }

   LPDIRECTSOUND3DBUFFER   pDS3DBuffer = NULL;
   if( FAILED(pDSB->QueryInterface( IID_IDirectSound3DBuffer, 
      (VOID**) &pDS3DBuffer ) ) )
   {
      return E_FAIL;
   }

   pDS3DBuffer->SetMode( mode , DS3D_DEFERRED );
   pDS3DBuffer->SetPosition(pos.x,pos.y,pos.z,DS3D_DEFERRED );
   pDS3DBuffer->SetMinDistance(min, DS3D_DEFERRED);
   pDS3DBuffer->SetMaxDistance(max, DS3D_DEFERRED);
   pDS3DBuffer->SetVelocity(vel.x,vel.y,vel.z,DS3D_DEFERRED );
   pDS3DBuffer->SetConeOutsideVolume(DSBVOLUME_MAX,DS3D_DEFERRED);

   SAFE_RELEASE( pDS3DBuffer );


   return pDSB->Play( 0, dwPriority, dwFlags );
}




//-----------------------------------------------------------------------------
// Name: CSound::Stop()
// Desc: Stops the sound from playing
//-----------------------------------------------------------------------------
HRESULT CSound::Stop()
{
   if( m_apDSBuffer == NULL )
      return CO_E_NOTINITIALIZED;

   HRESULT hr = 0;

   for( DWORD i=0; i<m_dwNumBuffers; i++ )
      hr |= m_apDSBuffer[i]->Stop();

   return hr;
}




//-----------------------------------------------------------------------------
// Name: CSound::Reset()
// Desc: Reset all of the sound buffers
//-----------------------------------------------------------------------------
HRESULT CSound::Reset()
{
   if( m_apDSBuffer == NULL )
      return CO_E_NOTINITIALIZED;

   HRESULT hr = 0;

   for( DWORD i=0; i<m_dwNumBuffers; i++ )
      hr |= m_apDSBuffer[i]->SetCurrentPosition( 0 );

   return hr;
}




//-----------------------------------------------------------------------------
// Name: CSound::IsSoundPlaying()
// Desc: Checks to see if a buffer is playing and returns TRUE if it is.
//-----------------------------------------------------------------------------
BOOL CSound::IsSoundPlaying()
{
   BOOL bIsPlaying = FALSE;

   if( m_apDSBuffer == NULL )
      return FALSE; 

   for( DWORD i=0; i<m_dwNumBuffers; i++ )
   {
      if( m_apDSBuffer[i] )
      {  
         DWORD dwStatus = 0;
         m_apDSBuffer[i]->GetStatus( &dwStatus );
         bIsPlaying |= ( ( dwStatus & DSBSTATUS_PLAYING ) != 0 );
      }
   }

   return bIsPlaying;
}




//-----------------------------------------------------------------------------
// Name: CStreamingSound::CStreamingSound()
// Desc: Setups up a buffer so data can be streamed from the wave file into 
//       buffer.  This is very useful for large wav files that would take a 
//       while to load.  The buffer is initially filled with data, then 
//       as sound is played the notification events are signaled and more data
//       is written into the buffer by calling HandleWaveStreamNotification()
//-----------------------------------------------------------------------------
CStreamingSound::CStreamingSound( LPDIRECTSOUNDBUFFER pDSBuffer, DWORD dwDSBufferSize, 
                                 CWaveFile* pWaveFile, DWORD dwNotifySize ) 
                                 : CSound( &pDSBuffer, dwDSBufferSize, 1, pWaveFile )           
{
   m_dwLastPlayPos     = 0;
   m_dwPlayProgress    = 0;
   m_dwNotifySize      = dwNotifySize;
   m_dwNextWriteOffset = 0;
   m_bFillNextNotificationWithSilence = FALSE;
}




//-----------------------------------------------------------------------------
// Name: CStreamingSound::~CStreamingSound()
// Desc: Destroys the class
//-----------------------------------------------------------------------------
CStreamingSound::~CStreamingSound()
{
}



//-----------------------------------------------------------------------------
// Name: CStreamingSound::HandleWaveStreamNotification()
// Desc: Handle the notification that tell us to put more wav data in the 
//       circular buffer
//-----------------------------------------------------------------------------
HRESULT CStreamingSound::HandleWaveStreamNotification( BOOL bLoopedPlay )
{
   HRESULT hr;
   DWORD   dwCurrentPlayPos;
   DWORD   dwPlayDelta;
   DWORD   dwBytesWrittenToBuffer;
   VOID*   pDSLockedBuffer = NULL;
   VOID*   pDSLockedBuffer2 = NULL;
   DWORD   dwDSLockedBufferSize;
   DWORD   dwDSLockedBufferSize2;

   if( m_apDSBuffer == NULL || m_pWaveFile == NULL )
      return CO_E_NOTINITIALIZED;

   // Restore the buffer if it was lost
   BOOL bRestored;
   if( FAILED( hr = RestoreBuffer( m_apDSBuffer[0], &bRestored ) ) )
      return sys_msg_hr( TEXT("RestoreBuffer"), hr );

   if( bRestored )
   {
      // The buffer was restored, so we need to fill it with new data
      if( FAILED( hr = FillBufferWithSound( m_apDSBuffer[0], FALSE ) ) )
         return sys_msg_hr( TEXT("FillBufferWithSound"), hr );
      return S_OK;
   }

   // Lock the DirectSound buffer
   if( FAILED( hr = m_apDSBuffer[0]->Lock( m_dwNextWriteOffset, m_dwNotifySize, 
      &pDSLockedBuffer, &dwDSLockedBufferSize, 
      &pDSLockedBuffer2, &dwDSLockedBufferSize2, 0L ) ) )
      return sys_msg_hr( TEXT("Lock"), hr );

   // m_dwDSBufferSize and m_dwNextWriteOffset are both multiples of m_dwNotifySize, 
   // it should the second buffer should never be valid
   if( pDSLockedBuffer2 != NULL )
      return E_UNEXPECTED; 

   if( !m_bFillNextNotificationWithSilence )
   {
      // Fill the DirectSound buffer with wav data
      if( FAILED( hr = m_pWaveFile->Read( (BYTE*) pDSLockedBuffer, 
         dwDSLockedBufferSize, 
         &dwBytesWrittenToBuffer ) ) )           
         return sys_msg_hr( TEXT("Read"), hr );
   }
   else
   {
      // Fill the DirectSound buffer with silence
      FillMemory( pDSLockedBuffer, dwDSLockedBufferSize, 
         (BYTE)( m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
      dwBytesWrittenToBuffer = dwDSLockedBufferSize;
   }

   // If the number of bytes written is less than the 
   // amount we requested, we have a short file.
   if( dwBytesWrittenToBuffer < dwDSLockedBufferSize )
   {
      if( !bLoopedPlay ) 
      {
         // Fill in silence for the rest of the buffer.
         FillMemory( (BYTE*) pDSLockedBuffer + dwBytesWrittenToBuffer, 
            dwDSLockedBufferSize - dwBytesWrittenToBuffer, 
            (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );

         // Any future notifications should just fill the buffer with silence
         m_bFillNextNotificationWithSilence = TRUE;
      }
      else
      {
         // We are looping, so reset the file and fill the buffer with wav data
         DWORD dwReadSoFar = dwBytesWrittenToBuffer;    // From previous call above.
         while( dwReadSoFar < dwDSLockedBufferSize )
         {  
            // This will keep reading in until the buffer is full (for very short files).
            if( FAILED( hr = m_pWaveFile->ResetFile() ) )
               return sys_msg_hr( TEXT("ResetFile"), hr );

            if( FAILED( hr = m_pWaveFile->Read( (BYTE*)pDSLockedBuffer + dwReadSoFar,
               dwDSLockedBufferSize - dwReadSoFar,
               &dwBytesWrittenToBuffer ) ) )
               return sys_msg_hr( TEXT("Read"), hr );

            dwReadSoFar += dwBytesWrittenToBuffer;
         } 
      } 
   }

   // Unlock the DirectSound buffer
   m_apDSBuffer[0]->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

   // Figure out how much data has been played so far.  When we have played
   // passed the end of the file, we will either need to start filling the
   // buffer with silence or starting reading from the beginning of the file, 
   // depending if the user wants to loop the sound
   if( FAILED( hr = m_apDSBuffer[0]->GetCurrentPosition( &dwCurrentPlayPos, NULL ) ) )
      return sys_msg_hr( TEXT("GetCurrentPosition"), hr );

   // Check to see if the position counter looped
   if( dwCurrentPlayPos < m_dwLastPlayPos )
      dwPlayDelta = ( m_dwDSBufferSize - m_dwLastPlayPos ) + dwCurrentPlayPos;
   else
      dwPlayDelta = dwCurrentPlayPos - m_dwLastPlayPos;

   m_dwPlayProgress += dwPlayDelta;
   m_dwLastPlayPos = dwCurrentPlayPos;

   // If we are now filling the buffer with silence, then we have found the end so 
   // check to see if the entire sound has played, if it has then stop the buffer.
   if( m_bFillNextNotificationWithSilence )
   {
      // We don't want to cut off the sound before it's done playing.
      if( m_dwPlayProgress >= m_pWaveFile->GetSize() )
      {
         m_apDSBuffer[0]->Stop();
      }
   }

   // Update where the buffer will lock (for next time)
   m_dwNextWriteOffset += dwDSLockedBufferSize; 
   m_dwNextWriteOffset %= m_dwDSBufferSize; // Circular buffer

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CStreamingSound::Reset()
// Desc: Resets the sound so it will begin playing at the beginning
//-----------------------------------------------------------------------------
HRESULT CStreamingSound::Reset()
{
   HRESULT hr;

   if( m_apDSBuffer[0] == NULL || m_pWaveFile == NULL )
      return CO_E_NOTINITIALIZED;

   m_dwLastPlayPos     = 0;
   m_dwPlayProgress    = 0;
   m_dwNextWriteOffset = 0;
   m_bFillNextNotificationWithSilence = FALSE;

   // Restore the buffer if it was lost
   BOOL bRestored;
   if( FAILED( hr = RestoreBuffer( m_apDSBuffer[0], &bRestored ) ) )
      return sys_msg_hr( TEXT("RestoreBuffer"), hr );

   if( bRestored )
   {
      // The buffer was restored, so we need to fill it with new data
      if( FAILED( hr = FillBufferWithSound( m_apDSBuffer[0], FALSE ) ) )
         return sys_msg_hr( TEXT("FillBufferWithSound"), hr );
   }

   m_pWaveFile->ResetFile();

   return m_apDSBuffer[0]->SetCurrentPosition( 0L );  
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::CWaveFile()
// Desc: Constructs the class.  Call Open() to open a wave file for reading.  
//       Then call Read() as needed.  Calling the destructor or Close() 
//       will close the file.  
//-----------------------------------------------------------------------------
CWaveFile::CWaveFile()
{
   m_pwfx    = NULL;
   m_hmmio   = NULL;
   m_pResourceBuffer = NULL;
   m_dwSize  = 0;
   m_bIsReadingFromMemory = FALSE;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::~CWaveFile()
// Desc: Destructs the class
//-----------------------------------------------------------------------------
CWaveFile::~CWaveFile()
{
   Close();

   if( !m_bIsReadingFromMemory )
      SAFE_DELETE_ARRAY( m_pwfx );

   if(!m_FileName.IsEmpty())
      DeleteFile(m_FileName);
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Open()
// Desc: Opens a wave file for reading
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Open( LPCTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags, CZipArchive* pZip )
{

   HRESULT hr;

   m_dwFlags = dwFlags;
   m_bIsReadingFromMemory = FALSE;

   if( m_dwFlags == WAVEFILE_READ )
   {
      if( strFileName == NULL )
         return E_INVALIDARG;

      if(pZip)
      {
         m_FileName = strFileName;
         if(!pZip->ExtractFileTo(strFileName, strFileName))
            return E_FAIL;
      }

      SAFE_DELETE_ARRAY( m_pwfx );



      m_hmmio = mmioOpen( (LPTSTR)strFileName, NULL, MMIO_ALLOCBUF | MMIO_READ );



      if( NULL == m_hmmio )
      {
         HRSRC   hResInfo;
         HGLOBAL hResData;
         DWORD   dwSize;
         VOID*   pvRes;

         // Loading it as a file failed, so try it as a resource
         if( NULL == ( hResInfo = FindResource( NULL, strFileName, TEXT("WAVE") ) ) )
         {
            if( NULL == ( hResInfo = FindResource( NULL, strFileName, TEXT("WAV") ) ) )
               return sys_msg_hr( TEXT("FindResource"), E_FAIL );
         }

         if( NULL == ( hResData = LoadResource( NULL, hResInfo ) ) )
            return sys_msg_hr( TEXT("LoadResource"), E_FAIL );

         if( 0 == ( dwSize = SizeofResource( NULL, hResInfo ) ) ) 
            return sys_msg_hr( TEXT("SizeofResource"), E_FAIL );

         if( NULL == ( pvRes = LockResource( hResData ) ) )
            return sys_msg_hr( TEXT("LockResource"), E_FAIL );

         m_pResourceBuffer = new CHAR[ dwSize ];
         memcpy( m_pResourceBuffer, pvRes, dwSize );

         MMIOINFO mmioInfo;
         ZeroMemory( &mmioInfo, sizeof(mmioInfo) );
         mmioInfo.fccIOProc = FOURCC_MEM;
         mmioInfo.cchBuffer = dwSize;
         mmioInfo.pchBuffer = (CHAR*) m_pResourceBuffer;

         m_hmmio = mmioOpen( NULL, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );
      }

      if( FAILED( hr = ReadMMIO() ) )
      {
         // ReadMMIO will fail if its an not a wave file
         mmioClose( m_hmmio, 0 );
         return sys_msg_hr( TEXT("ReadMMIO"), hr );
      }

      if( FAILED( hr = ResetFile() ) )
         return sys_msg_hr( TEXT("ResetFile"), hr );

      // After the reset, the size of the wav file is m_ck.cksize so store it now
      m_dwSize = m_ck.cksize;
   }
   else
   {
      m_hmmio = mmioOpen( (LPTSTR)strFileName, NULL, MMIO_ALLOCBUF  | 
         MMIO_READWRITE | 
         MMIO_CREATE );
      if( NULL == m_hmmio )
         return sys_msg_hr( TEXT("mmioOpen"), E_FAIL );

      if( FAILED( hr = WriteMMIO( pwfx ) ) )
      {
         mmioClose( m_hmmio, 0 );
         return sys_msg_hr( TEXT("WriteMMIO"), hr );
      }

      if( FAILED( hr = ResetFile() ) )
         return sys_msg_hr( TEXT("ResetFile"), hr );
   }

   return hr;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::OpenFromMemory()
// Desc: copy data to CWaveFile member variable from memory
//-----------------------------------------------------------------------------
HRESULT CWaveFile::OpenFromMemory( BYTE* pbData, ULONG ulDataSize, 
                                  WAVEFORMATEX* pwfx, DWORD dwFlags )
{
   m_pwfx       = pwfx;
   m_ulDataSize = ulDataSize;
   m_pbData     = pbData;
   m_pbDataCur  = m_pbData;
   m_bIsReadingFromMemory = TRUE;

   if( dwFlags != WAVEFILE_READ )
      return E_NOTIMPL;       

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::ReadMMIO()
// Desc: Support function for reading from a multimedia I/O stream.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_pwfx. 
//-----------------------------------------------------------------------------
HRESULT CWaveFile::ReadMMIO()
{
   MMCKINFO        ckIn;           // chunk info. for general use.
   PCMWAVEFORMAT   pcmWaveFormat;  // Temp PCM structure to load in.       

   m_pwfx = NULL;

   if( ( 0 != mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) ) )
      return sys_msg_hr( TEXT("mmioDescend"), E_FAIL );

   // Check to make sure this is a valid wave file
   if( (m_ckRiff.ckid != FOURCC_RIFF) ||
      (m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
      return sys_msg_hr( TEXT("mmioFOURCC"), E_FAIL ); 

   // Search the input file for for the 'fmt ' chunk.
   ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
   if( 0 != mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK ) )
      return sys_msg_hr( TEXT("mmioDescend"), E_FAIL );

   // Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
   // if there are extra parameters at the end, we'll ignore them
   if( ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT) )
      return sys_msg_hr( TEXT("sizeof(PCMWAVEFORMAT)"), E_FAIL );

   // Read the 'fmt ' chunk into <pcmWaveFormat>.
   if( mmioRead( m_hmmio, (HPSTR) &pcmWaveFormat, 
      sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat) )
      return sys_msg_hr( TEXT("mmioRead"), E_FAIL );

   // Allocate the waveformatex, but if its not pcm format, read the next
   // word, and thats how many extra bytes to allocate.
   if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
   {
      m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) ];
      if( NULL == m_pwfx )
         return sys_msg_hr( TEXT("m_pwfx"), E_FAIL );

      // Copy the bytes from the pcm structure to the waveformatex structure
      memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
      m_pwfx->cbSize = 0;
   }
   else
   {
      // Read in length of extra bytes.
      WORD cbExtraBytes = 0L;
      if( mmioRead( m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD) )
         return sys_msg_hr( TEXT("mmioRead"), E_FAIL );

      m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) + cbExtraBytes ];
      if( NULL == m_pwfx )
         return sys_msg_hr( TEXT("new"), E_FAIL );

      // Copy the bytes from the pcm structure to the waveformatex structure
      memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
      m_pwfx->cbSize = cbExtraBytes;

      // Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
      if( mmioRead( m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize))+sizeof(WORD)),
         cbExtraBytes ) != cbExtraBytes )
      {
         SAFE_DELETE( m_pwfx );
         return sys_msg_hr( TEXT("mmioRead"), E_FAIL );
      }
   }

   // Ascend the input file out of the 'fmt ' chunk.
   if( 0 != mmioAscend( m_hmmio, &ckIn, 0 ) )
   {
      SAFE_DELETE( m_pwfx );
      return sys_msg_hr( TEXT("mmioAscend"), E_FAIL );
   }

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::GetSize()
// Desc: Retuns the size of the read access wave file 
//-----------------------------------------------------------------------------
DWORD CWaveFile::GetSize()
{
   return m_dwSize;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::ResetFile()
// Desc: Resets the internal m_ck pointer so reading starts from the 
//       beginning of the file again 
//-----------------------------------------------------------------------------
HRESULT CWaveFile::ResetFile()
{
   if( m_bIsReadingFromMemory )
   {
      m_pbDataCur = m_pbData;
   }
   else 
   {
      if( m_hmmio == NULL )
         return CO_E_NOTINITIALIZED;

      if( m_dwFlags == WAVEFILE_READ )
      {
         // Seek to the data
         if( -1 == mmioSeek( m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC),
            SEEK_SET ) )
            return sys_msg_hr( TEXT("mmioSeek"), E_FAIL );

         // Search the input file for the 'data' chunk.
         m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
         if( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
            return sys_msg_hr( TEXT("mmioDescend"), E_FAIL );
      }
      else
      {
         // Create the 'data' chunk that holds the waveform samples.  
         m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
         m_ck.cksize = 0;

         if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) ) 
            return sys_msg_hr( TEXT("mmioCreateChunk"), E_FAIL );

         if( 0 != mmioGetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
            return sys_msg_hr( TEXT("mmioGetInfo"), E_FAIL );
      }
   }

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Read()
// Desc: Reads section of data from a wave file into pBuffer and returns 
//       how much read in pdwSizeRead, reading not more than dwSizeToRead.
//       This uses m_ck to determine where to start reading from.  So 
//       subsequent calls will be continue where the last left off unless 
//       Reset() is called.
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
   if( m_bIsReadingFromMemory )
   {
      if( m_pbDataCur == NULL )
         return CO_E_NOTINITIALIZED;
      if( pdwSizeRead != NULL )
         *pdwSizeRead = 0;

      if( (BYTE*)(m_pbDataCur + dwSizeToRead) > 
         (BYTE*)(m_pbData + m_ulDataSize) )
      {
         dwSizeToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
      }

      CopyMemory( pBuffer, m_pbDataCur, dwSizeToRead );

      if( pdwSizeRead != NULL )
         *pdwSizeRead = dwSizeToRead;

      return S_OK;
   }
   else 
   {
      MMIOINFO mmioinfoIn; // current status of m_hmmio

      if( m_hmmio == NULL )
         return CO_E_NOTINITIALIZED;
      if( pBuffer == NULL || pdwSizeRead == NULL )
         return E_INVALIDARG;

      if( pdwSizeRead != NULL )
         *pdwSizeRead = 0;

      if( 0 != mmioGetInfo( m_hmmio, &mmioinfoIn, 0 ) )
         return sys_msg_hr( TEXT("mmioGetInfo"), E_FAIL );

      UINT cbDataIn = dwSizeToRead;
      if( cbDataIn > m_ck.cksize ) 
         cbDataIn = m_ck.cksize;       

      m_ck.cksize -= cbDataIn;

      for( DWORD cT = 0; cT < cbDataIn; cT++ )
      {
         // Copy the bytes from the io to the buffer.
         if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
         {
            if( 0 != mmioAdvance( m_hmmio, &mmioinfoIn, MMIO_READ ) )
               return sys_msg_hr( TEXT("mmioAdvance"), E_FAIL );

            if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
               return sys_msg_hr( TEXT("mmioinfoIn.pchNext"), E_FAIL );
         }

         // Actual copy.
         *((BYTE*)pBuffer+cT) = *((BYTE*)mmioinfoIn.pchNext);
         mmioinfoIn.pchNext++;
      }

      if( 0 != mmioSetInfo( m_hmmio, &mmioinfoIn, 0 ) )
         return sys_msg_hr( TEXT("mmioSetInfo"), E_FAIL );

      if( pdwSizeRead != NULL )
         *pdwSizeRead = cbDataIn;

      return S_OK;
   }
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Close()
// Desc: Closes the wave file 
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Close()
{
   if( m_dwFlags == WAVEFILE_READ )
   {
      mmioClose( m_hmmio, 0 );
      m_hmmio = NULL;
      SAFE_DELETE_ARRAY( m_pResourceBuffer );
   }
   else
   {
      m_mmioinfoOut.dwFlags |= MMIO_DIRTY;

      if( m_hmmio == NULL )
         return CO_E_NOTINITIALIZED;

      if( 0 != mmioSetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
         return sys_msg_hr( TEXT("mmioSetInfo"), E_FAIL );

      // Ascend the output file out of the 'data' chunk -- this will cause
      // the chunk size of the 'data' chunk to be written.
      if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
         return sys_msg_hr( TEXT("mmioAscend"), E_FAIL );

      // Do this here instead...
      if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
         return sys_msg_hr( TEXT("mmioAscend"), E_FAIL );

      mmioSeek( m_hmmio, 0, SEEK_SET );

      if( 0 != (INT)mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) )
         return sys_msg_hr( TEXT("mmioDescend"), E_FAIL );

      m_ck.ckid = mmioFOURCC('f', 'a', 'c', 't');

      if( 0 == mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) ) 
      {
         DWORD dwSamples = 0;
         mmioWrite( m_hmmio, (HPSTR)&dwSamples, sizeof(DWORD) );
         mmioAscend( m_hmmio, &m_ck, 0 ); 
      }

      // Ascend the output file out of the 'RIFF' chunk -- this will cause
      // the chunk size of the 'RIFF' chunk to be written.
      if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
         return sys_msg_hr( TEXT("mmioAscend"), E_FAIL );

      mmioClose( m_hmmio, 0 );
      m_hmmio = NULL;
   }

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::WriteMMIO()
// Desc: Support function for reading from a multimedia I/O stream
//       pwfxDest is the WAVEFORMATEX for this new wave file.  
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_ck.  
//-----------------------------------------------------------------------------
HRESULT CWaveFile::WriteMMIO( WAVEFORMATEX *pwfxDest )
{
   DWORD    dwFactChunk; // Contains the actual fact chunk. Garbage until WaveCloseWriteFile.
   MMCKINFO ckOut1;

   dwFactChunk = (DWORD)-1;

   // Create the output file RIFF chunk of form type 'WAVE'.
   m_ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');       
   m_ckRiff.cksize = 0;

   if( 0 != mmioCreateChunk( m_hmmio, &m_ckRiff, MMIO_CREATERIFF ) )
      return sys_msg_hr( TEXT("mmioCreateChunk"), E_FAIL );

   // We are now descended into the 'RIFF' chunk we just created.
   // Now create the 'fmt ' chunk. Since we know the size of this chunk,
   // specify it in the MMCKINFO structure so MMIO doesn't have to seek
   // back and set the chunk size after ascending from the chunk.
   m_ck.ckid = mmioFOURCC('f', 'm', 't', ' ');
   m_ck.cksize = sizeof(PCMWAVEFORMAT);   

   if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
      return sys_msg_hr( TEXT("mmioCreateChunk"), E_FAIL );

   // Write the PCMWAVEFORMAT structure to the 'fmt ' chunk if its that type. 
   if( pwfxDest->wFormatTag == WAVE_FORMAT_PCM )
   {
      if( mmioWrite( m_hmmio, (HPSTR) pwfxDest, 
         sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
         return sys_msg_hr( TEXT("mmioWrite"), E_FAIL );
   }   
   else 
   {
      // Write the variable length size.
      if( (UINT)mmioWrite( m_hmmio, (HPSTR) pwfxDest, 
         sizeof(*pwfxDest) + pwfxDest->cbSize ) != 
         ( sizeof(*pwfxDest) + pwfxDest->cbSize ) )
         return sys_msg_hr( TEXT("mmioWrite"), E_FAIL );
   }  

   // Ascend out of the 'fmt ' chunk, back into the 'RIFF' chunk.
   if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
      return sys_msg_hr( TEXT("mmioAscend"), E_FAIL );

   // Now create the fact chunk, not required for PCM but nice to have.  This is filled
   // in when the close routine is called.
   ckOut1.ckid = mmioFOURCC('f', 'a', 'c', 't');
   ckOut1.cksize = 0;

   if( 0 != mmioCreateChunk( m_hmmio, &ckOut1, 0 ) )
      return sys_msg_hr( TEXT("mmioCreateChunk"), E_FAIL );

   if( mmioWrite( m_hmmio, (HPSTR)&dwFactChunk, sizeof(dwFactChunk)) != 
      sizeof(dwFactChunk) )
      return sys_msg_hr( TEXT("mmioWrite"), E_FAIL );

   // Now ascend out of the fact chunk...
   if( 0 != mmioAscend( m_hmmio, &ckOut1, 0 ) )
      return sys_msg_hr( TEXT("mmioAscend"), E_FAIL );

   return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Write()
// Desc: Writes data to the open wave file
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Write( UINT nSizeToWrite, BYTE* pbSrcData, UINT* pnSizeWrote )
{
   UINT cT;

   if( m_bIsReadingFromMemory )
      return E_NOTIMPL;
   if( m_hmmio == NULL )
      return CO_E_NOTINITIALIZED;
   if( pnSizeWrote == NULL || pbSrcData == NULL )
      return E_INVALIDARG;

   *pnSizeWrote = 0;

   for( cT = 0; cT < nSizeToWrite; cT++ )
   {       
      if( m_mmioinfoOut.pchNext == m_mmioinfoOut.pchEndWrite )
      {
         m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
         if( 0 != mmioAdvance( m_hmmio, &m_mmioinfoOut, MMIO_WRITE ) )
            return sys_msg_hr( TEXT("mmioAdvance"), E_FAIL );
      }

      *((BYTE*)m_mmioinfoOut.pchNext) = *((BYTE*)pbSrcData+cT);
      (BYTE*)m_mmioinfoOut.pchNext++;

      (*pnSizeWrote)++;
   }

   return S_OK;
}




