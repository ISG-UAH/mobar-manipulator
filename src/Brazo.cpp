#include "Brazo.h"
Brazo::Brazo()
{

}
Brazo::Brazo(char* filename){
	Config(filename);
}

int Brazo::Config(char* filename)
{
	RobotDH robotJoints(filename);
	int ret;

	if(robotJoints.check()==0){

		BRAZO_SERVOS servos_init;
		double min[5], max[5];
		unsigned int i;
		char command[100];

		// Se define la caracterizacion cinematica del robot:
		//chain.addSegment(Segment(Joint(Joint::RotZ),Frame::DH(double a, double alpha, double d, double theta)));

		for(int count=0; count < robotJoints.getDof(); count++){
			chain.addSegment(Segment(Joint(Joint::RotZ),Frame::DH(robotJoints.getOneJoint(count).a,
					robotJoints.getOneJoint(count).alpha, robotJoints.getOneJoint(count).d, robotJoints.getOneJoint(count).theta)));
		}

		// Tupla con las posiciones de los servos actuales
		 qServos(chain.getNrOfJoints());
		 qMin(chain.getNrOfJoints());
		 qMax(chain.getNrOfJoints());

		 // Inicializamos los valores de qServos

		 for(i=0; i<chain.getNrOfJoints(); i++){
				 servos_init.q[i] = M_PI/2;
				 min[i]=0;
				 max[i]=M_PI;
		 }
		 servos_init.qPinza = M_PI/2;
		 qServos = ServosToJnt(servos_init);


		 qMin = QLimites(min);
		 qMax = QLimites(max);

		 // Aplicación de la cinematica directa
		ChainFkSolverPos_recursive fkine(chain);

		fkine.JntToCart(qServos, coordBrazo);

		print();

		//ret = initArduino();

		std::cout << "Inicializacion Ok. ret = " << ret << std::endl;

		strcpy(command, "MoveTo: ");
		//sendCommand(command);
	}
	return ret;

}

Brazo::~Brazo()
{
	int ret;
	ret = serial_close(serial_fd);

	std::cout << "Destructor de Brazo. Ret: " << ret << std::endl;
}


/* Inicializar Arduino por puerto Serial */
int Brazo::initArduino(){
	char *puerto;
	int ret;

	strcpy(puerto, "/dev/ttyUSB0");
	ret = initArduino(puerto);
	return ret;
}

int Brazo::initArduino(char *portArduino){
	int ret=1;
	char data_rcv[1024];
	timeval tim;
	double t_init, t_end;

	serial_fd=serial_open(portArduino,B9600);

	//-- Error checking
	if (serial_fd==-1) {
		std::cout << "Error al conectar con Arduino " << portArduino << std::endl;
		perror("OPEN");
		exit(0);
	}

	 gettimeofday(&tim, NULL);
	 t_init =tim.tv_sec+(tim.tv_usec/1000000.0);

	 do{
		 ret = serial_read(serial_fd, data_rcv, 1024, TIMEOUT);
		 gettimeofday(&tim, NULL);
		 t_end =tim.tv_sec+(tim.tv_usec/1000000.0);
		 if(t_end - t_init > TIMEOUT/100000.0){
			 std::cout << "Cerrando el puerto " << serial_fd << " por TIMEOUT" << std::endl;
			 return -1;
		 }
	 }while(ret == 0);



	if (ret == 0){
		std::cout << "Timeout (" << TIMEOUT << ")." << std::endl;
	}else{
		std::cout << "Port Arduino("<< serial_fd <<"): " << portArduino << std::endl << "Arduino says (" << ret << "): " << data_rcv << std::endl;
	}
	return ret;
}

