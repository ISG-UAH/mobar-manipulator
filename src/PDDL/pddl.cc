/** @file "pddl.cc"
 *  @brief PDDL deliberative reactor
 *
 *  This file implements a PDDL deliberative plug-in.
 *
 *  UAH-PST-ESA NPI:CSAEM 2014
 *
 *  @author Pablo Muñoz
 */

#include "pddl.hh"

using namespace TREX::utils;
using namespace TREX::transaction;
using namespace TREX::pddl;

namespace {

  /** @brief TREX log entry point */
  SingletonUse<LogManager> s_log;

  /** @brief OgateReactor reactor declaration */
  TeleoReactor::xml_factory::declare<PddlReactor> decl("PddlReactor");
  
}

namespace TREX {
  
  /** @brief Plug-in initialisation
   *
   * This function is called by TREX after loading the OGATE plug-in.
   * It manage the initialisation of this plug-in
   */
  void initPlugin() {
    ::s_log->syslog("PddlReactor")<<"PDDL plugin loaded."<<std::endl;
  }

} // TREX

PddlReactor::PddlReactor(TeleoReactor::xml_arg_type arg)
  :TeleoReactor(arg, true) {

  /* Read name from cfg file */
  m_name = parse_attr<string>("PDDL", TeleoReactor::xml_factory::node(arg), "name");
  /** Configuration file for PDDL planner */
  config_file = parse_attr<string>("config", TeleoReactor::xml_factory::node(arg), "configFile");
}

PddlReactor::~PddlReactor()
{

}

void PddlReactor::handleInit()
{
    isplanning = false;
    replanning = false;
    reqplanning = true;
    changestatus = true;
    finish = false;
    currentgoal = 0;
    planner = new Planner((char*)config_file.c_str());
    planner->eliminar_inits();
    status = new Observation("MissionTimeline.mission_timeline", "Idle");
    std::cout << "----- ["<< m_name <<" reactor] " << "HANDLE INIT" << std::endl;
}


bool PddlReactor::getCoords(char* pos, int* x, int* y)
{
    char* temptok;
    pos[0] = ' '; // Delete first char
    temptok=strtok(pos, (char*)POS_DELIMITER); // First token, X position
    if(temptok == NULL)
        return false; // Malformed square
    else
        *x = atoi(temptok);
    // Next token
    temptok=strtok(NULL, (char*)POS_DELIMITER); // Second token, Y position
    if(temptok == NULL)
        return false; // Malformed square
    else
        *y = atoi(temptok);
    return true;
}

char* PddlReactor::getObjPos(char* objects, int pos)
{
    int numobjs = planner->numero_objetos_proceso();    
    if(pos < 0 || pos > numobjs)
        return NULL;
        
    char* ret = new char[128];
    char* aux=strtok(objects, (char*)" ");
    int i=0;
    while(aux != NULL)
    {
        if(i == pos)
        {
            strcpy(ret, (char*)aux);
            return ret;
        }
        aux = strtok(NULL," ");
        i++;
    }
    return NULL;
}


Observation PddlReactor::goalTOobs(Goal g)
{
    Observation newobs = Observation(g.object(), g.predicate());
    TREX::transaction::Predicate::iterator attr=g.begin();
    for(; attr!=g.end(); ++attr)
        if(attr->first != TREX::utils::Symbol("start") &&
           attr->first != TREX::utils::Symbol("duration") &&
           attr->first != TREX::utils::Symbol("end") )
              newobs.restrictAttribute(attr->first, attr->second.domain());
    return newobs;
}


