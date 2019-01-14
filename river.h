
//{{BLOCK(river)

//======================================================================
//
//	river, 512x512@4, 
//	+ palette 256 entries, not compressed
//	+ 217 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 6944 + 8192 = 15648
//
//	Time-stamp: 2018-11-24, 18:58:24
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_RIVER_H
#define GRIT_RIVER_H

#define riverTilesLen 6944
extern const unsigned short riverTiles[3472];

#define riverMapLen 8192
extern const unsigned short riverMap[4096];

#define riverPalLen 512
extern const unsigned short riverPal[256];

#endif // GRIT_RIVER_H

//}}BLOCK(river)
