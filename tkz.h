#pragma once
#include <deque>
#include <Lmcons.h>
#include "stdafx.h"
#define SoundUpdate(a,b,origin) {b=a;a.vOrigin=origin; a.bValid=true; a.dwTime=GetTickCount();}
#define SoundClear(a) {a.vOrigin[0]=0; a.vOrigin[1]=0; a.vOrigin[2]=0; a.dwTime=0; a.bValid=false;}

#define DegToRad(Angle)((M_PI/180.0)*Angle)

#define	PITCH	0
#define	YAW		1
#define	ROLL	2

typedef void(*con_func)();

void APIENTRY Hooked_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *data);

class tkz_state
{
public:

	char *cPrefix, *cServerIP, *pSetinfo;

	DWORD dwVersion, dwServerIP, SpeedPtr, dwSpeedptr;

	bool bNoDraw, bGameActive;
	static bool bSnapshot, bScreenshot, bMakeScreen;

	int iUpdateLast, iUpdateCurrent;
	static int iSnapTimer, iScreenTimer, iReadPixels;

	double *SpeedNet;

	void AntiScreen()
	{
		if (bSnapshot)
		{
			iSnapTimer--;

			if (iSnapTimer <= 0)
			{
				if (real_snapshot)
					real_snapshot();

				iSnapTimer = 6;

				bSnapshot = false;
			}
		}

		if (bScreenshot)
		{
			iScreenTimer--;

			if (iScreenTimer <= 0)
			{
				if (real_screenshot)
					real_screenshot();

				iScreenTimer = 6;

				bScreenshot = false;
			}
		}

		bMakeScreen = (bSnapshot || bScreenshot);
	}

	static void snapshot_hooked(), screenshot_hooked();

	con_func real_screenshot, real_snapshot;
};
extern tkz_state Tkz;
