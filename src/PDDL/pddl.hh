/** @file "ogate.hh"
 *  @brief OGATE plug-in utilities
 *
 *  This file defines the utilities provided by the OGATE plug-in.
 *
 *  UAH-PST-ESA NPI:CSAEM 2014
 *
 *  @author Pablo Muñoz
 */
#ifndef H_Pddl
#define H_Pddl

#include <string>
#include <iostream>
#include <sys/wait.h>

#include "TeleoReactor.hh"
#include "Observation.hh"
#include "Goal.hh"
#include "Plugin.hh"
#include "LogManager.hh"
#include "Planner.h"

using namespace std;
using namespace TREX::transaction;
using namespace TREX::utils;

#define POS_DELIMITER "_"

namespace TREX {
  /** @brief Ogate plug-in utilities
   *
   * This namespace embeds the classes and functions provided by the OGATE plug-in
   */
  namespace pddl {

    /** @brief OgateReactor reactor definition
     *
     * This class implements an OGATE interface reactor.
     * This reactor is not deliberative.
     * This reactor uses all system timelines.
     */
    class PddlReactor :public TREX::transaction::TeleoReactor {
    public:
      /** @brief XML constructor
       * @param arg An XML node definition
       *
       * This constructor is called to generate a OgateReactor reactor
       * based on an XML node. The expected XML format is the following:
       * @code
       * <OgateReactor name="<name>" latency="<int>" lookahead="<int>"/>
       * @endcode
       */
      PddlReactor(TREX::transaction::TeleoReactor::xml_arg_type arg);
      /** @brief Destructor */
      ~PddlReactor();

    private:
      void handleInit();
      void handleRequest(TREX::transaction::goal_id const &g);
      void handleRecall(TREX::transaction::goal_id const &g);
      void handleTickStart();
      void notify(Observation const &obs);
      bool synchronize();
      bool hasWork();
      void resume();

      bool getCoords(char* pos, int* x, int* y);
      char* getObjPos(char* objects, int pos);
      Observation goalTOobs(Goal g);
    
      std::string m_name;
      std::string config_file;
      Observation* status;
      bool changestatus;
      Planner* planner;
      int pidplanner;
      bool reqplanning;
      bool isplanning;
      bool replanning;
      bool finish;
      int currentgoal;
      std::vector<Observation> obslist;
      std::vector<Goal> goalslist;
      std::string lastPictDat;
    };

  }
}

#endif // H_Pddl
