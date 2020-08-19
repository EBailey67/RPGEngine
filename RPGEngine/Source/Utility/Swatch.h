#pragma once
#include <SDL.h>

#include "../Terminal/char.hpp"


class Swatch
{
public:
	// http://paletton.com/#uid=73d0u0k5qgb2NnT41jT74c8bJ8X
 
    const Term::Color PrimaryLightest { 110, 121, 119 };
    const Term::Color PrimaryLighter { 88, 100, 98 };
    const Term::Color Primary { 68, 82, 79 };
    const Term::Color PrimaryDarker { 48, 61, 59 };
    const Term::Color PrimaryDarkest { 29, 45, 42 };
 
    const Term::Color SecondaryLightest { 116, 120, 126 };
    const Term::Color SecondaryLighter { 93, 97, 105 };
    const Term::Color Secondary { 72, 77, 85 };
    const Term::Color SecondaryDarker { 51, 56, 64 };
    const Term::Color SecondaryDarkest { 31, 38, 47 };
 
    const Term::Color AlternateLightest { 190, 184, 174 };
    const Term::Color AlternateLighter { 158, 151, 138 };
    const Term::Color Alternate { 129, 121, 107 };
    const Term::Color AlternateDarker { 97, 89, 75 };
    const Term::Color AlternateDarkest { 71, 62, 45 };
 
    const Term::Color ComplimentLightest { 190, 180, 174 };
    const Term::Color ComplimentLighter { 158, 147, 138 };
    const Term::Color Compliment { 129, 116, 107 };
    const Term::Color ComplimentDarker { 97, 84, 75 };
    const Term::Color ComplimentDarkest { 71, 56, 45 };
 
    // http://pixeljoint.com/forum/forum_posts.asp?TID=12795
 
    const Term::Color DbDark { 20, 12, 28 };
    const Term::Color DbOldBlood { 68, 36, 52 };
    const Term::Color DbDeepWater { 48, 52, 109 };
    const Term::Color DbOldStone { 78, 74, 78 };
    const Term::Color DbWood { 133, 76, 48 };
    const Term::Color DbVegetation { 52, 101, 36 };
    const Term::Color DbBlood { 208, 70, 72 };
    const Term::Color DbStone { 117, 113, 97 };
    const Term::Color DbWater { 89, 125, 206 };
    const Term::Color DbBrightWood { 210, 125, 44 };
    const Term::Color DbMetal { 133, 149, 161 };
    const Term::Color DbGrass { 109, 170, 44 };
    const Term::Color DbSkin { 210, 170, 153 };
    const Term::Color DbSky { 109, 194, 202 };
    const Term::Color DbSun { 218, 212, 94 };
    const Term::Color DbLight { 222, 238, 214 };
};