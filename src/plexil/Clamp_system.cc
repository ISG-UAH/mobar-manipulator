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

#include "Clamp_system.hh"

using std::cout;
using std::endl;
using std::string;
using std::pair;

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

bool init_clamp(CLIENT_ID *cliente)
{
	STATUS status;
	char MBOXname[50];

	srand (time(NULL));
	sprintf(MBOXname, "ClampAdapter%d", rand() % 10000);

	// Create the MailBox
	status = csMboxInit(MBOXname, 10, CLAMP_CLIENT_MBOX_REPLY_SIZE);
    if(status != OK)
    {
        debugMsg("ClampAdapter", "Cannot connect to module.");
        return false;
    }else
    {
    	debugMsg("ClampAdapter", "Connecting successfully to module.");
    }

    // Initialize the client
	status = clampClientInit(cliente);
	if(status != OK)
	{
		debugMsg("ClampAdapter", "Cannot connect to the clamp client module.");
		status = clampClientEnd(*cliente);
		return false;
	}

	return true;
}

bool end_clamp(CLIENT_ID cliente)
{
    // Close connection with locomotion module
    if(csClientEnd(cliente) != OK || csMboxEnd() != OK )
    {
    	debugMsg("ClampAdapter", "Error Client close.");
        return false;
    }
    debugMsg("ClampAdapter", "Client closed successfully.");
    return true;
}

bool openClamp(CLIENT_ID cliente)
{
	int error;
	int activity, report;

	error = clampOpenGripRqstAndRcv(cliente, 10, &activity, &report);

	if(report != OK){
		return false;
	}

	return true;
}

bool closeClamp(CLIENT_ID cliente)
{
	int error;
	int activity, report;

	error = clampCloseGripRqstAndRcv(cliente, 10, &activity, &report);

	if(report != OK){
		return false;
	}

	return true;
}

bool checkBarrier(CLIENT_ID cliente){
	int error;
	int activity, report;
	GRIP_STATE grip;

	error = clampCheckBarrierRqstAndRcv(cliente, 10, &activity, &report);

	if(report != OK){
		return false;
	}

	clampStatusgripPosterRead(&grip);

	if(grip.barrier < BARRIER_MIN_VALUE){ // Object in barrier
		return false;
	}

	return true;

}

string getColor(CLIENT_ID cliente){
	int error;
	int activity, report;
	//string name[13] = {"BLACK", "BLUE", "BROWN", "CYAN", "DARK_GREEN", "GREY", "GREEN", "LIGHT_BLUE", "ORANGE", "PINK", "RED", "YELLOW", "WHITE"};
	string name[4] = {"BLUE", "GREEN", "PINK", "WHITE"};
	COLORRGB object;

	error = clampGetObjectRqstAndRcv(cliente, 10, &activity, &report);

	if(report != OK){
		return false;
	}

	clampStatusobjectPosterRead(&object);

	debugMsg("ClampAdapter", "Color detected: " << name[object.idname]);

	return name[object.idname];
}


