/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package barbero;

/**
 *
 * @author Jose
 */
public class BarberoMain {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        //Lectura y comprobación de los parametros.
        if (args.length != 2) {
            System.err.println("Hay que pasarle dos parametros: \n\t1. Numero de clientes.\n\t2. Numero de veces que se pela cada cliente.");
            return;
        }
        //Declaración de la instancia del monitor y las instancias de las clases que implementan Runnable
        Barberia barberia = new Barberia();
        Barbero barbero;
        Cliente cliente[];
        
        int numClientes =Integer.parseInt(args[0]);
        int numVeces=Integer.parseInt(args[1]);
        cliente = new Cliente[numClientes];
        
        //Creación y puesta en marcha de las hebras.
        barbero = new Barbero(barberia, numClientes*numVeces);
        for(int i=0; i<numClientes; i++){
                cliente[i]= new Cliente(barberia, i, numVeces);
                cliente[i].thr.start();
        }
        barbero.thr.start();
        
        //Se espera a que todos los clientes terminen de pelarse para acabar el programa.
        try {
            for(int i=0; i<numClientes; i++){
                cliente[i].thr.join();
            }
        } catch (InterruptedException ex) {
            System.out.println(ex);
        }
    }
}
