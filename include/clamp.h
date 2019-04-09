/*
 * clamp.h
 *
 *  	Created on: 04/07/2014
 *      Author: Juan Alejandro Mora Prieto
 */
#ifndef CLAMP_H
#define CLAMP_H

#include <stdio.h>
#include <strings.h>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include "controller.h"
//#include "clamp/clampStruct.h"

#define NUMCOLORS 4

using namespace std;


int getColor(int rgb[3], COLORRGB &color);

int colorMatch(COLORRGB &color);
int rgbToHsv(COLORRGB color, double hsv[3]);
double threeway_max(double a, double b, double c);
double threeway_min(double a, double b, double c);
void colorList(COLORRGB &color, const int index);



#endif
