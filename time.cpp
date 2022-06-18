#include <windows.h>
#include <mmsystem.h>
#include "time.h"
float  ClientTime::mapTime = 1;
double ClientTime::current = 1;
unsigned int ClientTime::current_ms = 1;
void ClientTime::reportMapTime(float mapTimeNew)
{
	mapTime = mapTimeNew;
	updateCurrentTime();
}
void ClientTime::updateCurrentTime()
{

	current_ms = timeGetTime();

	current = (double)current_ms / 1000.0;
}
