
#pragma once

struct menu_entrys
{
	char title[1000];
	int* value;
	int min;
	int max;
	int step;
	int* fvalue;
	int fmin;
	int fmax;
	int fstep;
};

class cMenu
{
private:
	int AddEntry(int n, char title[1000], int* value, int min, int max, int step);
	int menuSelect;
	int menuItems;
	menu_entrys menuEntry[1000];

public:

	void Init();
	void Draw();
	int KeyEvent(int keynum);
	int Active;
};

extern cMenu gMenu;