//screen.cpp

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

#include "c:/library/screen.h"
#include "c:/library/stringc.h"
#include "c:/library/fileobj.h"
#include "c:/library/asm.h"
#include "c:/library/dirdraw.h"

//**************************************

     //****************************
     //****************************

DRAWING_MANAGER:: DRAWING_MANAGER (const int w, const int h, 
								   void* MessageLoop, void* hInst, 
								   const bool fullscreen ) 
		: ALPHABET_MANAGER ()
{
    WindowHandle = NULL;
    Width		 = w;
    Height	     = h;
    NUM_PIXEL    = Height*Width;
    MousePointer = NULL;
	Color		 = 65535;
    ClipRect.left= 0;
    ClipRect.top = 0;
    ClipRect.right  = Width-1;
    ClipRect.bottom = Height-1;

    WindowHandle =	NULL;

	Alphabet = RetrieveLetterArray ();
	FullScreenFlag = 1;
	DirDraw = new DIRECT_DRAW_MANAGER();
	WindowHandle = DirDraw->CreateMainWindow (w, h, "Generic",  
			MessageLoop, hInst, fullscreen);
	ColorType = DirDraw->GetColorType();
}

     //****************************

DRAWING_MANAGER:: ~DRAWING_MANAGER()
{
    if (MousePointer!=NULL) delete MousePointer;
	if (DirDraw) delete DirDraw;
}

     //****************************
     //****************************
     //****************************

void DRAWING_MANAGER:: SetClippingRect (const int x1, const int y1, const int x2, const int y2)
{
	px1 = x1, py1 = y1, px2 = x2, py2 = y2;
    if (px1<0) px1=0;
    if (py1<0) py1 = 0;
    if (px2>=Width) px2 = Width-1;
    if (py2>=Height) py2 = Height-1;
    ClipRect.left= px1;
    ClipRect.top = py1;
    ClipRect.right  = px2;
    ClipRect.bottom = py2; 
}

     //****************************

void DRAWING_MANAGER:: RestoreClippingRect ()
{
    ClipRect.left= 0;
    ClipRect.top = 0;
    ClipRect.right  = Width-1;
    ClipRect.bottom = Height-1;
}


     //****************************
     //****************************

SCREEN_STRUCT* DRAWING_MANAGER::ObtainBackBuffer () const
{
	if (DirDraw) return DirDraw->GetScreen();
	return NULL;
}

SCREEN_STRUCT* DRAWING_MANAGER::GetScreen () const {return ObtainBackBuffer(); }

     //****************************
     //****************************

void DRAWING_MANAGER::PrepareDrawing ()// prep and end drawing// call these before and at the end of any drawing cycle
{
	SCREEN_STRUCT* st = DirDraw->PrepFrame();
	Screen = st->Screen; 
	Width = st->Width, Height = st->Height;
	RealWidth = st->RealWidth;
}

void DRAWING_MANAGER::EndDrawing ()
{
	DirDraw->InterruptFrame();
	Screen = NULL;
}

int  DRAWING_MANAGER:: Swap () const 
{
	return DirDraw->DrawFrame ();
}


     //****************************

void DRAWING_MANAGER::SetColor (const U32 r, const U32 g, const U32 b )
{
	if (ColorType == DIRECT_DRAW_MANAGER::_5x5x5_)
	Color = (U16)(((r&248)<<7) + ((g&248)<<2) + ((b&248)>>3));
	else //_5x6x5_
	Color = (U16)(((r&248)<<8) + ((g&252)<<2) + ((b&248)>>3));
}

     //****************************

void DRAWING_MANAGER::ClearScreen ()
{
    if (Screen == NULL) return;
    Memset16Bit (Screen, Color, Width*Height);
}

     //****************************
     //****************************

void DRAWING_MANAGER::DrawOffSurface (const int x, const int y, const int which) const 
{
	DirDraw->DrawOffSurface (x, y, which);
}

 //**************************** off surface stuff

int  DRAWING_MANAGER::CreateBlankOffSurface (const int w, const int h) const
{
	return DirDraw->CreateBlankOffSurface (w, h);
}

