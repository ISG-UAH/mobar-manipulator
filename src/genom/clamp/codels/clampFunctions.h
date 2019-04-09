/*
 * clampFunctions.h
 *
 *  Created on: 09/07/2014
 *      Author: goac
 */

#ifndef CLAMPFUNCTIONS_H_
#define CLAMPFUNCTIONS_H_

#include <portLib.h>
#include <string.h>
#include "server/clampHeader.h"
#include <controller.h>

#define MAX_APERTURE 1.2
#define MIN_APERTURE 0.45

CLIENT* createClient(void);
int setGrip(GRIP_STATE grip);
int checkBarrier(GRIP_STATE grip);
COLORRGB getColor(void);


#endif /* CLAMPFUNCTIONS_H_ */
