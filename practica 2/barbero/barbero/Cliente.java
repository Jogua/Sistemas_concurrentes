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
public class Cliente implements Runnable{
    private Barberia barberia;
    public Thread thr;
    private int indice;
    private int numVeces;
    private Random rand;
    
    public Cliente(Barberia barb, int idx, int nVeces){
        barberia=barb;
        indice=idx;
        thr = new Thread(this, "Cliente 1");
        numVeces=nVeces;
        rand = new Random();
    }
    
    @Override
    public void run(){
        int time;
        for(int i=0; i<numVeces; i++){
            time=(rand.nextInt(7)+4)*1000; //Tardara entre 4 y 10 segundos en pelarse y volver a la barberia.
            barberia.cortarPelo(indice);
            try {
                Thread.sleep(time);
            } catch (InterruptedException e) {
                System.out.println(e);
            }
        }
    }
    
    
}
