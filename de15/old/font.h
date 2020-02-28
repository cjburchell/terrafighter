// font.h : header file
//
// an output object for direct draw
// by Christiaan Burchell		97/08/27
/////////////////////////////////////////////////////////////////////////////
#ifndef __FONT_H
#define __FONT_H
#include "ddutil.h"

class font
{
// Construction
public:
	font(CDisplay* pDD,HWND hWnd);
// outtext outputs text to the back buffer
void outtexts(
			 int x, // IN: x position of the text 
			 int y, // IN: y position of the text 
			 char* s // IN: the string to be outputed
			 );
void outtext(int x, int y, char* format, ...);

// Implementation
public:
	~font();

protected:
	CDisplay* pDirDraw;
	HWND hWindow;
};
#endif

/////////////////////////////////////////////////////////////////////////////
