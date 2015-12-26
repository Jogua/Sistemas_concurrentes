/**
  *	@file filosofos.cpp
  *
  * @brief Código necesario para poder resolver el problema de los filosofos comensales con MPI.
  *
  * @author José Guadix
  */

#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define Quiero_Coger 0 ///<Tag utilizado para saber cuando un filósofo quiere coger un tenedor.
#define Quiero_Soltar 1 ///<Tag utilizado para saber cuando un filósofo quiere soltar un tenedor.

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
  *	@brief Esta función simula al filófoso comensal, realizando para ello todos los pasos necesarios.
  *
  * @param id Es la identificación del filósofo.
  * @param nprocesos Es el número de procesos que se estan ejecutando.
  *
  *	\note Para resolver el posible interbloqueo hemos decidido que el filósofo con id=0 invierta el orden de coger los tenedores.
  */
void Filosofo (int id, int nprocesos){
	int tenedorIzq = (id+1)%nprocesos;
	int tenedorDer = (id+nprocesos-1)%nprocesos;
	int peticion=1;

	while(true){

		if(id==0){
			MPI_Ssend (&peticion, 1, MPI_INT, tenedorDer, Quiero_Coger, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " ha cogido el tenedor " << tenedorDer << endl << flush;

			MPI_Ssend (&peticion, 1, MPI_INT, tenedorIzq, Quiero_Coger, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " ha cogido el tenedor " << tenedorIzq << endl << flush;
		}
		else{
			MPI_Ssend (&peticion, 1, MPI_INT, tenedorIzq, Quiero_Coger, MPI_COMM_WORLD);
			cout << "El filósofo " << id << " ha cogido el tenedor " << tenedorIzq << endl << flush;

			MPI_Ssend (&peticion, 1, MPI_INT, tenedorDer, Quiero_Coger, MPI_COMM_WORLD);
			cout << "El filósofo " << id << " ha cogido el tenedor " << tenedorDer << endl << flush;

		}

		Comer(id);

		MPI_Ssend (&peticion, 1, MPI_INT, tenedorIzq, Quiero_Soltar, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " ha soltado el tenedor " << tenedorIzq << endl << flush;

		MPI_Ssend (&peticion, 1, MPI_INT, tenedorDer, Quiero_Soltar, MPI_COMM_WORLD);
		cout << "El filósofo " << id << " ha soltado el tenedor " << tenedorDer << endl << flush;

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
	if(size!=10){
		if (rank==0){
			cout << "El número de procesos debe ser 10. 5 filósofos y 5 tenedores." << endl;
		}
		MPI_Finalize();
		return 0;
	}
	if(rank%2==0){
		Filosofo(rank,size);
	}else{
		Tenedor();
	}
	MPI_Finalize();
	return 0;
}
