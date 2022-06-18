#ifndef API_H
#define API_H
#include "color.h"
#include "client.h"
void UpdateMe(void);
bool bIsEntValid(cl_entity_s * ent,int index);
bool CalcScreen( float *pflOrigin, float *pflVecScreen );
bool bIsValidEnt(struct cl_entity_s *ent);
extern ColorEntry * PlayerColor(int ax);
extern void __cdecl add_log (const char *fmt, ...);
extern bool bPathFree( float *pflFrom, float *pflTo );
#endif