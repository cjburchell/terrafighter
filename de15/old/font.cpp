// font.cpp : implementation file
//
// an output object for direct draw
// by Christiaan Burchell		97/08/27
/////////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "font.h"
#include "dxutil.h"

/////////////////////////////////////////////////////////////////////////////
// font


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


font::font(CDisplay* pDD,HWND hWnd)
{
	pDirDraw=pDD;
	hWindow=hWnd;
}

// outtext outputs text to the back buffer
void font::outtexts(
			 int x, // IN: x position of the text 
			 int y, // IN: y position of the text 
			 char* s // IN: the string to be outputed
			 )
{

   CFont font;
   VERIFY(font.CreateFont(
   12,                        // nHeight
   0,                         // nWidth
   0,                         // nEscapement
   0,                         // nOrientation
   FW_NORMAL,                 // nWeight
   FALSE,                     // bItalic
   FALSE,                     // bUnderline
   0,                         // cStrikeOut
   ANSI_CHARSET,              // nCharSet
   OUT_DEFAULT_PRECIS,        // nOutPrecision
   CLIP_DEFAULT_PRECIS,       // nClipPrecision
   DEFAULT_QUALITY,           // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   "Courier"));               // lpszFacename
   CSurface* FontSerface = NULL;


	if(FAILED(pDirDraw->CreateSurfaceFromText(&FontSerface,font,s,RGB(0,0,255),RGB(255,255,255))))
	{
		//pDirDraw->FlipToGDISurface();
		//MessageBox(hWindow,"can't open file","ERROR",MB_ICONERROR|MB_OK);
	}
	else
	{
		pDirDraw->Blt(x*5,y*13,FontSerface);
		SAFE_DELETE(FontSerface);
	}
}


void font::outtext(int x, int y, char* format, ...)
{
	va_list ap;
	char s[500];

	if(format==NULL)
		return;
	va_start(ap,format);
	vsprintf(s,format,ap);
	va_end(ap);
	int i = 0;
	while(s[i] != '\0')
	{
		if(s[i] == '-')
			s[i] = ' ';
		i++;
	}
	outtexts(x,y,s);
}

/////////////////////////////////////////////////////////////////////
// destructor
font::~font()
{
}
