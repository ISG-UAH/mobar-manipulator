#include "RobotDH.h"

RobotDH::RobotDH()
{
	num_error = 0;
	ReadConfigFile("input");
}

RobotDH::RobotDH(const char* filename)
{
	num_error = 0;
	ReadConfigFile(filename);
}

RobotDH::~RobotDH()
{
	delete[] juntaRobot;
}

int RobotDH::ReadConfigFile(const char* filename)
{
	int size = 20, count;
	char cadena[size];

	// Apertura del fichero de configuracion
	ifstream fs(filename);

	if(fs.good()){
		// Numero de grados de libertad
		fs.getline(cadena, size, ':');
		fs.getline(cadena, size);
		dof = atoi(cadena);

		juntaRobot = new ROBOT_JOINT[dof];

		for(count = 0; count < dof; count++){
			fs.getline(cadena, size, ':');
			fs.getline(cadena, size);
			juntaRobot[count].numjoint = atof(cadena);

			// Parametro a
			fs.getline(cadena, size, ':');
			fs.getline(cadena, size);
			juntaRobot[count].a = atof(cadena);

			// Parametro alpha
			fs.getline(cadena, size, ':');
			fs.getline(cadena, size);
			juntaRobot[count].alpha = atof(cadena);

			// Parametro d
			fs.getline(cadena, size, ':');
			fs.getline(cadena, size);
			juntaRobot[count].d = atof(cadena);

			// Parametro theta
			fs.getline(cadena, size, ':');
			fs.getline(cadena, size);
			juntaRobot[count].theta = atof(cadena);
		}
		fs.close();
		num_error = 0;
		return dof;
	}else{
		perror("Error al crear el archivo. \n");
		num_error = 1;
		return -1;
	}

}

int RobotDH::NewConfigFile()
{
	int numdof, count, ok=0;
	const char* filename;
	double a, alpha, d, theta;
	string input;

	cout << "Nombre del archivo de configuracion: ";
	getline (cin, input);
	filename = input.c_str();
	ofstream fs(filename);

	if(fs.good()){
		cout << "Archivo " << filename <<" creado correctamente. " << endl;

		do{
			cout << "Numero de grados de libertad: ";
			getline (cin, input);
			stringstream(input) >> numdof;

			if(numdof > 0){
				ok = 1;
				fs <<"dof: " << numdof << endl;
			}else{
				ok = 0;
				cout << "El numero de grados de libertad ha de ser un numero positivo." << endl;
			}

		}while(ok!=1);

		for(count = 0; count < numdof; count++){
			cout << "Parametros de la junta " << count+1 << endl;
			fs <<"JOINT: " << count+1 << endl;

			do{
				// Parametro a
				cout << "a: ";
				getline (cin, input);
				stringstream(input) >> a;

				if(a > 0){
					ok = 1;
					fs <<"a: " << a << endl;
				}else{
					ok = 0;
					num_error = 2;
					cout << "El valor de 'a', ha de ser un numero positivo." << endl;
				}

			}while(ok!=1);

			do{
				// Parametro alpha
				cout << "alpha: ";
				getline (cin, input);
				stringstream(input) >> alpha;

				if(alpha > 0){
					ok = 1;
					fs <<"alpha: " << alpha << endl;
				}else{
					ok = 0;
					num_error = 3;
					cout << "El valor de 'alpha', ha de ser un numero positivo." << endl;
				}

			}while(ok!=1);

			do{
				// Parametro d
				cout << "d: ";
				getline (cin, input);
				stringstream(input) >> d;


				if(d > 0){
					ok = 1;
					fs <<"d: " << d << endl;
				}else{
					ok = 0;
					num_error = 4;
					cout << "El valor de 'd', ha de ser un numero positivo." << endl;
				}

			}while(ok!=1);

			do{
				// Parametro theta
				cout << "theta: ";
				getline (cin, input);
				stringstream(input) >> theta;

				if(theta > 0){
					ok = 1;
					fs <<"theta: " << theta << endl;
				}else{
					ok = 0;
					num_error = 5;
					cout << "El valor de 'theta', ha de ser un numero positivo." << endl;
				}

			}while(ok!=1);

		}


	}else{
		num_error = 1;
		perror("Error al crear el archivo. \n");
	}

	fs.close();
	return 0;
}


void RobotDH::PrintJoints(){

	cout << "PRINT JOINTS" << endl;
	int separacion=15;

	cout << setw(separacion)  << "j" << setw(separacion) << "theta" << setw(separacion) << "d" << setw(separacion) << "a" << setw(separacion) << "alpha" << endl;

	for(int count = 0; count < dof; count++){
		cout << setw(separacion)  << juntaRobot[count].numjoint << setw(separacion) << juntaRobot[count].theta << setw(separacion) << juntaRobot[count].d << setw(separacion) << juntaRobot[count].a << setw(separacion) << juntaRobot[count].alpha << endl;
	}
}

int RobotDH::getDof(){
	return dof;
}

int RobotDH::check(){

	/* Tipo de errores
	 *
	 * 0 - Todo correcto
	 * 1 - Error al leer el archivo
	 * 2 - Fallo en parametro a
	 * 3 - Fallo en parametro alpha
	 * 4 - Fallo en parametro d
	 * 5 - Fallo en parametro theta
	 *
	 */
	return num_error;
}

ROBOT_JOINT * RobotDH::getJoints(){
	return juntaRobot;
}

ROBOT_JOINT RobotDH::getOneJoint(int joint){
	return juntaRobot[joint];
}
