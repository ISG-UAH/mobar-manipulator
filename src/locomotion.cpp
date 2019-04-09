/*
 * locomotion.cpp
 *
 *  	Created on: 17/06/2014
 *      Author: Juan Alejandro Mora Prieto
 */
#include "locomotion.h"


locomotion::locomotion(){
	double min[5], max[5];
	int i, count;
	double a[5], alpha[5], d[5], theta[5];

	// Configure LOG
	log();

	// Valores iniciales de los servos
	float valservo[] = {90, 130, 90, 130, 90, 60};

	// Parametros DH del robot
	a[0] = 0;		alpha[0] = M_PI/2;	d[0] = 2;	theta[0] = 0;
	a[1] = 10.5;	alpha[1] = -M_PI;	d[1] = 0;	theta[1] = 0;
	a[2] = 7;		alpha[2] = 0;		d[2] = 0;	theta[2] = 0;
	a[3] = -3;		alpha[3] = M_PI/2;	d[3] = 0;	theta[3] = 0;
	a[4] = 0;		alpha[4] = 0;		d[4] = 10;	theta[4] = 0;

	// Se define la caracterizacion cinematica del robot:
	for(count=0; count < 5; count++){
		chain.addSegment(Segment(Joint(Joint::RotZ),Frame::DH(a[count], alpha[count], d[count], theta[count])));
	}
	// Tupla con las posiciones de los servos actuales
	 qMin(chain.getNrOfJoints());
	 qMax(chain.getNrOfJoints());

	 for(count=0; count<chain.getNrOfJoints(); count++){
			 min[count]=0;
			 max[count]=M_PI;
			 servos.q[count] = valservo[count]*M_PI/180;
	 }
	 qMin = QLimits(min);
	 qMax = QLimits(max);

	 servos.num_servos = chain.getNrOfJoints();
	 position = ForwKine(servos);

	 sub1->debug("LOC: Locomotion configured correctly. N. dof: %d", servos.num_servos);
}

locomotion::~locomotion(){
	sub1->debug("LOC: Locomotion destroyed");
}

/* Desplazamientos y Rotacion */
int locomotion::Move(double p[3], SERVOS &serv){
	POSITION positionTarget;
	Frame coordBrazo = Frame::Identity();
	int i, ret;

	positionTarget = FrameToPosition(coordBrazo);

	for(i=0; i<3; i++){
		positionTarget.p[i] = p[i];
	}

	ret = Move(positionTarget, serv);

	return ret;
}

int locomotion::Move(POSITION positionTarget, SERVOS &serv){
	Frame coordAct, coordNew;
	SERVOS initServos, retServos;
	POSITION initPos, retPos;
	int ret;

	if((ret=checkPosition(positionTarget)) == 0){
		initServos = servos;
		initPos = position;

		coordAct = PositionToFrame(initPos);
		coordNew = PositionToFrame(positionTarget);
		coordAct = coordAct * coordNew;

		initPos = FrameToPosition(coordAct);

		sub1->debug("LOC: Move. POS: n = {%1.3f, %1.3f, %1.3f}. o = {%1.3f, %1.3f, %1.3f}. a = {%1.3f, %1.3f, %1.3f}. p = {%1.3f, %1.3f, %1.3f}",
				initPos.n[0], initPos.n[1], initPos.n[2],
				initPos.o[0], initPos.o[1], initPos.o[2],
				initPos.a[0], initPos.a[1], initPos.a[2],
				initPos.p[0], initPos.p[1], initPos.p[2]);

		ret = InvKine(initServos, initPos, retServos, retPos);
		if(ret == 0){
			servos = retServos;
			position = retPos;
		}
		serv = retServos;
	}

	return ret;
}



int locomotion::MoveTo(double p[3], SERVOS &serv){
	Frame coordAct, coordNew;
	SERVOS initServos, retServos;
	POSITION initPos, retPos;
	int ret, i;
	double pos[3];

	initServos = servos;
	initPos = position;

	coordAct = PositionToFrame(initPos);
	coordNew = coordAct;

	for (i = 0; i<3; i++){
		coordNew.p[i] = p[i];
	}

	coordAct = coordAct.Inverse()*coordNew;

	initPos = FrameToPosition(coordAct);

	for (i = 0; i<3; i++){
		pos[i] = initPos.p[i];
	}

	ret = Move(pos, servos);
	return ret;
}


