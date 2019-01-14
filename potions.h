
//{{BLOCK(potions)

//======================================================================
//
//	potions, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 379 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 12128 + 2048 = 14688
//
//	Time-stamp: 2018-11-16, 21:51:12
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_POTIONS_H
#define GRIT_POTIONS_H

#define potionsTilesLen 12128
extern const unsigned short potionsTiles[6064];

#define potionsMapLen 2048
extern const unsigned short potionsMap[1024];

#define potionsPalLen 512
extern const unsigned short potionsPal[256];

#endif // GRIT_POTIONS_H

//}}BLOCK(potions)
