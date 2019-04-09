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
* THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND Value EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR Value DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON Value THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN Value WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// NOTE: this file contains a lot of static functions, which
// we might later choose to incorporate as class members.

#include "LocomotionAdapter.hh"

using PLEXIL::LabelStr;
using PLEXIL::State;

using std::cout;
using std::cerr;
using std::endl;
using std::list;
using std::map;
using std::string;
using std::vector;
using std::copy;


///////////////////////////// Conveniences //////////////////////////////////

// A preamble for error messages.
static string error = "Error in LocomotionAdapter: ";

// A prettier name for the "unknown" value.
static Value Unknown = PLEXIL::UNKNOWN();

// A localized handle on the adapter, which allows a
// decoupling between the sample system and adapter.
static LocomotionAdapter * Adapter;

// An empty argument vector.
static vector<Value> EmptyArgs;


///////////////////////////// State support //////////////////////////////////

// Queries the system for the value of a state and its arguments.
//
static Value fetch (const string& state_name, const vector<Value>& args)
{
  debugMsg("LocomotionAdapter:fetch",
           "Fetch called on " << state_name << " with " << args.size() << " args");
  Value retval;

  return retval;
}


// The 'receive' functions are the subscribers for system state updates.  They
// receive the name of the state whose value has changed in the system.  Then
// they propagate the state's new value to the executive.

static void propagate (const State& state, const vector<Value>& value)
{
  Adapter->propagateValueChange (state, value);
}

static void receive (const string& state_name, int val)
{
  propagate (State (LabelStr (state_name), EmptyArgs),
             vector<Value> (1, val));
}

static void receive (const string& state_name, float val)
{
  propagate (State (LabelStr (state_name), EmptyArgs),
             vector<Value> (1, val));
}

static void receive (const string& state_name, const string& val)
{
  propagate (State (LabelStr (state_name), EmptyArgs),
             vector<Value> (1, val));
}

static void receive (const string& state_name, bool val, const string& arg)
{
  State state (state_name, vector<Value> (1, arg));
  propagate (state, vector<Value> (1, val));
}

static void receive (const string& state_name, bool val, int arg1, int arg2)
{
  vector<Value> vec;
  vec.push_back (arg1);
  vec.push_back (arg2);
  State state (state_name, vec);
  propagate (state, vector<Value> (1, val));
}


///////////////////////////// Member functions //////////////////////////////////


LocomotionAdapter::LocomotionAdapter(PLEXIL::AdapterExecInterface& execInterface,
							 const pugi::xml_node& configXml) :
    InterfaceAdapter(execInterface, configXml)
{
  debugMsg("LocomotionAdapter", " created.");
}

bool LocomotionAdapter::initialize()
{
  m_execInterface.defaultRegisterAdapter(getId());
  Adapter = this;
  clientInitialized = false;
  debugMsg("LocomotionAdapter", " initialized.");
  return true;
}

bool LocomotionAdapter::start()
{
  debugMsg("LocomotionAdapter", " started.");
  return true;
}

bool LocomotionAdapter::stop()
{
  return true;
}

bool LocomotionAdapter::reset()
{
  debugMsg("LocomotionAdapter", " reset.");
  return true;
}

bool LocomotionAdapter::shutdown()
{
  debugMsg("LocomotionAdapter", " shut down.");
  return true;

}


// Sends a command (as invoked in a Plexil command node) to the system and sends
// the status, and return value if applicable, back to the executive.
//
void LocomotionAdapter::executeCommand(const CommandId& cmd)
{
  const LabelStr& command_name = cmd->getName();
  string name = command_name.toString();
  debugMsg("LocomotionAdapter", "Received executeCommand for " << name);

  Value retval = Unknown;
  vector<Value> argv(10);
  const vector<Value>& args = cmd->getArgValues();
  copy (args.begin(), args.end(), argv.begin());

  float px, py, pz;

  if(clientInitialized == false)
     {
       clientInitialized = init_locomotion(&client);
       if(clientInitialized == false){
      	 debugMsg("LocomotionAdapter", " can not start.");
       }
  }
  if (clientInitialized == true){
	  debugMsg("LocomotionAdapter", "Received executeCommand " << name);

	  if(name == "moveRef"){
		  retval = getXYposition(argv[0].c_str(), &px, &py);
		  if(retval == true){
			  pz = argv[1];
			  retval = moveRef(px, py, pz, client);
		  }

	  }else if(name == "moveTo"){
		  retval = getXYposition(argv[0].c_str(), &px, &py);
		  if(retval == true){
			  pz = argv[1];
			  retval = moveTo(px, py, pz, client);
		  }
	  }else if(name == "rotateRef"){
		  retval = rotateRef(argv[0], argv[1], client);
	  }else if(name == "setSpeed"){
		  retval = setSpeed(argv[0], client);
	  }else if(name == "getNowposition"){
	  	  retval = getNowposition(client);

	  }else if(name == "getBackposition"){
	  	  retval = getBackposition(argv[0].c_str(), argv[1]);

	  }else if(name == "getNowX"){
	  	  retval = getNowCoord(0,client);
	  }else if(name == "getNowY"){
	  	  retval = getNowCoord(1,client);
	  }else if(name == "getNowZ"){
	  	  retval = getNowCoord(2,client);
	  }

  }

  // This sends a command handle back to the executive.
    m_execInterface.handleValueChange
    (cmd->getAck(), CommandHandleVariable::COMMAND_SENT_TO_SYSTEM());
  // This sends the command's return value (if expected) to the executive.
  if (cmd->getDest() != ExpressionId::noId()) {
    m_execInterface.handleValueChange (cmd->getDest(), retval);
  }

  m_execInterface.notifyOfExternalEvent();
}


Value LocomotionAdapter::lookupNow (const State& state)
{
  // This is the name of the state as given in the plan's LookupNow
  const LabelStr& name = state.first;
  const vector<Value>& args = state.second;
  return fetch(name.toString(), args);
}


void LocomotionAdapter::subscribe(const State& state)
{
  LabelStr nameLabel = LabelStr (state.first);
  debugMsg("LocomotionAdapter:subscribe", " processing state "
           << nameLabel.toString());
  m_subscribedStates.insert(state);
}


void LocomotionAdapter::unsubscribe (const State& state)
{
  LabelStr nameLabel = LabelStr (state.first);
  debugMsg("LocomotionAdapter:subscribe", " from state "
           << nameLabel.toString());
  m_subscribedStates.erase(state);
}

// Does nothing.
void LocomotionAdapter::setThresholds (const State& state, double hi, double lo)
{
}


void LocomotionAdapter::propagateValueChange (const State& state,
                                          const vector<Value>& vals) const
{
  if (!isStateSubscribed(state))
	return; 
  m_execInterface.handleValueChange (state, vals.front());
  m_execInterface.notifyOfExternalEvent();
}


bool LocomotionAdapter::isStateSubscribed(const State& state) const
{
  return m_subscribedStates.find(state) != m_subscribedStates.end();
}

// Necessary boilerplate
extern "C" {
  void initLocomotionAdapter() {
    REGISTER_ADAPTER(LocomotionAdapter, "LocomotionAdapter");
  }
}