void PddlReactor::handleTickStart()
{
  std::cout << "----- ["<< m_name <<" reactor] " << "HANDLE TICK START INIT" << std::endl;
    if(!reqplanning && obslist.empty())
    {
        std::cout << "Reading next planned action... ";
        int numaccion=1;
        float duracion=1000;
        char accion[TAM_LINEA];
        char objetos[TAM_LINEA];
        numaccion = planner->sig_accion(accion, objetos, &duracion);
        if(numaccion > 0) // Put goals
        {
            bool ret = false;
            char* tm = new char[128];
            tm = planner->conector_accion();
            int t = getCurrentTick()+1;
            std::cout << "#" << numaccion << " "  << accion << " " << objetos << " [" << t << ", " << duracion << "]" << std::endl;
            std::cout << "Posting goals in external timeline "<< tm << std::endl;
            if(!strcmp(tm, (char*)"RobotBase.robot_base"))
            {
                int x,y;
                getCoords(getObjPos(objetos, 1), &x, &y);
                Goal g(tm, "GoingToLaser" );
                g.restrictTime(IntegerDomain(t,t),
                               IntegerDomain(1,(int)t+duracion),
                               IntegerDomain(t+2,(int)t+duracion) );
                g.restrictAttribute( Variable("X",IntegerDomain(x,x)));
                g.restrictAttribute( Variable("Y",IntegerDomain(y,y)));
                ret = postGoal(g);
                if(ret)
                {
                    status = new Observation("MissionTimeline.mission_timeline", "GoingToLaser");
                    status->restrictAttribute( Variable("X",IntegerDomain(x,x)));
                    status->restrictAttribute( Variable("Y",IntegerDomain(y,y)));
                    changestatus = true;
                    obslist.push_back(goalTOobs(g));
                    t++;
                    Goal g2(tm, "At" );
                    g2.restrictTime(IntegerDomain(t,t),
                                    IntegerDomain(1,(int)t+duracion),
                                    IntegerDomain(t+1,(int)t+duracion) );
                    g2.restrictAttribute( Variable("X",IntegerDomain(x,x)));
                    g2.restrictAttribute( Variable("Y",IntegerDomain(y,y)));
                    ret = postGoal(g2);
                    if(ret) obslist.push_back(goalTOobs(g2));
                }
            }else if(!strcmp(tm, (char*)"Platine.platine"))
            {
                int x,y;
                getCoords(getObjPos(objetos, 2), &x, &y);
                Goal g(tm, "MovingTo" );
                g.restrictTime(IntegerDomain(t,t),
                               IntegerDomain(1,(int)t+duracion),
                               IntegerDomain(t+1,(int)t+duracion) );
                g.restrictAttribute( Variable("Pan",IntegerDomain(x,x)));
                g.restrictAttribute( Variable("Tilt",IntegerDomain(y,y)));
                ret = postGoal(g);
                if(ret)
                {
                    status = new Observation("MissionTimeline.mission_timeline", "MovingPlatine");
                    status->restrictAttribute( Variable("Pan",IntegerDomain(x,x)));
                    status->restrictAttribute( Variable("Tilt",IntegerDomain(y,y)));
                    changestatus = true;
                    Goal g2(tm, "PointingAt");
                    t++;
                    g2.restrictTime(IntegerDomain(t,t),
                               IntegerDomain(1,(int)t+duracion),
                               IntegerDomain(t+1,(int)t+duracion) );
                    g2.restrictAttribute( Variable("X",IntegerDomain(x,x)));
                    g2.restrictAttribute( Variable("Y",IntegerDomain(y,y)));
                    obslist.push_back(goalTOobs(g2));
                }
            }else if(!strcmp(tm, (char*)"Camera.camera"))
            {
                Goal g(tm, "TakingPicture" );
                g.restrictTime(IntegerDomain(t,t),
                               IntegerDomain(1,(int)t+duracion),
                               IntegerDomain(t+1,(int)t+duracion) );
                char* objid = getObjPos(objetos, 0); objid[0] = ' ';
                int id = atoi(objid);
                g.restrictAttribute( Variable("A_id",IntegerDomain(id,id)));
                ret = postGoal(g);
                if(ret)
                {
                    status = new Observation("MissionTimeline.mission_timeline", "TakingPicture");
                    status->restrictAttribute( Variable("Id",IntegerDomain(id,id)));
                    changestatus = true;
                    t++;
                    Goal g2(tm, "CamIdle" );
                    g2.restrictTime(IntegerDomain(t,t),
                                    IntegerDomain(1,(int)t+duracion),
                                    IntegerDomain(t+1,(int)t+duracion) );
                    ret = postGoal(g2);
                    if(ret) obslist.push_back(goalTOobs(g2));
                }
            }else if(!strcmp(tm, (char*)"Communication.communication"))
            {
                Goal g(tm, "Communicating" );
                g.restrictTime(IntegerDomain(t,t),
                               IntegerDomain(1,(int)t+duracion),
                               IntegerDomain(t+1,(int)t+duracion) );
                char* objid = getObjPos(objetos, 0); objid[0] = ' ';
                int id = atoi(objid);
                g.restrictAttribute( Variable("A_id",IntegerDomain(id,id)));
                ret = postGoal(g);
                if(ret)
                {
                    status = new Observation("MissionTimeline.mission_timeline", "Communicating");
                    status->restrictAttribute( Variable("Id",IntegerDomain(id,id)));
                    changestatus = true;
                    t++;
                    Goal g2(tm, "CommIdle" );
                    g2.restrictTime(IntegerDomain(t,t),
                                    IntegerDomain(1,(int)t+duracion),
                                    IntegerDomain(t+1,(int)t+duracion) );
                    ret = postGoal(g2);
                    if(ret) obslist.push_back(goalTOobs(g2));
                }
            }
        } // Action readed --> put goals
        else
        {
            if(!finish)
            {
                status = new Observation("MissionTimeline.mission_timeline", "Idle");
                changestatus = true;
                finish = true;
            }
            std::cout << " no pending actions." << std::endl;
            if(autoFinish && getCurrentTick() > 10)
                ogateStatus = COMPONENT_STOPPED;
        }
    }
    else
    {
        std::cout << "WAITING FOR EXTERNAL STATES: " << std::endl;
        for(std::vector<Observation>::iterator it = obslist.begin(); it != obslist.end(); ++it)
             std::cout << *it << std::endl;
    }
  std::cout << "----- ["<< m_name <<" reactor] " << "HANDLE TICK START END " << std::endl;
}

