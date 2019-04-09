#include "clamp.h"


int getColor(int rgb[3], COLORRGB &color){
	int ret, i;
	int extRgb[3];

	// Copy rgb in COLORRGB struct
	color.red	= rgb[0];
	color.green = rgb[1];
	color.blue 	= rgb[2];

	ret = colorMatch(color);

	return ret;
}

int colorMatch(COLORRGB &color)
{
	int ret, index, bestindex;
	double hsv[3], matchHsv[3], bestHsv[3];
	double tolerance=0, besttolerance=2;
	COLORRGB colormatch, bestmatch;

	ret = rgbToHsv(color, hsv);

	for(index = 0; index < NUMCOLORS; index++){
		colorList(colormatch, index);
		rgbToHsv(colormatch, matchHsv);

		// Euclidean distance
		tolerance = sqrt(pow(sin(hsv[0]*2*M_PI/360) - sin(matchHsv[0]*2*M_PI/360), 2) + pow(cos(hsv[0]*2*M_PI/360) - cos(matchHsv[0]*2*M_PI/360), 2));
		tolerance += fabs(hsv[1] - matchHsv[1]) + fabs(hsv[2] - matchHsv[2]);

		if(tolerance < besttolerance){
			besttolerance = tolerance;
			bestmatch = colormatch;
			bestindex = index;
			for (int i = 0; i<3; i++)
				bestHsv[i] = matchHsv[i];
		}
	}

	color.tolerance = besttolerance;
	color.idname = bestindex;
	sprintf(color.code, "#%2X%2X%2X", bestmatch.red, bestmatch.green, bestmatch.blue);
	return ret;
}


/**
* Converts an RGB color value to HSV. Conversion formula
* adapted from http://en.wikipedia.org/wiki/HSV_color_space.
* Assumes r, g, and b are contained in the set [0, 255] and
* returns h, s, and v in the set [0, 1].
*/

int rgbToHsv(COLORRGB color, double hsv[3]){
    double rd = (double) color.red/255;
    double gd = (double) color.green/255;
    double bd = (double) color.blue/255;
    double max = threeway_max(rd, gd, bd);
    double min = threeway_min(rd, gd, bd);
    double h, s, v = max;
    int ret=0;

    double d = max - min;
    s = max == 0 ? 0 : d / max;

    if (max == min) {
        h = 0; // achromatic
        ret = -1;
    } else {
        if (max == rd) {
            h = 60*(gd - bd) / d + (gd < bd ? 360 : 0);
        } else if (max == gd) {
            h = 60*(bd - rd) / d + 120;
        } else if (max == bd) {
            h = 60*(rd - gd) / d + 240;
        }
    }

    hsv[0] = h;
    hsv[1] = s;
    hsv[2] = v;

    return ret;
}

double threeway_max(double a, double b, double c) {
    return fmax(a, fmax(b, c));
}

double threeway_min(double a, double b, double c) {
    return fmin(a, fmin(b, c));
}

void colorList(COLORRGB &color, const int index){
//	char  *name[13] = {"BLACK", "BLUE", "BROWN", "CYAN", "DARK_GREEN", "GREY", "GREEN", "LIGHT_BLUE", "ORANGE", "PINK", "RED", "YELLOW", "WHITE"};
//	int red[13] = 	{0, 0, 	 166, 0,   47, 192, 0,   192, 255, 188, 255, 255, 255};
//	int green[13] = {0, 0, 	 42,  255, 79, 192, 255, 217, 127, 143, 0,   255, 255};
//	int blue[13] = 	{0, 255, 42,  255, 47, 192, 0,   217, 0,   143, 0,     0, 255};

//	char  *name[4] = {"BLUE", "GREEN", "PINK", "WHITE"};
	int red[NUMCOLORS] = 	     {0,      47,     220,     255};
	int green[NUMCOLORS] =      {0,     79,    75,     255};
	int blue[NUMCOLORS] =   	{255,     47,     180,     255};


	color.red = red[index];
	color.green = green[index];
	color.blue = blue[index];
//	color.name = name[index];
}
