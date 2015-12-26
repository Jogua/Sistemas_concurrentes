/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package barbero;

import monitor.AbstractMonitor;
import monitor.Condition;
/* @author Jose/**

 *
 */

public class Barberia extends AbstractMonitor {

    private boolean sillaLibre;
    private int nClientesEsperando;
    private boolean barberoDurmiendo;
    private int clientePelandose;
    private Condition colaSilla;
    private Condition colaBarbero;
    
    public Barberia(){
        //inicialización de las variables.
        sillaLibre=true;
        nClientesEsperando=0;
        barberoDurmiendo=false;
        //Creacion de las colas condición, una para el barbero y otra para los clientes.
        colaBarbero = makeCondition();
        colaSilla = makeCondition();
    }

    public void cortarPelo(int idx) {
        enter();
        System.out.println("El cliente " + idx + " ha entrado en la barberia.");
        while(!sillaLibre){
            nClientesEsperando++;
            colaSilla.await();//Si la silla esta ocupada el cliente se pone en la cola clientes hasta que llegue su turno.
            nClientesEsperando--;
        }
        System.out.println("El cliente " + idx + " se sienta en la silla.");
        sillaLibre=false;
        clientePelandose=idx;
        if(barberoDurmiendo){
            System.out.println("Como el barbero está durmiendo, el cliente "+ idx + " lo ha despertado.");
            colaBarbero.signal();//si el barbero estaba dormido el cliente que entra a la barbería lo despierta.
        }
        System.out.println("El barbero ha comenzado a pelar al cliente " + idx);
        leave();
    }

    public void siguienteCliente() {
        enter();
        while (nClientesEsperando==0 && sillaLibre){
            barberoDurmiendo=true;
            System.out.println("No hay clientes esperando al barbero, por lo que el barbero se duerme.");
            colaBarbero.await();
            barberoDurmiendo=false;
            System.out.println("El barbero se ha despertado.");
        }
        leave();
    }

    public void finCliente() {
        enter();
        System.out.println("El barbero ha terminado de pelar al cliente "+clientePelandose+". El cliente sale de la barberia.");
        sillaLibre=true;
        colaSilla.signal();//Como el cliente que se acaba de pelar ha abandonado la barbería, avisamos a los clientes que esten esperando de que la silla esta libre.
        leave();
    }
}
