/* Copyright (c) 2006-2010, Universities Space Research Association (USRA).
*  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Universities Space Research Association nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _H_locomotion_system
#define _H_locomotion_system
#include "Debug.hh"
#include <LabelStr.hh>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "subscriber.hh"
#include "locomotion/server/locomotionMsgLib.h"
#include "locomotion/server/locomotionPosterLib.h"

#ifndef OK
#define OK 0
#endif
#ifndef ERROR
#define ERROR (-1)
#endif

#ifndef POS_DELIMITER
#define POS_DELIMITER "_"
#endif


// Init the module and connect to the GenoM module.
bool init_locomotion(CLIENT_ID *cliente);
bool end_locomotion(CLIENT_ID cliente);
int moveRef(float x, float y, float z, CLIENT_ID cliente);
int moveTo(float x, float y, float z, CLIENT_ID cliente);
int rotateRef(int axis, double angle, CLIENT_ID cliente);
bool setSpeed(double speed, CLIENT_ID cliente);
char* getNowposition(CLIENT_ID cliente);
char* getBackposition(const char *pos, float radius);
float getNowCoord(int coord, CLIENT_ID cliente);
bool getXYposition(const char* pos, float *px, float *py);

#endif