SCREEN_STRUCT* DRAWING_MANAGER::ObtainPointerToOffSurface (const int which) const
{
	return DirDraw->GetOffSurface (which);
}

void DRAWING_MANAGER::ReleaseOffSurface (const int which) const
{
	DirDraw->ReleaseOffSurface (which);
}


     //****************************
     //****************************

//**************************************************
//**************************************
//**************************************
 
void DRAWING_MANAGER:: SetClipRect (int x, int y, int w, int h)
{
    if (Screen == NULL) return;
   // rect left, top, right, bottom
    ClipRect.left = x, ClipRect.top = y,
    ClipRect.right = w+x, ClipRect.bottom = h+y;
}

     //****************************

void DrawRowOfColor (const char* source, char* dest, int width, const char color)
{
    while (width--)
    {
        if (!*source++) *dest=color;
        dest++;
    }
}

     //****************************
     //****************************

int FastHypotenuse (int x1, int y1, int x2, int y2)
{
    if ((x2-=x1) <0) x2=-x2;
    if ((y2-=y1) <0) y2=-y2;
    return (x2+y2 - (((x2>y2) ? y2 : x2)>>1));
}

     //****************************
     //****************************

void DRAWING_MANAGER:: DrawBitmap (SCREEN_STRUCT* image, int x, int y)
{
	if (Screen == NULL) return;
	int wid = image->Width; 
	U16ptr mp = image->Screen;
	int w = wid, h = image->Height;// w may be shortened due to clipping, 
									//wid will be the offset
	if (x+w<0) return;
    if (y+h<0) return;
    if (x>=Width) return;
	if (y>=Height) return;  
	

	if (x >= Width-w)  w = Width-x;
	if (y >= Height-h) h = Height-y;
	if (x < 0) {mp-=x; w+=x; x=0;}
	if (y < 0) {mp-=y*wid; h+=y; y=0;}	

	U16ptr ptr = Screen + x + y*RealWidth;	

	while(h-- > 0)
	{
		Memcpy16Bit (ptr, mp, w);
		ptr+=Width, mp+=wid;
	} 
}
     //****************************

void DRAWING_MANAGER:: DrawBitmap (U16ptr image, 
								   int x, int y, int w, int h)
{
	if (Screen == NULL) return;
	int wid = w; 
	U16ptr mp = image;

	if (x+w<0) return;
    if (y+h<0) return;
    if (x>=Width) return;
	if (y>=Height) return;  	

	if (x >= Width-w)  w = Width-x;
	if (y >= Height-h) h = Height-y;
	if (x < 0) {mp-=x; w+=x; x=0;}
	if (y < 0) {mp-=y*wid; h+=y; y=0;}	

	U16ptr ptr = Screen + x + y*RealWidth;
	
	while(h-- > 0)
	{
		memcpy(ptr, mp, w);
		ptr+=Width, mp+=wid;
	} 
}

     //****************************
     //****************************

void DRAWING_MANAGER:: DrawMousePointer (const int x, const int y)
{
    if (Screen == NULL) return;
	px1 = x, py1 = y;

    if (px1<0) {px1=0;}
    if (py1<0) {py1=0;}
    if (px1>=Width) {px1=Width-1;}
	if (py1>=Height) {py1=Height-1;}
    PosX=(Uint16)px1; PosY=(Uint16)py1;
	int wid = 8; if (px1 > Width-8)  wid = Width-px1;
	int hgt = 8; if (py1 > Height-8) hgt = Height-py1;

	char *mp = (char*)MousePointer;


	U16* ptr = (U16*) Screen;
		ptr += px1+py1*Width ;


	for (int i=0; i<hgt; i++)
	{
		U16* toptr = ptr;
		char* tmp = mp;
		for (int j=0; j<wid; j++)
		{
			if( *tmp++) *toptr =65535;// always white
			toptr++;
		}
		ptr+=Width; mp+=8;
	}

    
}

     //****************************
     //****************************

