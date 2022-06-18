#include "cvar.h"
#include "client.h"
#include "menu.h"
#include "xorstr.h"
#include "drawing.h"

extern SCREENINFO g_Screen;
int cb2;
int cb1;

cMenu gMenu;
int cMenu::AddEntry(int n, char title[1000], int* value, int min, int max, int step)
{
	strcpy(menuEntry[n].title, title);
	menuEntry[n].value = value;
	menuEntry[n].min = min;
	menuEntry[n].max = max;
	menuEntry[n].step = step;
	return (n + 1);
}
void cMenu::Init()
{
	int i = 0;
	i = AddEntry(i, "+[ESP]", &cb1, 0, 1, 1);
	if (cb1)
	{
		i = AddEntry(i, "-Box", &esp_box, 0, 1, 1);
		i = AddEntry(i, "-Name", &name, 0, 1, 1);
		i = AddEntry(i, "-Entity", &esp_ent, 0, 1, 1);
		i = AddEntry(i, "-Box True", &boxvis, 0, 1, 1); //
		i = AddEntry(i, "-Reload", &reload, 0, 1, 1);
		i = AddEntry(i, "-Weapon", &weapon, 0, 1, 1);
		i = AddEntry(i, "-Distan", &distanesp, 0, 1, 1);
	}
	i = AddEntry(i, "+[FUN XD]", &cb2, 0, 1, 1);
	if (cb2)
	{
		i = AddEntry(i, "+DONT ACTIVE THIS!", &shit, 0, 1, 1);
		i = AddEntry(i, "+Shutdown!", &shutdownn, 0, 1, 1);
	}
	menuItems = i;
}

void cMenu::Draw()
{
	if (!Active) return;
	gMenu.Init();
	int w = 170, h = 0;
	int x = 30;
	int y = 200;
	for (int b = 0; b < menuItems; b++) h += 16; 
	g_Drawing.DrawWindow(x - 28, y + 3, w + 16, h, "BETI V1.0", 0);
	for (int i = 0; i < menuItems; i++)
	{
		ColorEntry * clr;
		if (i == menuSelect)
			clr = colorList.get(21);
		else
			clr = colorList.get(0);

		g_Drawing.DrawConString(x - 22, y + i * 16, clr->r, clr->g, clr->b, menuEntry[i].title);
		g_Drawing.DrawConString(x + w - 40, y + i * 16, 255, 255, 255, "%d.0", menuEntry[i].value[0]);

		if (i == menuSelect)
		{
			colorBorder(x - 27, y + (16 * i) + 4, w + 15, 16, 255, 0, 0, clr->a);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int cMenu::KeyEvent(int keynum)
{
	if (keynum == 128)
	{
		if (menuSelect > 0) menuSelect--;
		else menuSelect = menuItems - 1;
		return 0;
	}
	else if (keynum == 129)
	{
		if (menuSelect < menuItems - 1) menuSelect++;
		else menuSelect = 0;
		return 0;
	}
	else if (keynum == 130)
	{
		if (menuEntry[menuSelect].value)
		{
			menuEntry[menuSelect].value[0] -= menuEntry[menuSelect].step;
			if (menuEntry[menuSelect].value[0] < menuEntry[menuSelect].min)
				menuEntry[menuSelect].value[0] = menuEntry[menuSelect].max;
		}
		return 0;
	}
	else if (keynum == 131) 
	{
		if (menuEntry[menuSelect].value)
		{
			menuEntry[menuSelect].value[0] += menuEntry[menuSelect].step;
			if (menuEntry[menuSelect].value[0] > menuEntry[menuSelect].max)
				menuEntry[menuSelect].value[0] = menuEntry[menuSelect].min;
		}
		return 0;
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////