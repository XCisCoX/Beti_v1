#include "client.h"
#include <string.h>
#define MAX_SPRITES 200
typedef struct 
{ 
    char szName[256]; 
    char szSprite[256]; 
    wrect_t rc; 
    int iRes; 
} spriteinfo_t; 
typedef struct 
{ 
    int iSpritesLoaded; 
    spriteinfo_t spriteinfo[MAX_SPRITES]; 
} spritedata_t;
extern spritedata_t spritedata;
int GetSpriteIndex(char *szName);
client_sprite_t* _cdecl hookpfnSPR_GetList(char *psz, int *piCount);