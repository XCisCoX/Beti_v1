#ifndef _X_H_
#define _X_H_
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "client.h"
#include "color.h"
class CDrawing
{
public:
	void InitVisuals(void);
	void FillArea(float x, float y, int w, int h, int r, int g, int b, int a, int rr, int gg, int bb, int aa);
	void FillRGBA(GLfloat x, GLfloat y, int w, int h, UCHAR r, UCHAR g, UCHAR b, UCHAR a);
	void FillRGBA(GLfloat x, GLfloat y, int w, int h, ColorEntry* clr);
	void DrawBox( int x, int y, int w, int h, int linewidth, int r, int g, int b, int a );
	int iStringLen( const char *fmt, ... );
	int iStringHeight( void );
	void DrawString( int x, int y, int r, int g, int b, const char *fmt, ... );
	void DrawStringCenter( int x, int y, int r, int g, int b, const char *fmt, ... );
	void DrawConString (int x, int y, int r, int g, int b, const char* fmt, ... );
	void DrawWindow(int x, int y, int w, int h, const char* title, ...);
	void tintArea(int x,int y,int w,int h, ColorEntry* clr);
	void tintArea(int x,int y,int w,int h,int r,int g,int b,int a);
	void blackBorder(int x,int y,int w, int h);
	void whiteBorder(int x,int y,int w, int h);
	void DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... );
	void DrawHudStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... );
	void PrintWithFont(int x, int y, int r, int g, int b, const char *fmt, ... );
private:
};
void colorBorder(int x, int y, int w, int h, int r, int g, int b, int a);
extern CDrawing g_Drawing;
#endif