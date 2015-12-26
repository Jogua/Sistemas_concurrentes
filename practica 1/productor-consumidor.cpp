#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>//funcion atoi

using namespace std;

int num_items;	//numero de items que se van a producir y consumir. Esta cantidad se pasa por parametros.
const int max_vector=10;
int v[max_vector]; //vector que almacena los items producidos
sem_t puede_leer;
sem_t puede_escribir;

int producir_dato()
{
	static int contador = 1;	//Solo se declara y se inicializa una vez.
	return contador ++;
}

void consumir_dato (int dato)
{
	cout << "Dato recibido: " << dato << endl;
}

void * productor (void *)
{
	int pos=0; //almacena la siguiente posicion que tiene que producir.
	for(unsigned i = 0; i < num_items; i++){
		//Produce el dato, comprueba si puede almacenarlo en el vector. Cuando pueda lo almacena y le dice al consumidor que ese dato puede ser leido. Por último, hace un reajuste de la siguiente posición.
		int dato=producir_dato();
		sem_wait ( &puede_escribir );
		v[pos]=dato;
		sem_post( &puede_leer );
		pos++;
		if(pos==max_vector){
			pos=0;
		}
	}
	return NULL;
}

void * consumidor (void *)
{
	int pos=0;	//almacena la siguiente posicion que tiene que consumir.
	for(unsigned i = 0; i < num_items; i++){
		//Espera a que pueda leer un dato. Cuando pueda, lo lee y lo almacena en la variable local dato, y le dice al productor que esa posicion puede volver a ser utilizada. Seguidamente, consume el dato y hace un reajuste de la siguiente posición a consumir.
		int dato;
		sem_wait ( &puede_leer );
		dato=v[pos];
		sem_post ( &puede_escribir );
		consumir_dato(dato);
		pos++;
		if(pos==max_vector){
			pos=0;
		}
	}
	return NULL;
}

int main (int argc, char * argv[])
{
	/* Se tiene que introducir un parametro, el cual corresponde al numero de items que se quieren producir, por tanto hay que comprobar si argc es distinto de 2, ya que el primero es el nombre del programa y el segundo el el numero de items.
	*/
	if(argc !=2){
		cerr << "Se debe introducir un parametro. El parametro dice cuantos items se van a producir." << endl;
		return 0;
	}
	num_items=atoi(argv[1]);

	//Esta inicialización se debe a que no tiene sentido de que se pueda leer sin que antes se haya escrito.
	sem_init (&puede_leer,0,0);
	//Esta inicialización se debe a que se puede escribir como maximo max_vector veces, en este caso es 10.
	sem_init (&puede_escribir,0,max_vector);

	//Declaración y creación de las hebras.
	pthread_t consumidora;
	pthread_t productora;

	pthread_create(&consumidora,NULL,consumidor,NULL);
	pthread_create(&productora,NULL, productor, NULL);

	//Se espera a las hebras anteriormente creadas.
	pthread_join(consumidora,NULL);
	pthread_join(productora,NULL);

	cout << "     Fin" << endl;

	//Con estas sentencias se va a eliminar la memoria asignada a los semaforos.
	sem_destroy( &puede_leer );
	sem_destroy( &puede_escribir );
}
