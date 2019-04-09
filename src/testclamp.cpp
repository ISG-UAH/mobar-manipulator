/*
 * testclamp.cpp
 *
 *  Created on: 07/07/2014
 *      Author: goac
 */

#include "clamp.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	COLORRGB color;
	clamp pinza;
	int rgb[3];


	rgb[0] = 150; rgb[1] = 200; rgb[2] = 170;
	printf("Color 1: Verde - %d, %d, %d\n", rgb[0], rgb[1], rgb[2]);
	pinza.getColor(rgb, color);

	rgb[0] = 255; rgb[1] = 150; rgb[2] = 150;
	printf("Color 2: Rosa - %d, %d, %d\n", rgb[0], rgb[1], rgb[2]);
	pinza.getColor(rgb, color);


	return 0;
}


