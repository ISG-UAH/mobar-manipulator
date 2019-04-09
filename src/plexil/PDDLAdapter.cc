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

#include <iostream>
#include "AdapterFactory.hh"
#include "CoreExpressions.hh"
#include "AdapterExecInterface.hh"
#include "Expression.hh"
#include "Debug.hh"
#include "subscriber.hh"
#include "PDDLAdapter.hh"

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
static string error = "Error in PDDLAdapter: ";

// A prettier name for the "unknown" value.
static Value Unknown = PLEXIL::UNKNOWN();

// A localized handle on the adapter, which allows a
// decoupling between the sample system and adapter.
static PDDLAdapter * Adapter;

// An empty argument vector.
static vector<Value> EmptyArgs;


///////////////////////////// State support //////////////////////////////////

// Queries the system for the value of a state and its arguments.
//
static Value fetch (const string& state_name, const vector<Value>& args)
{
  debugMsg("PDDLAdapter:fetch",
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


PDDLAdapter::PDDLAdapter(PLEXIL::AdapterExecInterface& execInterface,
							 const pugi::xml_node& configXml) :
    InterfaceAdapter(execInterface, configXml)
{
  debugMsg("PDDLAdapter", " created.");
}

bool PDDLAdapter::initialize()
{
  m_execInterface.defaultRegisterAdapter(getId());
  Adapter = this;
  clientInitialized = false;
  debugMsg("PDDLAdapter", " initialized.");
  return true;
}

bool PDDLAdapter::start()
{
  debugMsg("PDDLAdapter", " started.");
  return true;
}

bool PDDLAdapter::stop()
{
  debugMsg("PDDLAdapter", " stopped.");
  return true;
}

bool PDDLAdapter::reset()
{
  debugMsg("PDDLAdapter", " reset.");
  return true;
}

bool PDDLAdapter::shutdown()
{
  debugMsg("PDDLAdapter", " shut down.");
  return true;
}


// Sends a command (as invoked in a Plexil command node) to the system and sends
// the status, and return value if applicable, back to the executive.
//
void PDDLAdapter::executeCommand(const CommandId& cmd)
{
  const LabelStr& command_name = cmd->getName();
  string name = command_name.toString();
  bool success = true;
  int nplanner;

  debugMsg("PDDLAdapter", "Received executeCommand for " << name);

  Value retval = Unknown;
  vector<Value> argv(10);
  const vector<Value>& args = cmd->getArgValues();
  copy (args.begin(), args.end(), argv.begin());

  if(name != "create_planner")
	  nplanner = static_cast<int> (argv[0]);
  else
	  nplanner = -1;

  if(name == "create_planner")
  {
	  PLEXIL::LabelStr ruta(argv[0]);
	  planificador.push_back(new Planner((char*)ruta.c_str()));
	  ultimaacc.push_back(new stpredicado);

	  int num = planificador.size()-1;

	  debugMsg("PDDLAdapter", "New planner ID#: "<< num);

	  retval = num;
  }else //Comprobar que numplanner esta dentro del rango
  {

      if(nplanner>=0 && nplanner<planificador.size())
      {
          if(name == "get_plan")
          {
              bool replan = static_cast<bool> (argv[1]);

              if(replan) debugMsg("PDDLAdapter", "REPLANNING request in planner #"<<nplanner);

              success=planificador[nplanner]->planificar(replan)==0;

              retval = success;
          }

          else if(name == "delete_goals")
          {
        	  debugMsg("PDDLAdapter", "Delete goals ");

        	  success = planificador[nplanner]->eliminar_goals();

        	  retval = success;
          }

		  else if(name == "delete_inits")
		  {
			  debugMsg("PDDLAdapter", "Delete inits ");

			  success = planificador[nplanner]->eliminar_inits();

			  retval = success;
		  }

          else if(name == "next_action")
          {
              int numobj;
              char *objs = new char[MAX_CAR];
 //             if(ultimaacc[nplanner]->objetos != NULL)
 //                 delete[] (char*)ultimaacc[nplanner]->objetos;
              planificador[nplanner]->sig_accion(ultimaacc[nplanner]->nombre,objs,&ultimaacc[nplanner]->valor);
              numobj=planificador[nplanner]->numero_objetos_proceso();
              ultimaacc[nplanner]->objetos = new char*[numobj];
              char *aux=strtok(objs,(char*)" ");
              int i=0;
              while(aux!=NULL)
              {
                  ultimaacc[nplanner]->objetos[i]=new char[strlen(aux)];
                  strcpy(ultimaacc[nplanner]->objetos[i],(char*)aux);
                  aux=strtok(NULL," ");
                  i++;
              }
              delete[] objs;
              debugMsg("PDDLAdapter", "Next action is "<<ultimaacc[nplanner]->nombre<< " <> ["<<planificador[nplanner]->conector_accion()<<"]");

              PLEXIL::LabelStr accion(planificador[nplanner]->conector_accion());
              retval = accion.getKey();
          }

          else if(name == "insert_predicate")
          {
              PLEXIL::LabelStr predicado(argv[1]);
              PLEXIL::LabelStr objetos(argv[2]);
              bool meta = static_cast<bool> (argv[3]);

              debugMsg("PDDLAdapter", "Insert "<<predicado.c_str()<<" "<<objetos.c_str()<<(meta?" as goal":""));

              success= planificador[nplanner]->insertar_pred((char*)predicado.c_str(),(char*)objetos.c_str(),meta) == 0;

              retval = success;
          }

          else if(name == "modify_predicate")
          {
              PLEXIL::LabelStr predicado(argv[1]);
              PLEXIL::LabelStr objetos(argv[2]);
              PLEXIL::LabelStr nuevos(argv[3]);

              debugMsg("PDDLAdapter", "Replace ["<<objetos.c_str()<<"] with ["<<nuevos.c_str()<<"] on ("<<predicado.c_str()<<")");

              success= planificador[nplanner]->reemplazar_pred((char*)predicado.c_str(),(char*)objetos.c_str(),(char*)nuevos.c_str()) == 0;

              retval = success;
          }

          else if(name == "remove_predicate")
          {
              PLEXIL::LabelStr predicado(argv[1]);
              PLEXIL::LabelStr objetos(argv[2]);

              debugMsg("PDDLAdapter", "Remove ("<<predicado.c_str()<<" "<<objetos.c_str()<<")");

              success= planificador[nplanner]->eliminar_predicado((char*)predicado.c_str(),(char*)objetos.c_str()) == 0;

              retval = success;
          }

          else if(name == "modify_function")
          {
              PLEXIL::LabelStr predicado(argv[1]);
              PLEXIL::LabelStr objetos(argv[2]);
              float valor = static_cast<float>(argv[3]);

              debugMsg("PDDLAdapter", "Modify function value for ("<<predicado.c_str()<<" "<<objetos.c_str()<<"), new value: "<<valor);

              success= planificador[nplanner]->reemplazar_funcion((char*)predicado.c_str(),(char*)objetos.c_str(),valor) == 0;

              retval = success;
          }

          else if(name == "predicate_object")
          {
              PLEXIL::LabelStr predicado(argv[1]);
              int nobj = static_cast<int> (argv[2]);
              char aux[31];

              strcpy(aux,(char*)predicado.c_str());

              success = planificador[nplanner]->objeto_pred((char*)aux,nobj);

              PLEXIL::LabelStr objeto(aux);

              debugMsg("PDDLAdapter", "Object #"<<nobj<<" for ("<<predicado.c_str()<<") is "<<objeto.c_str());

              retval = objeto;
          }

          else if(name == "action_object")
          {
              int nobj = static_cast<int> (argv[1]);

              if(nobj>0 && nobj<=planificador[nplanner]->numero_objetos_proceso())
              {
                  PLEXIL::LabelStr obj(ultimaacc[nplanner]->objetos[nobj-1]);

                  debugMsg("PDDLAdapter", "Object #"<<nobj<<" for last action is "<<obj.c_str());

                  retval = obj.c_str();
              }
              else
              {
                  success=false;

                  PLEXIL::LabelStr vacia;

                  retval = vacia.getKey();
              }
          }

          else if(name == "action_duration")
          {
              debugMsg("PDDLAdapter", "Duration for last action is "<<ultimaacc[nplanner]->valor);

              retval = ultimaacc[nplanner]->valor;
          }

          else if(name == "function_value")
          {
              PLEXIL::LabelStr funcion(argv[1]);
              PLEXIL::LabelStr objetos(argv[2]);
              float valor;

              valor = planificador[nplanner]->valor_funcion((char*)funcion.c_str(),(char*)objetos.c_str());

              debugMsg("PDDLAdapter", "Value for function ("<<funcion.c_str()<<" "<<objetos.c_str()<<") is "<<valor);

              retval = valor;
          }

      }else //PLANIFICADOR FUERA DE RANGO
          retval=false;
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



Value PDDLAdapter::lookupNow (const State& state)
{
  // This is the name of the state as given in the plan's LookupNow
  const LabelStr& name = state.first;
  const vector<Value>& args = state.second;
  return fetch(name.toString(), args);
}


void PDDLAdapter::subscribe(const State& state)
{
  LabelStr nameLabel = LabelStr (state.first);
  debugMsg("PDDLAdapter:subscribe", " processing state "
           << nameLabel.toString());
  m_subscribedStates.insert(state);
}


void PDDLAdapter::unsubscribe (const State& state)
{
  LabelStr nameLabel = LabelStr (state.first);
  debugMsg("PDDLAdapter:subscribe", " from state "
           << nameLabel.toString());
  m_subscribedStates.erase(state);
}

// Does nothing.
void PDDLAdapter::setThresholds (const State& state, double hi, double lo)
{
}


void PDDLAdapter::propagateValueChange (const State& state,
                                          const vector<Value>& vals) const
{
  if (!isStateSubscribed(state))
	return;
  m_execInterface.handleValueChange (state, vals.front());
  m_execInterface.notifyOfExternalEvent();
}


bool PDDLAdapter::isStateSubscribed(const State& state) const
{
  return m_subscribedStates.find(state) != m_subscribedStates.end();
}

// Necessary boilerplate
extern "C" {
  void initPDDLAdapter() {
    REGISTER_ADAPTER(PDDLAdapter, "PDDLAdapter");
  }
}
