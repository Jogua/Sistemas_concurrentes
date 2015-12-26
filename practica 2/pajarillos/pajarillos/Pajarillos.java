/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package pajarillos;

import java.util.Random;

/**
 *
 * @author Jose
 */
public class Pajarillos implements Runnable{
    private Nido nido;
    public Thread thr;
    private int indice;
    private Random rand;
    
    public Pajarillos(Nido nido, int idx){
        this.nido=nido;
        indice=idx;
        thr=new Thread(this, "Pajarillo " + idx);
        rand = new Random();
    }
    
    public void run(){
        System.out.println("El pajarillo " + indice + " comienza a jugar.");
        int tiempo;                
        while(true){
            try {
                tiempo=(rand.nextInt(5)+1)*1000;    //Entre 1 y 6 segundos
                Thread.sleep(tiempo);//jugar
                nido.comerUnGusano(indice);
                tiempo=(rand.nextInt(3)+1)*1000;    //Entre 1 y 4 segundos
                Thread.sleep(tiempo); //comer
                nido.terminarComer(indice);
            } catch (InterruptedException ex) {
                System.out.println(ex);
            }
        }
    }
}
