
//{{BLOCK(forest)

//======================================================================
//
//	forest, 512x512@4, 
//	+ palette 256 entries, not compressed
//	+ 893 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 28576 + 8192 = 37280
//
//	Time-stamp: 2018-11-26, 08:30:58
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FOREST_H
#define GRIT_FOREST_H

#define forestTilesLen 28576
extern const unsigned short forestTiles[14288];

#define forestMapLen 8192
extern const unsigned short forestMap[4096];

#define forestPalLen 512
extern const unsigned short forestPal[256];

#endif // GRIT_FOREST_H

//}}BLOCK(forest)
