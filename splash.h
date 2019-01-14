
//{{BLOCK(splash)

//======================================================================
//
//	splash, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 522 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 16704 + 2048 = 19264
//
//	Time-stamp: 2018-11-23, 23:31:51
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SPLASH_H
#define GRIT_SPLASH_H

#define splashTilesLen 16704
extern const unsigned short splashTiles[8352];

#define splashMapLen 2048
extern const unsigned short splashMap[1024];

#define splashPalLen 512
extern const unsigned short splashPal[256];

#endif // GRIT_SPLASH_H

//}}BLOCK(splash)
