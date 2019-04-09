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

#include "ClampAdapter.hh"

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
static string error = "Error in ClampAdapter: ";

// A prettier name for the "unknown" value.
static Value Unknown = PLEXIL::UNKNOWN();

// A localized handle on the adapter, which allows a
// decoupling between the sample system and adapter.
static ClampAdapter * Adapter;

// An empty argument vector.
static vector<Value> EmptyArgs;


///////////////////////////// State support //////////////////////////////////

// Queries the system for the value of a state and its arguments.
//
static Value fetch (const string& state_name, const vector<Value>& args)
{
  debugMsg("ClampAdapter:fetch",
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


ClampAdapter::ClampAdapter(PLEXIL::AdapterExecInterface& execInterface,
							 const pugi::xml_node& configXml) :
    InterfaceAdapter(execInterface, configXml)
{
  debugMsg("ClampAdapter", " created.");
}

bool ClampAdapter::initialize()
{
  m_execInterface.defaultRegisterAdapter(getId());
  Adapter = this;
  clientInitialized = false;
  debugMsg("ClampAdapter", " initialized.");
  return true;
}

bool ClampAdapter::start()
{
  debugMsg("ClampAdapter", " started.");
  return true;
}

bool ClampAdapter::stop()
{
  debugMsg("ClampAdapter", " stopped.");
  return true;
}

bool ClampAdapter::reset()
{
  debugMsg("ClampAdapter", " reset.");
  return true;
}

bool ClampAdapter::shutdown()
{
  debugMsg("ClampAdapter", " shut down.");
  return true;
}


// Sends a command (as invoked in a Plexil command node) to the system and sends
// the status, and return value if applicable, back to the executive.
//
void ClampAdapter::executeCommand(const CommandId& cmd)
{
  const LabelStr& command_name = cmd->getName();
  string name = command_name.toString();
  debugMsg("ClampAdapter", "Received executeCommand for " << name);

  Value retval = Unknown;
  vector<Value> argv(10);
  const vector<Value>& args = cmd->getArgValues();
  copy (args.begin(), args.end(), argv.begin());

  if(clientInitialized == false)
   {
	 clientInitialized = init_clamp(&client);
	 if(clientInitialized == false){
		 debugMsg("ClampAdapter", " can not start.");
	 }
   }if (clientInitialized == true){
  	  debugMsg("ClampAdapter", "Received executeCommand " << name);

  	  if(name == "openClamp"){
  		  retval = openClamp(client);
  	  }else if(name == "closeClamp"){
  		  retval = closeClamp(client);
  	  }else if(name == "checkBarrier"){
  		  retval = checkBarrier(client);
  		debugMsg("ClampAdapter", "Barrier:  " << retval);
  	  }else if(name == "getColor"){
  		  retval = getColor(client);
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


Value ClampAdapter::lookupNow (const State& state)
{
  // This is the name of the state as given in the plan's LookupNow
  const LabelStr& name = state.first;
  const vector<Value>& args = state.second;
  return fetch(name.toString(), args);
}


void ClampAdapter::subscribe(const State& state)
{
  LabelStr nameLabel = LabelStr (state.first);
  debugMsg("ClampAdapter:subscribe", " processing state "
           << nameLabel.toString());
  m_subscribedStates.insert(state);
}


void ClampAdapter::unsubscribe (const State& state)
{
  LabelStr nameLabel = LabelStr (state.first);
  debugMsg("ClampAdapter:subscribe", " from state "
           << nameLabel.toString());
  m_subscribedStates.erase(state);
}

// Does nothing.
void ClampAdapter::setThresholds (const State& state, double hi, double lo)
{
}


void ClampAdapter::propagateValueChange (const State& state,
                                          const vector<Value>& vals) const
{
  if (!isStateSubscribed(state))
	return;
  m_execInterface.handleValueChange (state, vals.front());
  m_execInterface.notifyOfExternalEvent();
}


bool ClampAdapter::isStateSubscribed(const State& state) const
{
  return m_subscribedStates.find(state) != m_subscribedStates.end();
}

// Necessary boilerplate
extern "C" {
  void initClampAdapter() {
    REGISTER_ADAPTER(ClampAdapter, "ClampAdapter");
  }
}
