/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package pajarillos;

import monitor.AbstractMonitor;
import monitor.Condition;

/**
 *
 * @author Jose
 */
public class Nido extends AbstractMonitor{
    private int gusanosDisponibles;
    private boolean hayUnPajarilloComiendo;
    private Condition pajaroPadre, colaPajarillos, puedoComer;
    
    public Nido(int gusanosDisponibles, int numPajarillos){
        this.gusanosDisponibles=gusanosDisponibles;
        pajaroPadre=makeCondition();
        puedoComer=makeCondition();
        colaPajarillos=makeCondition();
        hayUnPajarilloComiendo=false;
    }
    
    public void serUnBuenPajaroPadre(){
        enter();
        if(gusanosDisponibles!=0){
            pajaroPadre.await();
        }
        System.out.println("El pájaro padre ha ido a buscar más gusanos.");
        leave();
    }
    
    public void ponerGusanos(int nGusanos){
        enter();
        gusanosDisponibles=nGusanos;
        System.out.println("El pájaro padre ha puesto " + nGusanos + " gusanos en el nido.");
        puedoComer.signal();
        leave();
    }
    
    public void comerUnGusano(int idx){
        enter();
        if(hayUnPajarilloComiendo){
            System.out.println("El pajarillo " + idx + " quiere comer.");
            colaPajarillos.await();
        }
        System.out.println("El pajarillo " + idx + " va al nido para comer.");
        hayUnPajarilloComiendo=true;
        if(gusanosDisponibles==0){
            System.out.println("No hay gusanos en el nido, así que el pajarillo " + idx + " despierta al padre y espera a que traiga gusanos.");
            pajaroPadre.signal();
            puedoComer.await();
        }
        System.out.println("El pajarillo " + idx + " empieza a comer.");
        leave();
    }
    
    public void terminarComer(int idx){
        enter();
        System.out.println("El pajarillo " + idx + " termina de comer y se pone a jugar.");
        gusanosDisponibles--;
        hayUnPajarilloComiendo=false;
        colaPajarillos.signal();
        leave();
    }
}