void DRAWING_MANAGER:: MousePointerPrepare(void)
{
    MousePointer = new U8[64];
    U8 ptr[]={
        1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        1, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1};
        
    memcpy (MousePointer, ptr, 64);
}

     //****************************
     //****************************

void DRAWING_MANAGER:: InterpretedLine(int x1,int y1,int x2, int y2)
{
    if (Screen == NULL) return;
    if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2);}
    int diffy=y2-y1; if (!diffy) return;
   // if (y1<0) {y1=0;}
   // if (y2>=Height) {y2=Height-1;}
    // we need more error checking
    int diffx = (x2-x1)<<16, ChangePerScanLine = diffx/diffy;    
    U16ptr ptr = Screen+x1+y1*Width;
    x1=0; diffy++;

    while(diffy--)
    {
        *(ptr+(x1>>16))=Color;
        x1+=ChangePerScanLine;
        ptr+=Width;
    }
}

     //****************************
     //****************************

void DRAWING_MANAGER:: FastLine (const int x1, const int y1, const int x2, const int y2)
{
	if (Screen == NULL) return;

    px1 = x1, py1 = y1, px2 = x2, py2 = y2;
    if (Clipper() ) return;
    if ( py1==py2 ){ HorizontalLine ();return;}
    if ( px1==px2 ){ VerticalLine (); return;}   
   
    //if ( y1==y2 ) {HorizontalLine ((Uint16)y1, (Uint16)x1, (Uint16)x2);return;}
    //if ( x1==x2 ) {VerticalLine ((Uint16)x1, (Uint16)y1, (Uint16)y2); return;}
    int distx = x2-x1, disty = y2-y1;
   
    int len = FastHypotenuse ( x1, y1, x2, y2 );
    int addx = (distx<<16)/len, addy = (disty<<16)/len;
    int workx = x1<<16, worky = y1<<16;

    while(len--)
    {
        Plot ( workx>>16, worky>>16 );
        workx += addx; worky += addy;       
    }   
}

     //****************************

void DRAWING_MANAGER:: BlendedLine(int x1,int y1,int x2, int y2, UCHAR c1, UCHAR c2)
{
 /*   if (Screen == NULL) return;
    //int shift=16;
   px1=x1, py1=y1, px2=x2, py2=y2;
   if (Clipper()) return;

   int distx=x2-x1, disty=y2-y1;
   
   int len=FastHypotenuse(x1, y1, x2, y2);
   int addx=(distx<<16)/len, addy=(disty<<16)/len;
   int workx=x1<<16, worky=y1<<16;
   unsigned int r1 = PalEntry[c1].peRed, g1 = PalEntry[c1].peGreen, b1 = PalEntry[c1].peBlue;
   unsigned int r2 = PalEntry[c2].peRed, g2 = PalEntry[c2].peGreen, b2 = PalEntry[c2].peBlue;
   //we must shift here because the palentries only have 8bits to work with
   r1<<=16; g1<<=16; b1<<=16;
   r2<<=16; g2<<=16; b2<<=16;
   int diffr = (r1-r2)/len;
   int diffg = (g1-g2)/len;
   int diffb = (b1-b2)/len;

   while(len--)
   {
	   Color = (U8)ColorMatch((UCHAR)(r2>>16), (UCHAR)(g2>>16), (UCHAR)(b2>>16));
       Plot(workx>>16, worky>>16);
       workx+=addx; worky+=addy;
       r2-=diffr; 
	   g2-=diffg; 
	   b2-=diffb;
   }*/   
}

     //****************************

