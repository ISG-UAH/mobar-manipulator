#ifndef ROBOTDH_H
#define ROBOTDH_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iomanip>      // std::setw

using namespace std;

typedef struct ROBOT_JOINT {
	int numjoint;
	double a;
	double alpha;
	double d;
	double theta;
} ROBOT_JOINT;


class RobotDH {

public:
	  RobotDH();
	  RobotDH(const char* filename);
	  ~RobotDH();

	  int ReadConfigFile(const char* filename);
	  int NewConfigFile();
	  void PrintJoints();

	  int getDof();
	  int check();
	  ROBOT_JOINT * getJoints();
	  ROBOT_JOINT getOneJoint(int joint);

private:
	  ROBOT_JOINT *juntaRobot;
	  int dof, num_error;

}; // End class RobotDH

#endif