int locomotion::Rotate(ANGLEROTATE rotate, SERVOS &serv){
	Frame coordBrazo;
	Frame coordAct, coordNew;
	SERVOS initServos, retServos;
	POSITION initPos, retPos;
	int ret;

	if((ret=checkAngle(rotate)) == 0){

		initServos = servos;
		initPos = position;

		coordAct = PositionToFrame(initPos);
		coordNew = coordAct;

		switch(rotate.axis){
		case 0: // Eje alpha
			coordNew.M.DoRotX(rotate.angle);
			break;
		case 1: // Eje theta
			coordNew.M.DoRotY(rotate.angle);
			break;
		case 2: // Eje psi
			coordNew.M.DoRotZ(rotate.angle);
			break;
		}

		initPos = FrameToPosition(coordNew);

		sub1->debug("LOC: Rotate. POS: n = {%1.3f, %1.3f, %1.3f}. o = {%1.3f, %1.3f, %1.3f}. a = {%1.3f, %1.3f, %1.3f}. p = {%1.3f, %1.3f, %1.3f}",
				initPos.n[0], initPos.n[1], initPos.n[2],
				initPos.o[0], initPos.o[1], initPos.o[2],
				initPos.a[0], initPos.a[1], initPos.a[2],
				initPos.p[0], initPos.p[1], initPos.p[2]);

		ret = InvKine(initServos, initPos, retServos, retPos);
		if(ret == 0){
			servos = retServos;
			position = retPos;
		}
		serv = retServos;
	}
	return ret;
}

int locomotion::GetServos(SERVOS &serv){
	serv = servos;
	return error;
}

int locomotion::GetPosition(POSITION &pos){
	pos = position;
	return error;
}

void locomotion::SetServos(SERVOS servosInit){
	servos = servosInit;
	position = ForwKine(servosInit);
}



/* Cinematica Desplazamientos y Rotacion */
int locomotion::InvKine(const SERVOS servosInit, const POSITION positionInit, SERVOS &servosTarget, POSITION &positionTarget){
	SERVOS retServos, errServos;
	JntArray qInit(chain.getNrOfJoints());
	Eigen::VectorXd weight_ts(6);
	Frame coordBrazo;
	double weights[6];
	int i, ret;

	qInit = ServosToJnt(servosInit);
	JntArray qFinal(chain.getNrOfJoints());
	errServos = servosInit;

	coordBrazo = PositionToFrame(positionInit);


	/* Establece unos pesos para eliminar los grados de libertad que falten hasta 6dof */
	for (i=0; i<chain.getNrOfJoints(); i++){
		weights[i] = 1;
		weight_ts[i] = 1.0;
	}
	for (i=chain.getNrOfJoints(); i<6; i++){
			weights[i] = 0;
			weight_ts[i] = 0.0;
	}

	ChainFkSolverPos_recursive cinematicaDirecta(chain);

	ChainIkSolverVel_wdls cinematicaInversaV(chain);
//	cinematicaInversaV.setWeightTS(weight_ts.asDiagonal());
	cinematicaInversaV.weight_ts = weight_ts.asDiagonal();

	ChainIkSolverPos_NR_JL_WE cinematicaInversa(chain, qMin, qMax, cinematicaDirecta, cinematicaInversaV, 100, 1e-6);
	cinematicaInversa.setWeights(weights);

	ret = cinematicaInversa.CartToJnt(qInit, coordBrazo, qFinal);

	if(ret >= 0){
		/* Calculos correctos de la cinematica inversa */
		servosTarget = JntToServos(qFinal);
		positionTarget = FrameToPosition(coordBrazo);

		error = 0;
		return error;
	}else{
		/* Error en el calculo de la cinematica inversa */
		sub1->error("LOC: Inverse Kinematics fails.");

		servosTarget = servosInit;
		positionTarget = positionInit;

		error = -1;
		return error;
	}
}
POSITION locomotion::ForwKine(SERVOS servosInit){
	POSITION positionTarget;
	JntArray qServos;
	Frame coordBrazo;

	qServos = ServosToJnt(servosInit);

	// Aplicacion de la cinematica directa
	ChainFkSolverPos_recursive fkine(chain);

	if(fkine.JntToCart(qServos, coordBrazo)<0){
		sub1->error("LOC: Forward Kinematics fails.");
		error = -2;
	}

	positionTarget = FrameToPosition(coordBrazo);

	return positionTarget;
}

/* Conversion de coordenadas */
JntArray locomotion::ServosToJnt(SERVOS qInt){
	JntArray qRet(chain.getNrOfJoints());
	int i;

	for (i = 0; i < chain.getNrOfJoints(); i++){
		qRet(i) = qInt.q[i];
	}

	return qRet;
}
SERVOS locomotion::JntToServos(JntArray qServos){
	SERVOS qRet;
	int i;
	qRet.num_servos = chain.getNrOfJoints();
	for (i = 0; i < chain.getNrOfJoints(); i++){
		qRet.q[i] = qServos(i);
	}

	return qRet;
}