void DRAWING_MANAGER:: BlendedRect(int x1,int y1,int x2, int y2, UCHAR c1, UCHAR c2)// left to right blend
{
/*	if (Screen == NULL) return;
   
    int len = x2-x1;
	if (len<1) return;

    unsigned int r1 = PalEntry[c1].peRed, g1 = PalEntry[c1].peGreen, b1 = PalEntry[c1].peBlue;
    unsigned int r2 = PalEntry[c2].peRed, g2 = PalEntry[c2].peGreen, b2 = PalEntry[c2].peBlue;

    //we must shift here because the palentries only have 8bits to work with
    r1<<=16; g1<<=16; b1<<=16;
    r2<<=16; g2<<=16; b2<<=16;
    int diffr = (r1-r2)/len;
    int diffg = (g1-g2)/len;
    int diffb = (b1-b2)/len;
	unsigned int lastr = r1>>16, lastg = g1>>16, lastb = b1>>16;

	Color = (U8) ColorMatch( (U8)lastr, (U8)lastg, (U8)lastb);
    while(len--)
    {
	    
		if (  ((r1>>16) != lastr) || ((g1>>16) != lastg) || ((b1>>16) != lastb)  )
		{
			lastr = r1>>16, lastg = g1>>16, lastb = b1>>16;
			Color = (U8)ColorMatch((U8)lastr, (U8)lastg, (U8)lastb);
		}

		VerticalLine(x1, y1, y2);
		x1++;
		r1-=diffr; 
		g1-=diffg; 
		b1-=diffb;
    }  */
}

     //****************************

void DRAWING_MANAGER:: Line(const int x1, const int y1, const int x2, const int y2)
{
   if (Screen == NULL) return;
   px1=x1, py1=y1, px2=x2, py2=y2;
   if (Clipper()) return;
   if (py1==py2){ HorizontalLine();return;}
   if (px1==px2){ VerticalLine(); return;}   
//1)setup stage 
   
  int y_unit,x_unit;						// Variables for amount of change in x and y

  U16* offset= (U16*) Screen;
  offset += py1*Width + px1; 
  int ydiff=py2-py1;					// Calculate difference between y coordinates
                 
  if (ydiff<0)							// If the line moves in the negative direction
  {                        
	ydiff=-ydiff;                       // ...get absolute value of difference
	y_unit=-Width;                   // ...and set negative unit in y dimension
  }
  else y_unit=Width;					// Else set positive unit in y dimension

  int xdiff=px2-px1;					// Calculate difference between x coordinates
  if (xdiff<0)							// If the line moves in the negative direction
  {                      
		  xdiff=-xdiff;					// ...get absolute value of difference
		  x_unit=-1;					// ...and set negative unit in x dimension
  }
  else x_unit=1;						// Else set positive unit in y dimension
  int error_term=0;						// Initialize error term

//2) drawing stage
  if (xdiff>ydiff)						// If difference is bigger in x dimension
  {    
	  int length=xdiff+1;				// ...prepare to count off in x direction
	for (int i=0; i<length; i++)		// Loop through points in x direction
	{
	  *offset=Color;					// Set the next pixel in the line to COLOR
	  offset+=x_unit;					// Move offset to next pixel in x direction
	  error_term+=ydiff;				// Check to see if move required in y direction
	  if (error_term>xdiff)
	  {        // If so...
		error_term-=xdiff;				// ...reset error term
		offset+=y_unit;					// ...and move offset to next pixel in y dir.
	  }
	}
  }
  else
  {										// If difference is bigger in  y dimension
	int length=ydiff+1;					// ...prepare to count off in  y direction
	for (int i=0; i<length; i++)		// Loop through points in y direction
	{                     
		*offset=Color;				// Set the next pixel in the line to COLOR
		offset+=y_unit;					// Move offset to next pixel  in y direction
		error_term+=xdiff;    
	  if (error_term>0)					// Check to see if move required in x direction
	  {									// If so...
		error_term-=ydiff;				// ...reset error term
		offset+=x_unit;					// ...and move offset to next pixel in x dir.
	  }
	}
  }

}

     //****************************

void DRAWING_MANAGER:: HorizontalLine ()// horizontal line drawing
{
    if (Screen == NULL) return;
    if (px1==px2) return;
    if (py1>-1 && py1<Height)
    {
		if (px1>px2)   SWAP (px1, px2); // align left to right
		if(px2<0 || px1>Width) return;
		if (px1<0) px1=0; if (px2>=Width) px2=Width-1;    

		U16* sc = (U16*) Screen;
			sc += py1*Width+px1;
		Memset16Bit (sc, Color, px2-px1);
    }
}

     //****************************

