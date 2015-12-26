/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package fumadores;

/**
 *
 * @author Jose
 */
public class PruebaFumadores {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        //Declaración de la instancia del monitor y las instancias de las clases que implementan Runnable
        Estanco estanco = new Estanco();
        Fumador fumadores[];
        Estanquero estanquero;
        fumadores = new Fumador[3];
        
        //Creación de las hebras.
        for (int i=0; i<3; i++){
            fumadores[i]=new Fumador(estanco, i);
        }
        estanquero = new Estanquero(estanco);
        
        //lanzar las hebras.
        for (int i=0; i<3; i++){
            fumadores[i].thr.start();
        }
        estanquero.thr.start();
        
        try{
            for (int i=0; i<3; i++){
                fumadores[i].thr.join();
            }
            estanquero.thr.join();
        }catch (InterruptedException ex){
            System.out.println(ex);
        }
    }
}
