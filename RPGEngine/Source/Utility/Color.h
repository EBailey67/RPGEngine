#pragma once
#include <cstdint>
#include <SDL.h>

struct Color
{
	uint8_t r, g, b, a;

	Color() : r(0), g(0), b(0), a(255) {}
	Color(const uint8_t r_, const uint8_t g_, const uint8_t b_, const uint8_t a_ = 255) :
		r(r_), g(g_), b(b_), a(a_)
	{}

	Color(const SDL_Color& color) :
		r(color.r), g(color.g), b(color.b), a(color.a)
	{}

	operator SDL_Color() const { return {r, g, b, a};}

	static const Color AliceBlue;			// System-defined color ( R:240 G:248 B:255 ).
	static const Color AntiqueWhite;		// System-defined color ( R:250 G:235 B:215 ).
	static const Color Aqua;				// System-defined color ( R:0 G:255 B:255 ).
	static const Color Aquamarine;			// System-defined color ( R:127 G:255 B:212 ).
	static const Color Azure;				// System-defined color ( R:240 G:255 B:255 ).
	static const Color Beige;				// System-defined color ( R:245 G:245 B:220 ).
	static const Color Bisque;				// System-defined color ( R:255 G:228 B:196 ).
	static const Color Black;				// System-defined color ( R:0 G:0 B:0 ).
	static const Color BlanchedAlmond;		// System-defined color ( R:255 G:235 B:205 ).
	static const Color Blue;				// System-defined color ( R:0 G:0 B:255 ).
	static const Color BlueViolet;			// System-defined color ( R:138 G:43 B:226 ).
	static const Color Brown;				// System-defined color ( R:165 G:42 B:42 ).
	static const Color BurlyWood;			// System-defined color ( R:222 G:184 B:135 ).
	static const Color CadetBlue;			// System-defined color ( R:95 G:158 B:160 ).
	static const Color Chartreuse;			// System-defined color ( R:127 G:255 B:0 ).
	static const Color Chocolate;			// System-defined color ( R:210 G:105 B:30 ).
	static const Color Coral;				// System-defined color ( R:255 G:127 B:80 ).
	static const Color CornflowerBlue;		// System-defined color ( R:100 G:149 B:237 ).
	static const Color Cornsilk;			// System-defined color ( R:255 G:248 B:220 ).
	static const Color Crimson;				// System-defined color ( R:220 G:20 B:60 ).
	static const Color Cyan;				// System-defined color ( R:0 G:255 B:255 ).
	static const Color DarkBlue;			// System-defined color ( R:0 G:0 B:139 ).
	static const Color DarkCyan;			// System-defined color ( R:0 G:139 B:139 ).
	static const Color DarkGoldenrod;		// System-defined color ( R:184 G:134 B:11 ).
	static const Color DarkGray;			// System-defined color ( R:169 G:169 B:169 ).
	static const Color DarkGreen;			// System-defined color ( R:0 G:100 B:0 ).
	static const Color DarkKhaki;			// System-defined color ( R:189 G:183 B:107 ).
	static const Color DarkMagenta;			// System-defined color ( R:139 G:0 B:139 ).
	static const Color DarkOliveGreen;		// System-defined color ( R:85 G:107 B:47 ).
	static const Color DarkOrange;			// System-defined color ( R:255 G:140 B:0 ).
	static const Color DarkOrchid;			// System-defined color ( R:153 G:50 B:204 ).
	static const Color DarkRed;				// System-defined color ( R:139 G:0 B:0 ).
	static const Color DarkSalmon;			// System-defined color ( R:233 G:150 B:122 ).
	static const Color DarkSeaGreen;		// System-defined color ( R:143 G:188 B:139 ).
	static const Color DarkSlateBlue;		// System-defined color ( R:72 G:61 B:139 ).
	static const Color DarkSlateGray;		// System-defined color ( R:47 G:79 B:79 ).
	static const Color DarkTurquoise;		// System-defined color ( R:0 G:206 B:209 ).
	static const Color DarkViolet;			// System-defined color ( R:148 G:0 B:211 ).
	static const Color DeepPink;			// System-defined color ( R:255 G:20 B:147 ).
	static const Color DeepSkyBlue;			// System-defined color ( R:0 G:191 B:255 ).
	static const Color DimGray;				// System-defined color ( R:105 G:105 B:105 ).
	static const Color DodgerBlue;			// System-defined color ( R:30 G:144 B:255 ).
	static const Color Firebrick;			// System-defined color ( R:178 G:34 B:34 ).
	static const Color FloralWhite;			// System-defined color ( R:255 G:250 B:240 ).
	static const Color ForestGreen;			// System-defined color ( R:34 G:139 B:34 ).
	static const Color Fuchsia;				// System-defined color ( R:255 G:0 B:255 ).
	static const Color Gainsboro;			// System-defined color ( R:220 G:220 B:220 ).
	static const Color GhostWhite;			// System-defined color ( R:248 G:248 B:255 ).
	static const Color Gold;				// System-defined color ( R:255 G:215 B:0 ).
	static const Color Goldenrod;			// System-defined color ( R:218 G:165 B:32 ).
	static const Color Gray;				// System-defined color ( R:128 G:128 B:128 ).
	static const Color Green;				// System-defined color ( R:0 G:128 B:0 ).
	static const Color GreenYellow;			// System-defined color ( R:173 G:255 B:47 ).
	static const Color Honeydew;			// System-defined color ( R:240 G:255 B:240 ).
	static const Color HotPink;				// System-defined color ( R:255 G:105 B:180 ).
	static const Color IndianRed;			// System-defined color ( R:205 G:92 B:92 ).
	static const Color Indigo;				// System-defined color ( R:75 G:0 B:130 ).
	static const Color Ivory;				// System-defined color ( R:255 G:255 B:240 ).
	static const Color Khaki;				// System-defined color ( R:240 G:230 B:140 ).
	static const Color Lavender;			// System-defined color ( R:230 G:230 B:250 ).
	static const Color LavenderBlush;		// System-defined color ( R:255 G:240 B:245 ).
	static const Color LawnGreen;			// System-defined color ( R:124 G:252 B:0 ).
	static const Color LemonChiffon;		// System-defined color ( R:255 G:250 B:205 ).
	static const Color LightBlue;			// System-defined color ( R:173 G:216 B:230 ).
	static const Color LightCoral;			// System-defined color ( R:240 G:128 B:128 ).
	static const Color LightCyan;			// System-defined color ( R:224 G:255 B:255 ).
	static const Color LightGoldenrodYellow;// System-defined color ( R:250 G:250 B:210 ).
	static const Color LightGray;			// System-defined color ( R:211 G:211 B:211 ).
	static const Color LightGreen;			// System-defined color ( R:144 G:238 B:144 ).
	static const Color LightPink;			// System-defined color ( R:255 G:182 B:193 ).
	static const Color LightSalmon;			// System-defined color ( R:255 G:160 B:122 ).
	static const Color LightSeaGreen;		// System-defined color ( R:32 G:178 B:170 ).
	static const Color LightSkyBlue;		// System-defined color ( R:135 G:206 B:250 ).
	static const Color LightSlateGray;		// System-defined color ( R:119 G:136 B:153 ).
	static const Color LightSteelBlue;		// System-defined color ( R:176 G:196 B:222 ).
	static const Color LightYellow;			// System-defined color ( R:255 G:255 B:224 ).
	static const Color Lime;				// System-defined color ( R:0 G:255 B:0 ).
	static const Color LimeGreen;			// System-defined color ( R:50 G:205 B:50 ).
	static const Color Linen;				// System-defined color ( R:250 G:240 B:230 ).
	static const Color Magenta;				// System-defined color ( R:255 G:0 B:255 ).
	static const Color Maroon;				// System-defined color ( R:128 G:0 B:0 ).
	static const Color MediumAquamarine;	// System-defined color ( R:102 G:205 B:170 ).
	static const Color MediumBlue;			// System-defined color ( R:0 G:0 B:205 ).
	static const Color MediumOrchid;		// System-defined color ( R:186 G:85 B:211 ).
	static const Color MediumPurple;		// System-defined color ( R:147 G:112 B:219 ).
	static const Color MediumSeaGreen;		// System-defined color ( R:60 G:179 B:113 ).
	static const Color MediumSlateBlue;		// System-defined color ( R:123 G:104 B:238 ).
	static const Color MediumSpringGreen;	// System-defined color ( R:0 G:250 B:154 ).
	static const Color MediumTurquoise;		// System-defined color ( R:72 G:209 B:204 ).
	static const Color MediumVioletRed;		// System-defined color ( R:199 G:21 B:133 ).
	static const Color MidnightBlue;		// System-defined color ( R:25 G:25 B:112 ).
	static const Color MintCream;			// System-defined color ( R:245 G:255 B:250 ).
	static const Color MistyRose;			// System-defined color ( R:255 G:228 B:225 ).
	static const Color Moccasin;			// System-defined color ( R:255 G:228 B:181 ).
	static const Color NavajoWhite;			// System-defined color ( R:255 G:222 B:173 ).
	static const Color Navy;				// System-defined color ( R:0 G:0 B:128 ).
	static const Color OldLace;				// System-defined color ( R:253 G:245 B:230 ).
	static const Color Olive;				// System-defined color ( R:128 G:128 B:0 ).
	static const Color OliveDrab;			// System-defined color ( R:107 G:142 B:35 ).
	static const Color Orange;				// System-defined color ( R:255 G:165 B:0 ).
	static const Color OrangeRed;			// System-defined color ( R:255 G:69 B:0 ).
	static const Color Orchid;				// System-defined color ( R:218 G:112 B:214 ).
	static const Color PaleGoldenrod;		// System-defined color ( R:238 G:232 B:170 ).
	static const Color PaleGreen;			// System-defined color ( R:152 G:251 B:152 ).
	static const Color PaleTurquoise;		// System-defined color ( R:175 G:238 B:238 ).
	static const Color PaleVioletRed;		// System-defined color ( R:219 G:112 B:147 ).
	static const Color PapayaWhip;			// System-defined color ( R:255 G:239 B:213 ).
	static const Color PeachPuff;			// System-defined color ( R:255 G:218 B:185 ).
	static const Color Peru;				// System-defined color ( R:205 G:133 B:63 ).
	static const Color Pink;				// System-defined color ( R:255 G:192 B:203 ).
	static const Color Plum;				// System-defined color ( R:221 G:160 B:221 ).
	static const Color PowderBlue;			// System-defined color ( R:176 G:224 B:230 ).
	static const Color Purple;				// System-defined color ( R:128 G:0 B:128 ).
	static const Color Red;					// System-defined color ( R:255 G:0 B:0 ).
	static const Color RosyBrown;			// System-defined color ( R:188 G:143 B:143 ).
	static const Color RoyalBlue;			// System-defined color ( R:65 G:105 B:225 ).
	static const Color SaddleBrown;			// System-defined color ( R:139 G:69 B:19 ).
	static const Color Salmon;				// System-defined color ( R:250 G:128 B:114 ).
	static const Color SandyBrown;			// System-defined color ( R:244 G:164 B:96 ).
	static const Color SeaGreen;			// System-defined color ( R:46 G:139 B:87 ).
	static const Color SeaShell;			// System-defined color ( R:255 G:245 B:238 ).
	static const Color Sienna;				// System-defined color ( R:160 G:82 B:45 ).
	static const Color Silver;				// System-defined color ( R:192 G:192 B:192 ).
	static const Color SkyBlue;				// System-defined color ( R:135 G:206 B:235 ).
	static const Color SlateBlue;			// System-defined color ( R:106 G:90 B:205 ).
	static const Color SlateGray;			// System-defined color ( R:112 G:128 B:144 ).
	static const Color Snow;				// System-defined color ( R:255 G:250 B:250 ).
	static const Color SpringGreen;			// System-defined color ( R:0 G:255 B:127 ).
	static const Color SteelBlue;			// System-defined color ( R:70 G:130 B:180 ).
	static const Color Tan;					// System-defined color ( R:210 G:180 B:140 ).
	static const Color Teal;				// System-defined color ( R:0 G:128 B:128 ).
	static const Color Tomato;				// System-defined color ( R:255 G:99 B:71 ).
	static const Color Turquoise;			// System-defined color ( R:64 G:224 B:208 ).
	static const Color Wheat;				// System-defined color ( R:245 G:222 B:179 ).
	static const Color White;				// System-defined color ( R:255 G:255 B:255 ).
	static const Color WhiteSmoke;			// System-defined color ( R:245 G:245 B:245 ).
	static const Color YellowGreen;			// System-defined color ( R:154 G:205 B:50 ).

	
};