void DRAWING_MANAGER:: VerticalLine ()
{
    if (Screen == NULL) return;
    if (px1>-1 && px1<Width)
	{
        if (py1>py2) SWAP(py1, py2);
            if (py1<0)py1=0;
            if (py2>=Height) py2=Height-1;
            if (py2<0 || py1>=Height)return;
        
		int h = py2-py1+1;

		U16* scr = (U16*)Screen;
			scr += px1+py1*Width;// we only need x or xt
		while (h--)
		{
			*scr=Color; scr+=Width;
		}
	}
}

     //****************************

void DRAWING_MANAGER:: HorizontalLine (int y, int x1, int x2) const// horizontal line drawing
{
    if (Screen == NULL) return;
    if (x1>x2)   SWAP (x1, x2); // align left to right
    if(x2<0 || x1>Width) return;
    if (x1<0) x1=0; if (x2>=Width) x2=Width-1;

	U16* sc = Screen;
		sc +=y*Width+x1;
	Memset16Bit (sc, Color, x2-x1);    
}

     //****************************

void DRAWING_MANAGER:: VerticalLine(int x, int y1, int y2) const
{
    if (Screen == NULL) return;
    if (x>-1 && x<Width)
	{
        if (y1>y2) SWAP(y1, y2);
            if (y1<0)y1=0;
            if (y2>=Height) y2=Height-1;
            if (y2<0 || y1>=Height)return;
        
		int h = y2-y1;

		U16* scr = Screen+x+y1*Width;
		while (h--)
		{
			*scr=Color; scr+=Width;
		}
	}
}


     //****************************
     //****************************

void DRAWING_MANAGER:: LoadMousePointer (const STRINGC& file)
{
}

     //****************************
     //****************************

     //****************************

void DRAWING_MANAGER:: Plot (const int x)
{
    //if (Screen == NULL) return;
    Screen[x]=Color;
}

     //****************************

void DRAWING_MANAGER:: Plot (const int x1, const int y1)
{
	//if (Screen == NULL) return;
	Screen[ y1*Width + x1] = Color;
}

     //****************************
     //****************************

void DRAWING_MANAGER:: FrameRect (const RECTA& rect) const
{
    FrameRect (rect.l, rect.t, rect.r, rect.b);
}

     //****************************

void DRAWING_MANAGER:: FrameRect (const RECTANGLE& rect) const
{
	FrameRect(rect.left, rect.top, rect.right, rect.bottom);
}

     //****************************

void DRAWING_MANAGER:: FrameRect (int x1, int y1, int x2, int y2) const
{
    HorizontalLine (y1, x1, x2);
    HorizontalLine (y2, x1, x2);
    VerticalLine (x1, y1, y2);
    VerticalLine (x2, y1, y2);
}

     //****************************
     //****************************

void DRAWING_MANAGER:: FillRect (const RECTA& rect) const
{
    FillRect(rect.l, rect.t, rect.r, rect.b);
}

     //****************************
 
void DRAWING_MANAGER:: FillRect (const RECTANGLE& rect) const
{
	FillRect(rect.left, rect.top, rect.right, rect.bottom);
}

     //****************************

void DRAWING_MANAGER:: FillRect (int x1, int y1, int x2, int y2) const
{
    if (Screen == NULL) return;
    if (x1>x2) SWAP (x1, x2);
    if (y1>y2) SWAP (y1, y2);
    if (x2<0) return;
    if (x1>=Width) return;
    if (y2<0) return;
    if (y1>=Height) return;
    if (x1<0) x1=0;
    if (x2>=Width) x2= Width-1;
    if (y1<0) y1=0;
    if (y2>=Height) y2= Height-1;

    int h=y2-y1, w=x2-x1;
	U16* off = (U16*)Screen;
	off += x1 + y1 * Width;

	while(h--)
	{
		Memset16Bit (off, Color, w);
		off+=Width;
	}
}

     //****************************
     //****************************

