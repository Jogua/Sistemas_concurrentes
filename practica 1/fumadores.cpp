#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h> //time()
#include <unistd.h> //sleep()
#include <stdlib.h> //rand()

using namespace std;

//Indica si el estanco esta abierto. El estanco cerrara cuando haya alzanzado produccion.
bool estanco_abierto=true;
//es el numero maximo de productos que va a vender el estanquero. Cuando el estanquero produzca esta cantidad, él cerrara el estanco. Esta variable sera introducida por el usuario desde la entrada estandar.
int produccion;

//controla si el estanquero puede producir, es decir, si el fumador correspondiente ha cogido los ingredientes que le faltaban.
sem_t producir;
//controla si el fumador que tiene papel puede fumar.
sem_t f_papel;
//controla si el fumador que tiene tabaco puede fumar.
sem_t f_tabaco;
//controla si el fumador que tiene cerillas puede fumar.
sem_t f_cerillas;

enum Ingrediente { papel, tabaco, cerillas };

void fumar ()
{
	//simula el momento de fumar, va de 1 a 5 seg.
	sleep(1+rand()%5);
}

void * estanco (void *)
{
	int producidos=0;
	while (estanco_abierto){

		sem_wait(&producir);
		
		int ingr1=rand()%3;	//asigna a ingr1 un ingrediente aleatorio.
		int ingr2;
		do{
			ingr2=rand()%3;	//asigna a ingr2 un ingrediente aleatorio, con la condicion que sea distinto a ingr1.
		}while(ingr1==ingr2);

		if (ingr1==papel && ingr2==tabaco || ingr1==tabaco && ingr2==papel){
			cout << "\nEl estanquero pone en el mostrador papel y tabaco." << endl;
			sem_post(&f_cerillas);	//fuma el que tenga cerillas
		}
		else{
			if (ingr1==papel && ingr2==cerillas || ingr1==cerillas && ingr2==papel){
			cout << "\nEl estanquero pone en el mostrador papel y cerillas." << endl;
				sem_post(&f_tabaco);	//fuma el que tenga tabaco
			}
			else{
			cout << "\nEl estanquero pone en el mostrador cerillas y tabaco." << endl;
				sem_post(&f_papel);		//fuma el que tenga papel
			}
		}

		producidos++;	//aumentamos la variable producidos
		//si el estanquero ha producido lo mismo que lo que introdujo el usuario. El estanquero despierta a todos los fumadores para que ninguno se quede en una espera infinita.
		if (producidos==produccion){
			sem_wait(&producir);
			estanco_abierto=false;
			sem_post(&f_papel);
			sem_post(&f_tabaco);
			sem_post(&f_cerillas);
		}
	}
}

void * funcion_fumadores (void * i)
{
	//El ingrediente que tiene cada fumador lo pasamos por parametro, de tipo void, y para usarlo, lo pasamos a unsigned long.
	unsigned long ingr=(unsigned long) i;
	while (estanco_abierto){
		
		if (ingr==papel){	//fuma el q tenga papel
			sem_wait(&f_papel);
			//comprueba si el estanco esta abierto. Si esta cerrado no fuma.
			if (!estanco_abierto)
				return 0;
			cout << "El fumador 1 tiene papel, por lo que se lia el cigarro." << endl;
		}
		else{
			if (ingr==tabaco){	//fuma el q tenga tabaco
				sem_wait(&f_tabaco);
				//comprueba si el estanco esta abierto. Si esta cerrado no fuma.
				if (!estanco_abierto)
					return 0;
				cout << "El fumador 2 tiene tabaco, por lo que se lia el cigarro." << endl;
			}
			else{ //fuma el q tenga cerillas
				sem_wait(&f_cerillas);
				//comprueba si el estanco esta abierto. Si esta cerrado no fuma.
				if (!estanco_abierto)
					return 0;
				cout << "El fumador 3 tiene cerillas, por lo que se lia el cigarro." << endl;
			}
		}
		cout << "El fumador " << ingr+1 << " comienza a fumar."<<endl;
		sem_post(&producir);
		fumar();
		cout << "El fumador " << ingr+1 << " ha terminado de fumar." << endl;
	}
}


int main()
{
	cout << "Introduce la producción: ";
	cin >> produccion;

	srand (time(NULL));	//Se reinicia la semilla del random.

	//La siguiente inicialización de semaforos se debe a que los tres fumadores no podran fumar hasta que el estanquero no se lo diga, por tanto los tres estan inicializados a 0. Sin embargo, el semaforo producir hace referencia al estanquero. Éste está inicializado a 1 para que pueda empezar a generar productos.
	sem_init(&producir,0,1);
	sem_init(&f_papel,0,0);
	sem_init(&f_tabaco,0,0);
	sem_init(&f_cerillas,0,0);

	//Definición y creación de las hebras.
	pthread_t estanquero;
	pthread_t fumadores[3];

	pthread_create (&estanquero,NULL,estanco,NULL);
	for (unsigned long i=0; i<3; i++)
		pthread_create (&fumadores[i],NULL,funcion_fumadores, (void *) i);

	//Se espera a la hebra estanquero, cuando esta vuelve, es porque ya ha cerrado, por lo tanto, lo indicamos.
	pthread_join(estanquero,NULL);
	cout << "\nEl estanco ha cerrado."<<endl;

	//se espera a los fumadores, para asegurarnos de que terminan de fumar.
	for (unsigned long i=0; i<3; i++)
		pthread_join(fumadores[i],NULL);

	//Con estas sentencias se va a eliminar la memoria asignada a los semaforos.
	sem_destroy(&producir);
	sem_destroy(&f_papel);
	sem_destroy(&f_tabaco);
	sem_destroy(&f_cerillas);
}
