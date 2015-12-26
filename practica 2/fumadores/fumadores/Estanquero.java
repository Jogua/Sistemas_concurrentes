/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package fumadores;

import java.util.Random;

/**
 *
 * @author Jose
 */
public class Estanquero implements Runnable{
    private Estanco estanco;
    public Thread thr;
    private Random r;

    
    public Estanquero(Estanco est){
        estanco=est;
        r = new Random();
        thr = new Thread(this, "Estanquero");
    }
    
    @Override
    public void run(){
        int ingr;
        while (true){
            ingr=r.nextInt(3);
            estanco.producirIngrediente(ingr);
        }
    }
}
