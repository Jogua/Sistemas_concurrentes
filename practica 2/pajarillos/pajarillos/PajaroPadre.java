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
public class PajaroPadre implements Runnable{
    private Nido nido;
    public Thread thr;
    private int nGusanos;
    private Random rand;
    
    public PajaroPadre(Nido nido, int nGusanos){
        this.nido=nido;
        this.nGusanos=nGusanos;
        thr=new Thread(this, "Pajaro padre.");
        rand = new Random();
    }
    
    public void run() {
        while (true){
            try {
                int tiempo;
                nido.serUnBuenPajaroPadre();
                tiempo=(rand.nextInt(2)+1)*1000;    //Tarda entre 1 y 3 segundos
                Thread.sleep(tiempo);
                nido.ponerGusanos(nGusanos);
            } catch (InterruptedException ex) {
                System.out.println(ex);
            }
        }
    }
}
