//screen.h

//******************************************
// copyright 1998	Mickey Kawick
// This file may be used anywhere for free, but this
// comment info must be left here.

// If you use this code in any shape or form or even
// as a reference for profit send me a dollar. 
// Not a dollar per copy, just a single US dollar. 

// If you are not in the US, send me
// a few different coins or the equivalent of a dollar.
// I love foreign currency, and I have a little collection.
// MKawick@sprintmail.com
//******************************************

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifndef __SCREEN_H_
#define __SCREEN_H_

#define FC __fastcall

#ifndef __ALPHABET_H_
#include "C:/library/alphabet.h"
#endif
/*
#ifndef __DIRDRAW_H_
#define __DIRDRAW_H_
#include "C:/library/dirdraw.h"
#endif*/

#include <windows.h>


//****************************************
//****************************************

//typedef void* MessageProc;
class	SPRITE;
struct	RECT2;
class	POLY_F;
class	STRINGC;
class	DIRECT_DRAW_MANAGER;

//****************************************
//****************************************


//****************************************
//****************************************

//****************************************
//****************************************

typedef class DRAWING_MANAGER : public ALPHABET_MANAGER
{
public:
    enum STATUS {BAD, GOOD};
	enum {WHITE=255, BLACK=0, RED=249, BLUE=252, YELLOW=251, GREEN=250, ORANGE=194};
	enum {NONE, _8BIT, _16BIT_5X5X5_, _16BIT_5X6X5_};
    
protected:
    
    int			FullScreenFlag;
	int			ColorType;

    U8ptr		MousePointer;
	U16			Color;
    RECTANGLE	ClipRect;
	void*		WindowHandle;

    int			px1, py1, px2, py2;			// points used for lines and clipping
    //U8*			Alphabet;				// Alphabet pointer
    int			NUM_PIXEL;					// the number of pixels on the screen / stored for speed only

	DIRECT_DRAW_MANAGER* DirDraw;
     //****************************

    void SetClipRect (int x, int y, int w, int h);		// used by 3 different clipping set routines below
	void CreateOffSurface (int which, int w, int h);
	int  Clipper ();

	void HorizontalLine ();
    void VerticalLine (); 
	void ErrorMessage (char* str);
    
public:
  
     //****************************

	DRAWING_MANAGER (const int w, const int h, 
					void* MessageLoop, void* hInst, 
					const bool fullscreen = true);

    ~DRAWING_MANAGER ();
	void*GetWindowHandle () const {return WindowHandle;}

     //**************************** direct draw stuff

    int  FC CreateDirectDraw (const void*);// hand it an HWND
	SCREEN_STRUCT* ObtainBackBuffer () const;	
	SCREEN_STRUCT* GetScreen () const;

     //**************************** off surface stuff

	void DrawOffSurface (const int x, const int y, const int which) const;
	int  CreateBlankOffSurface (const int w, const int h) const; 
	SCREEN_STRUCT* 
		 ObtainPointerToOffSurface (const int which) const;// lock
	void ReleaseOffSurface (const int which) const;// unlock	

     //**************************** before and after the drawing process

    void PrepareDrawing ();// prep and end drawing// call these before and at the end of any drawing cycle
    void EndDrawing ();

	 //**************************** page flipping

    void ClearScreen ();
    int  Swap () const;// moves offscreen into screen, returns 0 if OK and 
	// any other value for "try again later"

     //**************************** draw external data

	// this is a straight image without any compression
    void DrawBitmap (SCREEN_STRUCT* image, int x, int y);
	void DrawBitmap (U16ptr image, int x, int y, int w, int h);

     //**************************** lines

    void Line (const int x1, const int y1, const int x2, const int y2);

     //**************************** special lines

    void FastLine (const int x1, const int y1, const int x2, const int y2);
    void InterpretedLine (const int x1, const int y1, const int x2, const int y2);
    void HorizontalLine (int y, int x1, int x2) const;
    void VerticalLine (int x, int y1, int y2) const;  
	void BlendedLine (const int x1, const int y1, const int x2, const int y2, 
						const UCHAR c1, const UCHAR c2);

     //**************************** mouse
	
    void LoadMousePointer (const STRINGC& file);
    void MousePointerPrepare ();
	void DrawMousePointer (const int x1, const int y1);

     //**************************** pixels 

    void Plot (const int pos);
	void Plot (const int x1, const int y1);

     //**************************** rectangles

    void FrameRect (const RECTA& rect) const;
	void FrameRect (const RECTANGLE& rect) const;
    void FrameRect (int x1, int y1, int x2, int y2) const;
    void FillRect (const RECTA& rect) const;
	void FillRect (const RECTANGLE& rect) const;
    void FillRect (int x1, int y1, int x2, int y2) const;
	void BlendedRect (const int x1, const int y1, const int x2, const int y2, 
						const UCHAR c1, const UCHAR c2);
    void FillDiamond (const int x1, const int y1, const int x2, const int y2);

     //**************************** circles

	void Circle (const int x, const int y, const int radius);
	void FilledCircle (const int x, const int y, const int radius);

     //**************************** set params

    void AddColor (const char c);
	void SetColor (const U16 c) {Color = c;}
	void SetColor (const U32 r, const U32 g, const U32 b);
    void SetClippingRect (const RECTANGLE* r);
    void SetClippingRect (const RECT2* r);
    void SetClippingRect (const int x1, const int y1, const int x2, const int y2);
    void RestoreClippingRect ();

     //**************************** filled polys

    void DrawColoredPoly (const SPRITE*sp);
    void DrawColoredPoly (const POLY_F*sp);

     //**************************** text

    void PassAlphabet (const U8* ar) {Alphabet = (U8*) ar;}
 /*   void SetPosition (int x, int y) {PosX=x; PosY=y;}
    void DrawLetter (char which);
    void DrawText (char* text);
    void DrawNumber (int num);
    void DrawLetter (U8* stuff);
    void PrintText (int x, int y, char* text);
    void PrintNumber (int x, int y, int num);*/

     //**************************** GDI text

    void PaintText (const int x, const int y, const char* string);
	void PaintText (const int x, const int y, STRINGC& string);

}DRAWING_MANAGER, *DRAWING_MANAGERptr, **DRAWING_MANAGERlist;

#undef FC 
#endif


//****************************************
//****************************************
//****************************************

