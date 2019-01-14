
//{{BLOCK(corridors)

//======================================================================
//
//	corridors, 512x512@4, 
//	+ palette 256 entries, not compressed
//	+ 445 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 14240 + 8192 = 22944
//
//	Time-stamp: 2018-12-01, 20:17:29
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CORRIDORS_H
#define GRIT_CORRIDORS_H

#define corridorsTilesLen 14240
extern const unsigned short corridorsTiles[7120];

#define corridorsMapLen 8192
extern const unsigned short corridorsMap[4096];

#define corridorsPalLen 512
extern const unsigned short corridorsPal[256];

#endif // GRIT_CORRIDORS_H

//}}BLOCK(corridors)
