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

#include "Locomotion_system.hh"

using std::cout;
using std::endl;
using std::string;
using std::pair;

// The system's state, as variables.
//
//CLIENT_ID cliente;

// Functions that provide access (read and write) for the simple parameter-less
// states.  These functions are very similar and thus conveniently defined with
// a macro.  Note that state readers are lookups in a Plexil plan; state writers
// are commands.

#define defAccessors(name, type) \
type get##name () \
{ \
  return name; \
} \
void set##name (const type & s) \
{ \
  if (s != name) { \
    name = s; \
    publish (#name, s); \
  } \
}

// Example:
//	static float Size = 5.1;
//	defAccessors(Size, float)



bool init_locomotion(CLIENT_ID *cliente)
{
	STATUS status;
	char MBOXname[50];

	srand (time(NULL));
	sprintf(MBOXname, "LocomotionAdapter%d", rand() % 10000);

	// Create the MailBox
	status = csMboxInit(MBOXname, 10, LOCOMOTION_CLIENT_MBOX_REPLY_SIZE);
    if(status != OK)
    {
        debugMsg("LocomotionAdapter", "Cannot connect to module.");
        return false;
    }else
    {
    	debugMsg("LocomotionAdapter", "Connecting successfully to module.");
    }

    // Initialize the client
	status = locomotionClientInit(cliente);
	if(status != OK)
	{
		debugMsg("LocomotionAdapter", "Cannot connect to the locomotion client module.");
		status = locomotionClientEnd(*cliente);
		return false;
	}else{
		debugMsg("LocomotionAdapter", "Locomotion client module conected successfully.");
		return true;
	}


}

bool end_locomotion(CLIENT_ID cliente)
{
    STATUS status;
	// Close connection with locomotion module
    if((status = locomotionClientEnd(cliente)) != OK)
    {
    	debugMsg("LocomotionAdapter", "Error Client close.");
        return false;
    }
    if ((status = csMboxEnd()) != OK)
    {
    	debugMsg("LocomotionAdapter", "Error Client close in csMboxEnd.");
		return false;
    }

    debugMsg("LocomotionAdapter", "Client closed successfully.");
    return true;
}

int moveRef(float x, float y, float z, CLIENT_ID cliente)
{
	int error;
	int activity, report;
	float movement[3];

	movement[0] = x;
	movement[1] = y;
	movement[2] = z;

	debugMsg("LocomotionAdapter", "MoveRef Variables: " << x << " " << y << " " << z);

	error = locomotionMoveRefRqstAndRcv(cliente, 10, movement, &activity, &report);

	if(report != OK){
		return ERROR;
	}

	return OK;
}

int moveTo(float x, float y, float z, CLIENT_ID cliente)
{
	int error;
	int activity, report;
	float movement[3];

	movement[0] = x;
	movement[1] = y;
	movement[2] = z;

	debugMsg("LocomotionAdapter", "MoveTo Variables: " << x << " " << y << " " << z);

	error = locomotionMoveToRqstAndRcv(cliente, 10, movement, &activity, &report);

	debugMsg("LocomotionAdapter", "Report: " << (report==OK ? "OK " : "Error ") << report << "Ret: " << error);

	if(report != OK){

		return ERROR;
	}

	return OK;
}

int rotateRef(int axis, double angle, CLIENT_ID cliente)
{
	int error;
	int activity, report;
	ANGLEROTATE in_angle;

	in_angle.axis = axis; in_angle.angle = angle;

	error = locomotionRotateRefRqstAndRcv(cliente, 10, &in_angle, &activity, &report);

	if(report != OK){
		return ERROR;
	}

	return OK;
}

bool setSpeed(double speed, CLIENT_ID cliente)
{
	int error;
	int activity, report;

	error = locomotionSetSpeedRqstAndRcv(cliente, 10, &speed, &activity, &report);

	if(report != OK){
		return false;
	}

	return true;
}

char* getNowposition(CLIENT_ID cliente)
{
	int error;
	int activity, report;
	POSITION position;
	//char ret[32];
	char *ret;

	error = locomotionStatuspositionPosterRead(&position);

	sprintf(ret, "P%.2f_%.2f", position.p[0], position.p[1]);

	debugMsg("LocomotionAdapter", "getNowPosition: " << ret);

	return ret;
}

float getNowCoord(int coord, CLIENT_ID cliente)
{
	int error;
	int activity, report;
	POSITION position;
	float retval;

	error = locomotionStatuspositionPosterRead(&position);

	retval = position.p[coord];

	return retval;
}

//void getXYposition(const PLEXIL::LabelStr square, float *px, float *py, bool *success)
bool getXYposition(const char* pos, float *px, float *py)
{
	char aux[16];
	char* temptok;

	strcpy(aux, pos);

	aux[0] = ' '; // Delete first 'P'

	temptok=strtok(aux,(char*)POS_DELIMITER); // First token, X position
	if(temptok == NULL)
		return false; // Malformed square
	else
		*px = atof(temptok);

	// Next token
	temptok=strtok(NULL,(char*)POS_DELIMITER); // Second token, Y position
	if(temptok == NULL)
		return false; // Malformed square
	else
		*py = atof(temptok);

	return true;
}

char* getBackposition(const char *pos, float radius)
{
	//char ret[32];
	char *ret;
	float px, py;
	float alpha, px1, py1;

	if(!getXYposition(pos, &px, &py))
		return "ERROR";

	alpha = atan2f(px, py);

	px1 = radius * sin(alpha);
	py1 = radius * cos(alpha);

	sprintf(ret, "P%.2f_%.2f", px1, py1);

	debugMsg("LocomotionAdapter", "getBackPosition: " << ret);

	return ret;

}