int Brazo::Move(double posicion[3]){
	Frame Tfinal;
	char command[100];
	int ret=0;
	double p[3];

	// Calculo de las coordenadas nuevas de Tobj
	for(int i=0; i<3; i++){
		p[i] = coordBrazo.M(i,0)*posicion[i] + coordBrazo.M(i,1)*posicion[i] + coordBrazo.M(i,2)*posicion[i] + coordBrazo.p(i);
	}

	Tfinal.M = coordBrazo.M;
	Tfinal.p = Vector(p[0], p[1], p[2]);

	coordBrazo = Tfinal;
	InvKine();

	strcpy(command, "MoveTo: ");
	ret = sendCommand(command);
	return ret;

}

int Brazo::Clamp(double apertura){
	char command[100];
	int ret;

	qPinza = apertura;

	strcpy(command, "MoveServo: 5 ");
	ret = sendCommand(command);
	return ret;

}

int Brazo::Rotate(int eje, double angulo){
	int ret=0;
	char command[100];

	// Eje 0 (alpha), 1 (theta), 2 (psi)

	switch(eje){
	case 0: // Eje alpha
		coordBrazo.M.DoRotX(angulo);
		break;
	case 1: // Eje theta
		coordBrazo.M.DoRotY(angulo);
		break;
	case 2: // Eje psi
		coordBrazo.M.DoRotZ(angulo);
		break;
	}

	InvKine();

	strcpy(command, "MoveTo: ");
	ret = sendCommand(command);
	return ret;
}

BRAZO_SERVOS Brazo::GetPosition(void){
	char command[100];

	strcpy(command, "GetPos");
	sendCommand(command);
	return JntToServos();

}

int Brazo::InvKine(){
	JntArray qFinal(chain.getNrOfJoints());
	Eigen::VectorXd weight_ts(6);
	double weights[6];
	int i, ret;

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
	cinematicaInversaV.setWeightTS(weight_ts.asDiagonal());

	ChainIkSolverPos_NR_JL_WE cinematicaInversa(chain, qMin, qMax, cinematicaDirecta, cinematicaInversaV, 100, 1e-6);
	cinematicaInversa.setWeights(weights);

	ret = cinematicaInversa.CartToJnt(qServos, coordBrazo, qFinal);

	if(ret >= 0){
		qServos = qFinal;
	}else{
		/* Error en el calculo de la cinematica inversa */
		cout << "Error calculo cinematica inversa: " << ret << endl;
	}

	return ret;
}

/* Velocidad */
int Brazo::SetVel(BRAZO_VELOCITY speed){
	int ret=0;
	char command[100];
	double newSpeed = ConvertSpeed(speed);

	if(newSpeed >0 && newSpeed <= MAX_SPEED){

		strcpy(command, "SetVel: ");
		ret = sendCommand(command);
		return ret;
	} else {
		return -1;
	}
}

BRAZO_VELOCITY Brazo::GetVel(void){
	int ret=0;
	char command[100];

	strcpy(command, "GetVel");
	ret = sendCommand(command);
	return ConvertSpeed(speed);
}

double Brazo::ConvertSpeed(BRAZO_VELOCITY vel){
	double convertValues[4]={8, 6, 4, 2};

	if(vel == FAST)
		return convertValues[0];
	else if (vel == NORMAL)
		return convertValues[1];
	else if (vel == SLOW)
		return convertValues[2];
	else if (vel == VERYSLOW)
		return convertValues[3];
	else
		return convertValues[1];
}

BRAZO_VELOCITY Brazo::ConvertSpeed(double vel){
	double convertValues[4]={8, 6, 4, 2};

	if(vel == convertValues[0])
		return FAST;
	else if(vel == convertValues[1])
		return NORMAL;
	else if(vel == convertValues[2])
		return SLOW;
	else if(vel == convertValues[0])
		return VERYSLOW;
	else
		return NORMAL;
}