bool PddlReactor::synchronize()
{
//Observation stateObs("CommunicationVW.communication_windows", "visible");
    if(changestatus)
    {
        postObservation(*status);
        changestatus = false;
        delete status;
    }
//    std::cout << "----- ["<< m_name <<" reactor] " << "SYNCHRONIZE" << std::endl;
  return( true );
}

void PddlReactor::notify(Observation const &obs)
{
    std::cout << "----- ["<< m_name <<" reactor] " << "NOTIFY INIT" << std::endl;
    // Search if waiting for observation
    std::ostringstream out;
    std::string co1, co2;
    out << obs;
    co1 = out.str();
    out.str("");
    bool removed = false;
    std::vector<Observation>::iterator it = obslist.begin();
    while(it != obslist.end() && !removed)
    {
        out << *it;
        co2 = out.str();
        out.str("");
        if(!co1.compare(co2))
        {
            std::cout << "Removing pending state " << *it << std::endl;
            removed = true;
            obslist.erase(it);
        }
        it++;
    }
    if(!removed)
        std::cout << "No waiting for state " << obs << std::endl;


    // UPDATE PDDL PROBLEM FACTS
    if(obs.predicate() == TREX::utils::Symbol("At"))
    {
        TREX::transaction::Predicate::const_iterator attr=obs.begin();
        out << " C";
        for(; attr!=obs.end(); ++attr)
            if(attr->first == TREX::utils::Symbol("X") ||
               attr->first == TREX::utils::Symbol("B_X"))
                out << attr->second.domain();
            else if(attr->first == TREX::utils::Symbol("Y") ||
                    attr->first == TREX::utils::Symbol("C_Y"))
                out << "_" << attr->second.domain();

        if(planner->reemplazar_pred((char*)"position", NULL, (char*)out.str().c_str()) == 0)
            std::cout << "Modified PDDL state (position " << out.str() << ")" << std::endl;
        out.str("");
    }
    else if(obs.predicate() == TREX::utils::Symbol("PointingAt"))
    {
        TREX::transaction::Predicate::const_iterator attr=obs.begin();
        out << "NavCam p";
        for(; attr!=obs.end(); ++attr)
            if(attr->first == TREX::utils::Symbol("Pan") ||
               attr->first == TREX::utils::Symbol("D_pan") ||
               attr->first == TREX::utils::Symbol("X"))
                out << attr->second.domain();
            else if(attr->first == TREX::utils::Symbol("Tilt") ||
                    attr->first == TREX::utils::Symbol("E_tilt") ||
                    attr->first == TREX::utils::Symbol("Y"))
                out << "_" << attr->second.domain();

        if(planner->reemplazar_pred((char*)"platine_pos", (char*)"NavCam", (char*)out.str().c_str()) == 0)
            std::cout << "Modified PDDL state (platine_pos " << out.str() << ")" << std::endl;
        out.str("");
    }
    else if(obs.predicate() == TREX::utils::Symbol("TakingPicture"))
    {
        TREX::transaction::Predicate::const_iterator attr=obs.begin();
        for(; attr!=obs.end(); ++attr)
            if(attr->first == TREX::utils::Symbol("A_ID"))
                out << "i" << attr->second.domain();
            else if(attr->first == TREX::utils::Symbol("B_X"))
                out << " C" << attr->second.domain();
            else if(attr->first == TREX::utils::Symbol("C_Y"))
                out << "_" << attr->second.domain();
            else if(attr->first == TREX::utils::Symbol("D_A"))
                out << " p" << attr->second.domain();
            else if(attr->first == TREX::utils::Symbol("E_B"))
                out << "_" << attr->second.domain();
            lastPictDat = out.str();
            out.str("");
    }
    else if(obs.predicate() == TREX::utils::Symbol("CamIdle")  && lastPictDat.size() > 9)
    {
        if(planner->insertar_pred((char*)"picture", (char*)lastPictDat.c_str(), false) == 0)
            std::cout << "Included PDDL state (picture " << lastPictDat << ")" << std::endl;
        out.str("");
    }
    else if(obs.predicate() == TREX::utils::Symbol("CommIdle") && lastPictDat.size() > 9)
    {
        if(planner->insertar_pred((char*)"transmited", (char*)lastPictDat.c_str(), false) == 0)
            std::cout << "Included PDDL state (transmited " << out.str() << ")" << std::endl;
        out.str("");
        lastPictDat.clear();
    }

    std::cout << "----- ["<< m_name <<" reactor] " << "NOTIFY END " << std::endl;
}