JntArray locomotion::QLimits(double limits[5]){
	JntArray qLim(chain.getNrOfJoints());

	for(int i= 0; i<chain.getNrOfJoints(); i++)
		qLim(i) = limits[i];

	return qLim;
}

POSITION locomotion::FrameToPosition(Frame coordBrazo){
	POSITION positionTarget;
	int i;

	for(i=0; i<3; i++){
		positionTarget.n[i] = coordBrazo.M(i, 0);
		positionTarget.o[i] = coordBrazo.M(i, 1);
		positionTarget.a[i] = coordBrazo.M(i, 2);
		positionTarget.p[i] = coordBrazo.p(i);
	}

	return positionTarget;
}

Frame locomotion::PositionToFrame(POSITION positionTarget){
	Frame coordBrazo;

	int i;

	for(i=0; i<3; i++){
		coordBrazo.M(i, 0) = positionTarget.n[i];
		coordBrazo.M(i, 1) = positionTarget.o[i];
		coordBrazo.M(i, 2) = positionTarget.a[i];
		coordBrazo.p(i) = positionTarget.p[i];
	}
	return coordBrazo;

}

/* Funciones para imprimir por pantalla */
void locomotion::print(){
	print(position);
	print(servos);
}

void locomotion::print(POSITION pos){
	int i;

	for (i = 0; i<3; i++)
	{
		cout << setprecision(PRECISION) << pos.n[i]  << setw(WIDTH) << setprecision(PRECISION) << pos.o[i] << setw(WIDTH) << setprecision(PRECISION) << pos.a[i] << setw(WIDTH) << setprecision(PRECISION) <<pos.p[i] << endl;
	}
	cout << setw(WIDTH)  << 0 << setw(WIDTH) << 0 << setw(WIDTH) << 0 << setw(WIDTH) << 1 << endl << endl;

}

void locomotion::print(SERVOS ser){
	int i;

	for (i = 0; i<ser.num_servos; i++)
	{
		cout << ser.q[i] << endl;
	}
}


void locomotion::log(){
    string initFileName = "log4cpp.properties";
    log4cpp::PropertyConfigurator::configure(initFileName);

    root = &log4cpp::Category::getRoot();
    sub1 = &log4cpp::Category::getInstance(std::string("sub1"));

}

/* Check errors */

int locomotion::checkError(void){
	/**
	 * Error Table:
	 * error =  0: OK
	 * error = -1: Inverse Kinematics fail.
	 * error = -2: Fordward Kinematics fail.
	 * error = -3: Struct POSITION is not correct.
	 * error = -4: Struct SERVOS is not correct.
	 * error = -5: Struct ANGLEROTATE is not correct.
	 *
	 * @return void.
	 */

	return error;
}

int locomotion::checkPosition(POSITION pos){
	int retError = 0;
	int i;

	for(i = 0; i<3; i++){
		if(abs(pos.n[i])>1){
			error = -3;
			sub1->error("LOC: POSITION is not correct. The 'n' value (%1.3f) must be between -1 and 1.", pos.n[i]);
			retError = -1;
		}
		if(abs(pos.o[i])>1){
			error = -3;
			sub1->error("LOC: POSITION is not correct. The 'o' value (%1.3f) must be between -1 and 1.", pos.o[i]);
			retError = -1;
		}
		if(abs(pos.a[i])>1){
			error = -3;
			sub1->error("LOC: POSITION is not correct. The 'a' value (%1.3f) must be between -1 and 1.", pos.a[i]);
			retError = -1;
		}
	}

	return retError;
}

int locomotion::checkServos(SERVOS serv){
	int retError = 0;
	int i;

	if(serv.num_servos<0 || serv.num_servos>chain.getNrOfJoints()){
		error = -4;
		sub1->error("LOC: SERVOS are not correct. The number of servos (%d) must be between 0 and %d.", serv.num_servos, chain.getNrOfJoints());
		retError = -1;
	}

	for(i=0; i<serv.num_servos; i++){
		if(serv.q[i]<0 || serv.q[i]>M_PI){
			error = -4;
			sub1->error("LOC: SERVOS are not correct. The %d servo value (%1.3f) must be between 0 and M_PI.", i, serv.q[i]);
			retError = i;
		}
	}


	return retError;
}

int locomotion::checkAngle(ANGLEROTATE ang){
	int retError = 0;

	if(ang.axis<0 || ang.axis>2){
		error = -5;
		sub1->error("LOC: ANGLEROTATE is not correct. The axis value (%d) must be between 0 and 2.", ang.axis);
		retError = -1;
	}

	if(abs(ang.angle)>M_PI/2){
		error = -5;
		sub1->error("LOC: ANGLEROTATE is not correct. The angle value (%1.3f) must be between -M_PI/2 and M_PI/2 radians.", ang.angle);
		retError = -1;
	}

	return retError;
}