void DRAWING_MANAGER:: FillDiamond (const int x1, const int y1, const int x2, const int y2)
{
    if (Screen == NULL) return;
	px1 = x1, py1 = y1, px2 = x2, py2 = y2;

    if (px1>px2) SWAP (px1, px2);
    if (py1>py2) SWAP (py1, py2);
    if (px2<0) return;
    if (px1>=Width) return;
    if (py2<0) return;
    if (py1>=Height) return;
    if (px1<0) px1=0;
    if (px2>=Width) px2= Width-1;
    if (py1<0) py1=0;
    if (py2>=Height) py2= Height-1;

    int h = py2-y1, w = px2-x1;
    int halfH = h>>1, halfW=w>>1, Rows = halfH + h%2;
    int add=(halfH<<16)/halfW;
    
    U16ptr off = Screen + px1 + py1 * Width;

    while(Rows--)
    {
        U16ptr top = off - Rows*Width -(((halfH-Rows)*add)>>16);
        memset(top, Color, (((halfH-Rows)*add)>>16)*2);
        U16ptr bottom = off + Rows*Width -(((halfH-Rows)*add)>>16);
        memset(bottom, Color, (((halfH-Rows)*add)>>16)*2);
        
    }   
}

     //****************************
     //****************************

//**************************************
//void DRAWING_MANAGER:: AddColor(Uint8 c){Color+=c;}
//void DRAWING_MANAGER:: SetColor(Uint8 c){color=c;}
    
//**************************************

inline void memcpy255as1 (Uint8* dest, const Uint8* source, int num)
{
    while (num--)
    {
        if (*source==255) *dest=1;
        else *dest=*source;
        dest++; source++;
    }
}

     //****************************
     //****************************

void DRAWING_MANAGER:: SetClippingRect (const RECTANGLE* r)
{
    memcpy(&ClipRect, r, sizeof(RECT));
    if (ClipRect.left > ClipRect.right) SWAP(ClipRect.left, ClipRect.right);
    if (ClipRect.top > ClipRect.bottom) SWAP(ClipRect.top, ClipRect.bottom);
    
    if (ClipRect.left < 0) ClipRect.left = 0;// clip
    if (ClipRect.right >= Width) ClipRect.right = Width-1;
    if (ClipRect.top < 0) ClipRect.top = 0;
    if (ClipRect.bottom >= Height) ClipRect.bottom = Height-1;
}

     //****************************

void DRAWING_MANAGER:: SetClippingRect (const RECT2* r)
{
    memcpy(&ClipRect, r, sizeof(RECT));
    ClipRect.right+=ClipRect.left; // correction for different data types
    ClipRect.bottom+=ClipRect.top;
    if (ClipRect.left > ClipRect.right) SWAP(ClipRect.left, ClipRect.right);
    if (ClipRect.top > ClipRect.bottom) SWAP(ClipRect.top, ClipRect.bottom);
    
    if (ClipRect.left < 0) ClipRect.left = 0;//clip
    if (ClipRect.right >= Width) ClipRect.right = Width-1;
    if (ClipRect.top < 0) ClipRect.top = 0;
    if (ClipRect.bottom >= Height) ClipRect.bottom = Height-1;
}

     //****************************
     //****************************

