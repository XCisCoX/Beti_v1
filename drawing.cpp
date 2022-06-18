#include "drawing.h"
#include "client.h"
#include "Engine.h"
#include "color.h"
#include "cSprites.h"
#include "cvar.h"
bool oglSubtractive;
int displayCenterX,displayCenterY;
SCREENINFO g_Screen;
int Count = 0;
void CDrawing::InitVisuals(void)
{
	static bool bOnce = false;
	if(bOnce) return;
	g_Screen.iSize = sizeof(SCREENINFO);
	pfnGetScreenInfo(&g_Screen);
	displayCenterX = g_Screen.iWidth/2;
	displayCenterY = g_Screen.iHeight/2;
	spritedata.iSpritesLoaded = 0;
	int o = 1;
	hookpfnSPR_GetList("sprites/WEAPONLIST_knife.txt",&o);
	bOnce = true;
}
void CDrawing::FillArea(float x, float y, int w, int h, int r, int g, int b, int a, int rr, int gg, int bb, int aa)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLE_STRIP);
	glColor4ub(rr,gg,bb,aa);
	glVertex2f(x,y + h);
	glVertex2f(x + w,y + h);
	glColor4ub(r,g,b,a);
	glVertex2f(x,y);
	glVertex2f(x + w,y);
	glEnd();
	
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}
void CDrawing::FillRGBA(GLfloat x, GLfloat y, int w, int h, UCHAR r, UCHAR g, UCHAR b, UCHAR a)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(r,g,b,a);
	  glBegin(GL_QUADS);
	    glVertex2f(x,y);
		glVertex2f(x+w,y);
		glVertex2f(x+w,y+h);
		glVertex2f(x,y+h);
	  glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}
void CDrawing::FillRGBA(GLfloat x, GLfloat y, int w, int h, ColorEntry* clr)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(clr->r,clr->g,clr->b,clr->a);
	  glBegin(GL_QUADS);
	    glVertex2f(x,y);
		glVertex2f(x+w,y);
		glVertex2f(x+w,y+h);
		glVertex2f(x,y+h);
	  glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void colorBorder(int x, int y, int w, int h, int r, int g, int b, int a)
{
	oglSubtractive = true;
 FillRGBA(x - 1, y - 1, w + 2, 1, r, g, b, a); //top
 FillRGBA(x - 1, y, 1, h - 1, r, g, b, a);	//left
 FillRGBA(x + w, y, 1, h - 1, r, g, b, a);	//right
 FillRGBA(x - 1, y + h - 1, w + 2, 1, r, g, b, a); //bottom
	oglSubtractive = false;
}

void CDrawing::DrawBox( int x, int y, int w, int h, int linewidth, int r, int g, int b, int a )
{

	FillRGBA( x,					y,					w,					linewidth,		r, g, b, a ); 
	FillRGBA( x + w - linewidth,	y + linewidth,		linewidth,			h - linewidth,	r, g, b, a ); 
	FillRGBA( x,					y + linewidth,		linewidth,			h - linewidth,	r, g, b, a ); 
	FillRGBA( x + linewidth,		y + h - linewidth,	w - linewidth * 2,	linewidth,		r, g, b, a ); 
}

int CDrawing::iStringLen( const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start( va_alist, fmt );
	_vsnprintf( buf, sizeof( buf ), fmt, va_alist );
	va_end( va_alist );
	int iWidth, iHeight;
	pfnDrawConsoleStringLen( buf, &iWidth, &iHeight );
	return iWidth;
}

int CDrawing::iStringHeight( void )
{
	int iWidth, iHeight;
	pfnDrawConsoleStringLen( "F", &iWidth, &iHeight );
	return iHeight;
}

