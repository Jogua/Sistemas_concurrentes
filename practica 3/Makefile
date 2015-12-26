default: productor-consumidor filosofos filosofos-camarero

all: default documentacion

# ************ Ejecutables ******************

productor-consumidor: productor-consumidor.cpp
	mpicxx -o productor-consumidor productor-consumidor.cpp

filosofos: filosofos.cpp
	mpicxx -o filosofos filosofos.cpp

filosofos-camarero: filosofos-camarero.cpp
	mpicxx -o filosofos-camarero filosofos-camarero.cpp

# ************ Ejecución ******************

ejecutar-productor-consumidor: productor-consumidor
	mpirun -np 10 productor-consumidor

ejecutar-filosofos: filosofos
	mpirun -np 10 filosofos

ejecutar-camarero: filosofos-camarero
	mpirun -np 11 filosofos-camarero

# ************ Generación de documentación ******************
documentacion:
	doxygen doc/doxys/Doxyfile

# ************ Limpieza ************
clean :
	rm productor-consumidor filosofos filosofos-camarero ./doc/html/*
