/*
 * locomotion.h
 *
 *  	Created on: 17/06/2014
 *      Author: Juan Alejandro Mora Prieto
 */
#ifndef LOCOMOTION_H
#define LOCOMOTION_H

#include <stdio.h>
#include <strings.h>
#include <Eigen/Dense>
#include <Eigen/SVD>

/* Cinematica Directa e Inversa - Orocos KDL */
#include <kdl/tree.hpp>
#include <kdl/chain.hpp>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/frames_io.hpp>
#include <kdl/frames.hpp>
#include <kdl/chainiksolver.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/chainiksolvervel_wdls.hpp>
#include <kdl/chainiksolverpos_nr.hpp>
#include <kdl/chainiksolverpos_nr_jl.hpp>
#include "chainiksolverpos_nr_jl_we.hpp"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

//#include "../genom/locomotion/locomotionStruct.h"
#include "controller.h"

#define PRECISION 3
#define WIDTH 15

using namespace KDL;
using namespace std;

class locomotion {

public:
		/**
		 * The constructor of locomotion class. It setup a chain with DH parameters.
		 * It must be necessary call to SetServos function with default servos values.
		 *
		 */
		locomotion();
		~locomotion();

		/**
		 * SetServos function. This function is necessary after instace the class in order to setup
		 * default values of servos.
		 *
		 * @param servosInit: SERVOS struct with servos default values, given by Arduino's code.
		 */
		void SetServos(SERVOS servosInit);

		/**
		 * Move function. Calculate the servos values for a position matrix.
		 *
		 * @param p[3]: position array represent (x,y,z) coordinates.
		 *
		 * @return SERVOS struct with servos angles values in radians.
		 */
		int Move(double p[3], SERVOS &serv);

		/**
		 * Move function. Calculate the servos values for a position matrix.
		 *
		 * @param positionTarget: POSITION struct with Rotation coordinates (n, o, a)
		 * and Position coordinates p (x, y, z).
		 *
		 * @return SERVOS struct with servos angles values in radians.
		 */
		int Move(POSITION positionTarget, SERVOS &serv);
		int MoveTo(double p[3], SERVOS &serv);
		/**
		 * Rotate function. Calculate the servos values for an angle rotate.
		 *
		 * @param rotate: ANGLEROTATE struct wich indicates axis (0 = alpha, 1 = theta, 2 = psi)
		 * and angle value in radians.
		 *
		 * @return SERVOS struct with servos angles values in radians.
		 */
		int Rotate(ANGLEROTATE rotate, SERVOS &serv);

		/**
		 * GetServos function. This function returns servos values and error.
		 *
		 * @param serv: SERVOS struct as reference.
		 *
		 * @return error value.
		 */
		int GetServos(SERVOS &serv);

		/**
		 * GetPosition function. This function returns POSITION struct and error.
		 *
		 * @param pos: POSITION struct as reference.
		 *
		 * @return error value
		 */
		int GetPosition(POSITION &pos);

		/**
		 * Print functions. This functions show in prompt POSITION and SERVOS values .
		 *
		 * @return void.
		 */
		void print();
		void print(POSITION pos);
		void print(SERVOS ser);

		/**
		 * checkError functions. This functions returns int error value.
		 *
		 * @return error value.
		 */
		int checkError(void);
		int checkPosition(POSITION pos);
		int checkServos(SERVOS serv);
		int checkAngle(ANGLEROTATE ang);

private:
		Chain chain;
		JntArray qMin, qMax;
		POSITION position;
		SERVOS servos;
		int error;
		log4cpp::Category *root, *sub1; // LOG

		/**
		 * InvKine function. Calculate inverse kinematics with an algorithm based on Newton-Raphson
		 * iterations considering 5 dofs and joint's limits.
		 *
		 * @param const servosInit: SERVOS struct with initial servos desired.
		 * const positionInit: POSITION initial struct.
		 * servosTarget: SERVOS target struct.
		 * positionTarget: POSITION target struct.
		 * @return int error. 0 = correct. -1 = error.
		 */
		int InvKine(const SERVOS servosInit, const POSITION positionInit, SERVOS &servosTarget, POSITION &positionTarget);

		/**
		 * ForwKine function. Calculate fordward kinematics based on initial servos values
		 *
		 * @param servosInit: SERVOS struct with initial servos desired.
		 *
		 * @return POSITION struct wich indicates axis (0 = alpha, 1 = theta, 2 = psi)
		 * and angle value in radians.
		 */
		POSITION ForwKine(SERVOS servosInit);

		/**
		 * ServosToJnt function. Complementary function to transform SERVOS struct into JntArray
		 * used in KDL library.
		 *
		 * @param qInt: SERVOS struct to transform.
		 *
		 * @return KDL JntArray.
		 */
		JntArray ServosToJnt(SERVOS qInt);

		/**
		 * JntToServos function. Complementary function to transform JntArray used in KDL library into
		 * SERVOS struct.
		 *
		 * @param qServos: KDL JntArray to transform.
		 *
		 * @return SERVOS struct transformed.
		 */
		SERVOS JntToServos(JntArray qServos);

		/**
		 * FrameToPosition function. Complementary function to transform Frame class used in KDL library into
		 * POSITION friendly struct.
		 *
		 * @param coordBrazo: KDL Frame to transform.
		 *
		 * @return POSITION struct transformed.
		 */
		POSITION FrameToPosition(Frame coordBrazo);

		/**
		 * PositionToFrame function. Complementary function to transform POSITION struct into Frame class
		 * used in KDL library.
		 * .
		 * @param positionTarget: POSITION struct.
		 *
		 * @return KDL Frame transformed.
		 */
		Frame PositionToFrame(POSITION positionTarget);

		/**
		 * QLimits function. Copy limits values of servos to JntArray from KDL library.
		 *
		 * @param limits[5]: double values for servos limits.
		 *
		 * @return KDL JntArray.
		 */
		JntArray QLimits(double limits[5]);

		/**
		 * Log function. Configure log options for log4cpp library.
		 *
		 * @return void.
		 */
		void log();

}; // End class locomotion


#endif