void PddlReactor::handleRequest(TREX::transaction::goal_id const &g)
{
    std::cout << "----- ["<< m_name <<" reactor] " << "HANDLE REQUEST INIT" << std::endl;
    std::cout << "New goal "<<*g<<std::endl;
    Goal newgoal(*g);
    goalslist.push_back(newgoal);
    reqplanning = true;
    if(getCurrentTick() > 0)
        replanning = true;
    std::cout << "----- ["<< m_name <<" reactor] " << "HANDLE REQUEST END " << std::endl;
}

void PddlReactor::handleRecall(TREX::transaction::goal_id const &g)
{
    std::cout << "----- ["<< m_name <<" reactor] " << "HANDLE RECALL INIT" << std::endl;
    // Search if waiting for observation
    Goal gg(*g);
    std::ostringstream out;
    std::string cg1, cg2;
    out << gg;
    cg1 = out.str();
    out.str("");
    bool removed = false;
    std::vector<Goal>::iterator it = goalslist.begin();
    while(it != goalslist.end() && !removed)
    {
        out << *it;
        cg2 = out.str();
        out.str("");
        if(!cg1.compare(cg2))
        {
            std::cout<< "Removing pending goal "<<*it<<std::endl;
            removed = true;
            reqplanning = true;
            replanning = true;
            goalslist.erase(it);
        }
    }
    if(!removed)
        std::cout << "The goal is not pending " << gg << std::endl;
    std::cout << "----- ["<< m_name <<" reactor] " << "HANDLE RECALL END " << std::endl;
}

bool PddlReactor::hasWork()
{
    return (reqplanning || isplanning);
}

void PddlReactor::resume()
{
    std::cout << "----- ["<< m_name <<" reactor] " << "RESUME INIT" << std::endl;
    if(reqplanning && !isplanning)
    {
        // Remove previous goals from PDDL problem
        planner->eliminar_goals();
        status = new Observation("MissionTimeline.mission_timeline", "Deliberating");
        changestatus = true;
        // Update PDDL problem file
        std::ostringstream out;
        for(std::vector<Goal>::iterator it = goalslist.begin(); it != goalslist.end(); ++it)
        {
            out.str("");
            if((*it).predicate() == TREX::utils::Symbol("TakePicture"))
            {
                TREX::transaction::Predicate::iterator attr=(*it).begin();
                for(; attr!=(*it).end(); ++attr)
                    if(attr->first == TREX::utils::Symbol("A_id"))
                        out << "i" << attr->second.domain() << " C";
                    else if(attr->first == TREX::utils::Symbol("B_X"))
                        out << attr->second.domain();
                    else if(attr->first == TREX::utils::Symbol("C_Y"))
                        out << "_" << attr->second.domain();
                    else if(attr->first == TREX::utils::Symbol("D_pan"))
                        out << " p" << attr->second.domain();
                    else if(attr->first == TREX::utils::Symbol("E_tilt"))
                        out << "_" << attr->second.domain();

                planner->insertar_pred((char*)"transmited", (char*)out.str().c_str(), true);
            }
            else if((*it).predicate() == TREX::utils::Symbol("GoTo"))
            {
                TREX::transaction::Predicate::iterator attr=(*it).begin();
                out << "C";
                for(; attr!=(*it).end(); ++attr)
                    if(attr->first == TREX::utils::Symbol("X") ||
                       attr->first == TREX::utils::Symbol("B_X"))
                        out << attr->second.domain();
                    else if(attr->first == TREX::utils::Symbol("Y") ||
                            attr->first == TREX::utils::Symbol("C_Y"))
                        out << "_" << attr->second.domain();

                planner->insertar_pred((char*)"position", (char*)out.str().c_str(), true);
            }
        }

        std::cout << "----- ["<< m_name <<" reactor] " << "IS DELIBERATING" << std::endl;
        if(planner->num_replanificaciones() > 0) std::cout << "Number of replans: " << planner->num_replanificaciones() << std::endl;
        isplanning = true;
        pidplanner = planner->planificarBG();
        if(pidplanner > 0)
        {
            int estadosal;
            waitpid(pidplanner, &estadosal, 0);
            planner->inicializarEstructuras(replanning);
            isplanning = false;
            reqplanning = false;
            replanning = false;
            finish = false;
            currentgoal = 0;
            obslist.clear();
        }
    }
    std::cout << "----- ["<< m_name <<" reactor] " << "RESUME END " << std::endl;
}

