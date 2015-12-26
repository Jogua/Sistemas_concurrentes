/**
  *	@file filosofos-camarero.cpp
  *
  * @brief Código necesario para poder resolver el problema de los filosofos comensales con la presencia de un camarero, utilizando MPI.
  */

#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define camarero 10  ///<Usado para conocer la identificación del camarero.
#define Quiero_Coger 0 ///<Tag utilizado para saber cuando un filósofo quiere coger un tenedor.
#define Quiero_Soltar 1 ///<Tag utilizado para saber cuando un filósofo quiere soltar un tenedor.
#define Quiero_Sentarme 2 ///<Tag utilizado para saber cuando un filósofo quiere ocupar un sitio de la mesa.
#define Quiero_Levantarme 3 ///<Tag utilizado para saber cuando un filósofo quiere dejar libre un sitio de la mesa.

using namespace std;

/**
  *	@brief Esta función simula el tiempo que el filófoso pasa pensando.
  *
  * @param id Es la identificación del filósofo.
  *
  *	\note El tiempo que se simula está entre 1 y 7 segundos.
  */
void Pensar(int id){
	cout << "El filósofo " << id << " está pensando." << endl << flush;
	int time = 1+rand()%7;
	sleep(time);
}

/**
  *	@brief Esta función simula el tiempo que el filófoso pasa comiendo.
  *
  * @param id Es la identificación del filósofo.
  *
  *	\note El tiempo que se simula está entre 1 y 5 segundos.
  */
void Comer(int id){
	cout << "El filósofo " << id << " está comiendo." << endl << flush;
	int time = 1+rand()%5;
	sleep(time);
}


/**
  *	@brief Esta función simula al camarero, realizando para ello todos los pasos necesarios.
  *
  *	\note Para resolver el posible interbloqueo hemos decidido tener un proceso que controle el número de filósofos que pueden estar sentados a la vez, que es 4. Si sólo hay 4 filosofos sentados en la mesa, no se da posibilidad de interbloqueo.
  */
void Camarero (){
	int plazasLibres=4;
	int peticion, puedeSentarse=1;
	MPI_Status status;

	while(true){
		MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
		if(status.MPI_TAG==Quiero_Sentarme && plazasLibres){
			MPI_Recv(&peticion,1,MPI_INT, MPI_ANY_SOURCE, Quiero_Sentarme, MPI_COMM_WORLD, &status);
			plazasLibres--;
		}
		else{
			MPI_Recv(&peticion,1,MPI_INT, MPI_ANY_SOURCE, Quiero_Levantarme, MPI_COMM_WORLD, &status);
			plazasLibres++;
		}
	}
}

/**
  *	@brief Esta función simula al filófoso comensal, realizando para ello todos los pasos necesarios.
  *
  * @param id Es la identificación del filósofo.
  * @param nprocesos Es el número de procesos que se estan ejecutando.
  */
void Filosofo (int id, int nprocesos){
	int tenedorIzq = (id+1)%nprocesos;
	int tenedorDer = (id+nprocesos-1)%nprocesos;
	int peticion=1;
	int puedoSentarme;
	MPI_Status status;
	id=id/2;
	while(true){
		cout << "El filósofo " << id << " quiere sentarse." << endl;
		MPI_Ssend (&peticion, 1, MPI_INT, camarero, Quiero_Sentarme, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " se ha sentado." << endl;

		MPI_Ssend (&peticion, 1, MPI_INT, tenedorIzq, Quiero_Coger, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " ha cogido el tenedor izquierdo."<< endl << flush;

		MPI_Ssend (&peticion, 1, MPI_INT, tenedorDer, Quiero_Coger, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " ha cogido el tenedor derecho." << endl << flush;

		Comer(id);

		MPI_Ssend (&peticion, 1, MPI_INT, tenedorIzq, Quiero_Soltar, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " ha soltado el tenedor izquierdo." << endl << flush;

		MPI_Ssend (&peticion, 1, MPI_INT, tenedorDer, Quiero_Soltar, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " ha soltado el tenedor derecho." << endl << flush;

		MPI_Ssend (&peticion, 1, MPI_INT, camarero, Quiero_Levantarme, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " se ha levantado." << endl;
		Pensar(id);
	}
}

/**
  *	@brief Esta función simula a un tenedor.
  *
  *	\note El tenedor sólo recibe una petición de un filósofo para solicitar de que quiere cogerlo, y otra petición para solicitarle que quiere soltarlo.
  */
void Tenedor (){
	int peticion;
	MPI_Status status;
	while(true){
		MPI_Recv(&peticion,1,MPI_INT, MPI_ANY_SOURCE, Quiero_Coger, MPI_COMM_WORLD, &status);
		MPI_Recv(&peticion,1,MPI_INT, MPI_ANY_SOURCE, Quiero_Soltar, MPI_COMM_WORLD, &status);
	}
}

int main(int argc, char * argv[]){
	int rank, size;
	srand(time(0));
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size!=11){
		if (rank==0){
			cout << "El número de procesos debe ser 11. 5 filósofos, 5 tenedores y 1 camarero." << endl;
		}
		MPI_Finalize();
		return 0;
	}
	if(rank==camarero){
		Camarero();
	}else if (rank%2==0){
		Filosofo(rank,size-1);
	}else{
		Tenedor();
	}
	MPI_Finalize();
	return 0;
}
