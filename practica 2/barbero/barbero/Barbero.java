/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package barbero;

import java.util.Random;

/**
 *
 * @author Jose
 */
public class Barbero implements Runnable{
    private Barberia barberia;
    public Thread thr;
    private int numVeces;
    private Random rand;
    
    public Barbero(Barberia barb, int nVeces){
        barberia=barb;
        thr = new Thread(this, "Barbero");
        numVeces=nVeces;
        rand = new Random();
    }
    
    @Override
    public void run(){
        int time;
        for (int i=0; i<numVeces; i++){
            time = (rand.nextInt(3)+1)*1000;
            barberia.siguienteCliente();
            try {
                Thread.sleep(time);
            } catch (InterruptedException e) {
                System.out.println(e);
            }
            barberia.finCliente();
        }
        System.out.println("El barbero ha terminado de pelar a todos los clientes y ha cerrado la barberia.");
    }
}