int     DRAWING_MANAGER:: Clipper ()  // returns 1 if no line is drawn	
{
    if (py1==py2)// line off the screen test
    {
        if (py1<=ClipRect.top) return 1;
        if (py1>=ClipRect.bottom) return 1;
    }
    if (px1==px2)
    {
        if (px1<=ClipRect.left) return 1;
        if (px1>=ClipRect.right) return 1;
    }
    // -------------------------------- clip on the right and bottom
    if (px1>=ClipRect.right)
    {
        if (py2>=ClipRect.bottom || py2<ClipRect.top) return 1;
        int edge=ClipRect.right-1;
        int ratio=(((edge-px2)<<16) / (px1-px2)) ;//r= new / old
        px1=edge;// clipped to width of screen
        py1=py2 + ((((py1-py2)*ratio)>>16));// new= old* r
    }
   if (py1>=ClipRect.bottom)
    {
        if (px2>=ClipRect.right || px2<0) return 1;
        int edge=ClipRect.bottom-1;
        int ratio=(((edge-py2)<<16) / (py1-py2));//r= new / old
        py1=edge;// clipped to height of screen
        px1=px2 + ((((px1-px2) * ratio)>>16));
    }
    if (px2>=ClipRect.right)
    {
        if (py1>=ClipRect.bottom || py1<0) return 1;
        int edge=ClipRect.right-1;
        int ratio=(((edge-px1)<<16)/((px2-px1))) ;//r= new / old
        px2=edge;// clipped to width of screen
        py2=py1 + (((py2-py1) * ratio)>>16);// new= old* r            
                
    }
    if (py2>=ClipRect.bottom)
    {
        if (px1>=ClipRect.right || px1<0)return 1;
        int edge=ClipRect.bottom-1;
        int ratio=(((edge-py1)<<16) / (py2-py1));//r= new / old
        py2=edge; // clipped to height of screen     
        px2=px1 + ((((px2-px1) * ratio)>>16));
    }
    //----------------------------------clip on the top and left
   if (px1<ClipRect.left)
    {if (py2>ClipRect.bottom || py2<0) return 1;
        int edge=ClipRect.left;
        int ratio=(((edge-px2)<<16) / (px1-px2)) ;//r= new / old
        px1=edge;// clipped to left     
        py1=py2 + ((((py1-py2) * ratio)>>16));// new= old* r       
    }
   if (py1<ClipRect.top)
    {if (px2>ClipRect.right || px2<0) return 1;
        int edge=ClipRect.top;
        int ratio=(((edge-py2)<<16) / (py1-py2));//r= new / old
        py1=edge; // clipped to top
        px1=px2 + ((((px1-px2) * ratio)>>16));
    }
    if (px2<ClipRect.left)
    {if (py1>ClipRect.bottom || py1<0) return 1;
        int edge=ClipRect.left;
        int ratio=(((edge-px1)<<16) / (px2-px1)) ;//r= new / old
        px2=edge;// clipped to left
        py2=py1 + ((((py2-py1) * ratio)>>16));// new= old* r       
    }
    if (py2<ClipRect.top)
    {if (px1>ClipRect.right || px1<0) return 1;
        int edge=ClipRect.top;
        int ratio=(((edge-py1)<<16) / (py2-py1));//r= new / old
        py2=edge; // clipped to top
        px2=px1 + ((((px2-px1) * ratio)>>16));
    }
    return 0;
}
//*******************************

//********************************************************
//-------------------------text---------------------------
//********************************************************

void  DRAWING_MANAGER:: PaintText (const int x, const int y, const char* string) 
{
	DirDraw->PaintText (x, y, string);
}

void DRAWING_MANAGER:: PaintText (const int x, const int y, STRINGC& string)
{
	char* ptr = string.GetString ();
	DirDraw->PaintText (x, y, ptr);
}
     //****************************
/*
void DRAWING_MANAGER::  PrintText(int x, int y, char* text)
{
    int len=strlen (text);
    PosX=(Uint16)x; PosY=(Uint16)y;
    for (int i=0; i<len; i++, PosX+=8)
    {
        PrintASCIIchr (text[i]);
    }    
}

     //****************************

void  DRAWING_MANAGER::  PrintNumber(int x, int y, int num)
{
	char text[20];
    itoa(num, text, 10);
    int len=strlen (text);
    PosX=(Uint16)x; PosY=(Uint16)y;
    for (int i=len; i>=0; i--, PosX-=8)
    {
        PrintASCIIchr (text[i]);
    }   
}
*/
     //****************************



     //****************************

// to clip GDI text
//HRGN hRgn= CreateRectRgn(left, top, right, bottom);
//HRGN hRgn= CreateRectRgnIndirect(&rect);
//SelectClipRgn(hdc, hRgn);