/* Comunicacion con placa */
int Brazo::sendCommand(char command[100]){
	int return_needed = 0, ret;
	BRAZO_SERVOS qServosRead;
	char aux[10];
	char *data_rcv, data_snd[1024], *command_rcv;
	string data(command);

	if (data == "MoveTo: "){
		for (unsigned int count = 0; count < chain.getNrOfJoints(); count++){
				sprintf(aux, "%1.4f ", qServos(count));
				data += aux;
			}
		// qPinza
		sprintf(aux, "%1.4f;", qPinza);
		data += aux;

	} else if (data == "MoveServo: 5 "){
		// qPinza
		sprintf(aux, "%1.4f;", qPinza);
		data += aux;
	} else if (data == "SetVel: "){
		sprintf(aux, "%2.2f;", speed);
		data += aux;
	} else if (data == "GetVel" || data == "GetPos"){
		sprintf(aux, ": ;");
		data += aux;
		return_needed = 1;
	} else { //default
		return 0;
	}

	strcpy(data_snd, data.c_str());

	serial_send(serial_fd, data_snd, strlen(data_snd));
	serial_read(serial_fd, data_rcv, 1024, TIMEOUT);

	if (return_needed){
		command_rcv = strtok(data_rcv, ": ");

		if (command_rcv == "GetVel"){

			speed = atof(strtok(NULL, " "));

		} else if (command_rcv == "GetPos"){

			for (ret = 0; ret < 6; ret++){
				qServosRead.q[ret] = atof(strtok(NULL, " "));
			}

			qServos = ServosToJnt(qServosRead);
			return 6;
		}
	}
	return 0;

}


/* Conversion de coordenadas */
JntArray Brazo::ServosToJnt(BRAZO_SERVOS qInt){
	JntArray qRet(chain.getNrOfJoints());
	int i;

	for (i = 0; i < chain.getNrOfJoints(); i++){
		qRet(i) = qInt.q[i];
	}

	qPinza = qInt.qPinza;

	return qRet;
}

BRAZO_SERVOS Brazo::JntToServos(void){
	BRAZO_SERVOS qRet;
	int i;

	for (i = 0; i < chain.getNrOfJoints(); i++){
		qRet.q[i] = qServos(i);
	}

	qRet.qPinza = qPinza;

	return qRet;
}

JntArray Brazo::QLimites(double limites[6]){
	JntArray qLim(chain.getNrOfJoints());

	for(int i= 0; i<chain.getNrOfJoints(); i++)
		qLim(i) = limites[i];

	return qLim;
}

BRAZO_COORD Brazo::FrameToCoord(void){
	BRAZO_COORD position;
	int i;

	for(i=0; i<3; i++){
		position.n[i] = coordBrazo.M(i, 0);
		position.o[i] = coordBrazo.M(i, 1);
		position.a[i] = coordBrazo.M(i, 2);
		position.p[i] = coordBrazo.p(i);
	}

	return position;
}

/* Consulta de variables */
BRAZO_SERVOS Brazo::GetServos(void){
	return JntToServos();

}
BRAZO_COORD Brazo::GetCoord(void){
	return FrameToCoord();
}



/* Funciones para imprimir por pantalla */
void Brazo::print(){
	print(coordBrazo);
	print(qServos);
}

void Brazo::print(Frame coordenadas){
	int separacion=15, count;

	cout << setw(separacion)  << 'n' << setw(separacion) << 'o' << setw(separacion) << 'a' << setw(separacion) << 'p' << endl << endl;
	for(count=0; count<3; count++){
		cout << setw(separacion)  << coordenadas.M(count,0) << setw(separacion) << coordenadas.M(count,1) << setw(separacion) << coordenadas.M(count,2) << setw(separacion) << coordenadas.p(count) << endl;
	}
	cout << setw(separacion)  << 0 << setw(separacion) << 0 << setw(separacion) << 0 << setw(separacion) << 1 << endl << endl;

}

void Brazo::print(JntArray servos){
	int separacion=15, count;

	for (count = 0; count < chain.getNrOfJoints()+1; count++){
		cout << setw(separacion)  << "q" << count+1;
	}
	cout << endl;

	for (count = 0; count < chain.getNrOfJoints(); count++){
		cout << setw(separacion)  << servos(count);
	}
	cout << setw(separacion)  << qPinza;
	cout << endl;
}