void CDrawing::DrawString( int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start( va_alist, fmt );
	_vsnprintf( buf, sizeof( buf ), fmt, va_alist );
	va_end( va_alist );
	pfnDrawSetTextColor( (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f );
	pfnDrawConsoleString( x, y, buf );
}

void CDrawing::DrawStringCenter( int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start( va_alist, fmt );
	_vsnprintf( buf, sizeof( buf ), fmt, va_alist );
	va_end( va_alist );
	int iWidth = iStringLen( "%s", buf );
	pfnDrawSetTextColor( (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f );
	pfnDrawConsoleString( x - iWidth / 2, y, buf );
}

void CDrawing::tintArea(int x,int y,int w,int h, ColorEntry* clr)
{
	oglSubtractive = true;
	FillRGBA(x,y,w,h,clr->r,clr->g,clr->b,clr->a);
	oglSubtractive = false;
}

void CDrawing::whiteBorder(int x,int y,int w, int h)
{
	oglSubtractive = true;
	FillRGBA( x-1, y-1, w+2, 1 ,255,255,255,254 );
	FillRGBA( x-1, y, 1, h-1 ,255,255,255,254 );
	FillRGBA( x+w, y, 1, h-1 ,255,255,255,254 );	
	FillRGBA( x-1, y+h-1, w+2, 1 ,255,255,255,254 ); 
	oglSubtractive = false;
}

void CDrawing::blackBorder(int x,int y,int w, int h)
{
	oglSubtractive = true;
	FillRGBA( x-1, y-1, w+2, 1 ,0,0,0,254 ); 
	FillRGBA( x-1, y, 1, h-1 ,0,0,0,254 );	
	FillRGBA( x+w, y, 1, h-1 ,0,0,0,254 );	
	FillRGBA( x-1, y+h-1, w+2, 1 ,0,0,0,254 ); 
	oglSubtractive = false;
}

void CDrawing::tintArea(int x,int y,int w,int h,int r,int g,int b,int a)
{
	oglSubtractive = true;
	FillRGBA(x,y,w,h,r,g,b,a);
	oglSubtractive = false;
}

void CDrawing::DrawConString (int x, int y, int r, int g, int b, const char* fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start(va_alist, fmt);
	_vsnprintf(buf, sizeof(buf), fmt, va_alist);
	va_end(va_alist);

	int length, height;

	y += 4;
 pfnDrawConsoleStringLen(buf, &length, &height);
 pfnDrawSetTextColor(r / 255.0f, g / 255.0f, b / 255.0f);
 DrawConsoleString(x, y, buf);
}

int DrawLen(char *fmt)
{
	int len=0;
	for ( char * p = fmt; *p; p++ ) 
		len+=g_Screen.charWidths[*p]; 
	return len;
}

void CDrawing::DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	

		pfnDrawSetTextColor((float)r/255.0,(float)g/255.0,(float)b/255.0);
		pfnDrawConsoleString(x,y,buf);
	
}

void CDrawing::DrawHudStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	
		int length, height;
		pfnDrawConsoleStringLen( buf, &length, &height );
		x = x - length/2;
		pfnDrawSetTextColor((float)r/255.0,(float)g/255.0,(float)b/255.0);
		pfnDrawConsoleString(x,y,buf);

	
	
}

void CDrawing::PrintWithFont(int x, int y, int r, int g, int b, const char *fmt, ... )
{

}

void CDrawing::DrawWindow(int x, int y, int w, int h, const char* title, ...)
{
	va_list va_alist;
	char buf[256];

	va_start(va_alist, title);
	_vsnprintf(buf, sizeof(buf), title, va_alist);
	va_end(va_alist);

	ColorEntry* color = colorList.get(8); 
	ColorEntry* color1 = colorList.get(9); 

	oglSubtractive = true;
	FillRGBA(x, y - 16, w, 1, 255, 255, 255, 150); 
	FillRGBA(x, y - 16, 1, 16, 255, 255, 255, 120);
	FillRGBA(x + w, y - 16, 1, 16 + 1, 20, 20, 50, 200);	

	FillArea(x + 1, y - 15, w - 1, 16, color->r, color->g, color->b, color->a, color1->r, color1->g, color1->b, color1->a); // title box body 标题头色彩

	DrawConString(x + 38, y - 19, 255, 255, 255, buf); 

	FillRGBA(x, y, w, 1, 0, 0, 0, 255); 
	FillArea(x + 1, y + 1, w - 1, h - 1, color->r, color->g, color->b, color->a, color1->r, color1->g, color1->b, color1->a);

	oglSubtractive = false;
}

CDrawing g_Drawing;
