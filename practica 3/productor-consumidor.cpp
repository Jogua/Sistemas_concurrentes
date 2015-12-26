/**
  *	@file productor-consumidor.cpp
  *
  * @brief Código necesario para poder resolver el problema del productor-consumidor, utilizando MPI.
  */

#include "mpi.h"
#include <iostream>
#include <cmath>

#define Tag_Productor 1 ///<Tag utilizado para saber que el mensaje viene de un productor.
#define Tag_Consumidor 2 ///<Tag utilizado para saber que el mensaje viene de un consumidor.
#define ITERS 20 ///<Número de iteracciones que va a hacer cada productor.
#define TAM_BUFFER 5  ///<Tamaño del buffer donde se almacenan los datos producidos y que todavia no han sido consumidos.

int Buffer; ///<Usado para conocer la identificación del buffer.

using namespace std;

/**
  *	@brief Esta función simula al productor, realizando para ello todos los pasos necesarios.
  *
  *	\note Esta función la realizan todos los productores, y la repiten "ITERS" veces.
  */
void productor(){
	for (unsigned int i=0; i<ITERS; i++){
		MPI_Ssend (&i, 1, MPI_INT, Buffer, Tag_Productor, MPI_COMM_WORLD);
		cout << "Productor pruduce el valor "<<i<<endl<<flush;
	}
}

/**
  *	@brief Esta función simula al consumidor, realizando para ello todos los pasos necesarios.
  *
  *	@param iteraciones Es el número de iteraciones que tiene que hacer cada consumidor.
  *
  *	\note Esta función la realizan todos los consumidores, y la repiten "iteraciones" veces.
  */
void consumidor(int iteraciones){
	int value, peticion=1;
	float raiz;
	MPI_Status status;
	for (unsigned int i=0; i<iteraciones; i++){
		MPI_Ssend(&peticion, 1, MPI_INT, Buffer, Tag_Consumidor, MPI_COMM_WORLD);
		MPI_Recv(&value, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD, &status);
		cout << "Consumidor recibe valor " << value << " de Buffer " << endl << flush;
		raiz = sqrt(value);
	}
}

/**
  *	@brief Esta función simula al buffer, realizando para ello todos los pasos necesarios.
  *
  *	\note Para resolver el problema de una manera más eficiente, y que no sea que el productor le envia un dato al consumidor y espera a que lo reciba, creamos otra función para otro proceso, que se encargue de gestionar los datos que recibe del productor, y los envie al consumidor. Este proceso puede almacenar un máximo de "TAM_BUFFER" elementos.
  */
void buffer (){
	int value[TAM_BUFFER], peticion, pos=0, rama;
	MPI_Status status;
	for(unsigned int i=0; i<ITERS*2*Buffer; i++){
		if (pos==0){
			rama=0;
		}else if(pos==TAM_BUFFER){
			rama = 1;
		}else{
			MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			rama=((status.MPI_SOURCE < Buffer) ?0 :1);
		}

		switch(rama){
			case 0:
				MPI_Recv(&value[pos],1,MPI_INT, MPI_ANY_SOURCE, Tag_Productor, MPI_COMM_WORLD, &status);
				cout << "El buffer recibe " << value[pos] << " del productor." << endl << flush;
				pos++;
				break;
			case 1:
				MPI_Recv(&peticion,1,MPI_INT, MPI_ANY_SOURCE, Tag_Consumidor, MPI_COMM_WORLD, &status);
				MPI_Ssend(&value[pos-1], 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
				cout << "El buffer envia " << value[pos-1] << " al consumidor." << endl << flush;
				pos--;
				break;
		}
	}
}

int main(int argc, char * argv[]){
	int rank, size;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	Buffer = size/2;

	if (rank<Buffer){
		productor();
	}else if(rank==Buffer){
		buffer();
	}else{
		int iterConsumidor = ITERS*Buffer/(size-1-Buffer);
		consumidor(iterConsumidor);
	}
	MPI_Finalize();
}
