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
public class Fumador implements Runnable{
    private int miIngrediente;
    public Thread thr;
    private Estanco estanco;
    private Random r;
    
    public Fumador (Estanco est, int ingr){
        estanco=est;
        miIngrediente=ingr;
        r = new Random();
        thr = new Thread(this,"Fumador "+ ingr);
    }
    
    @Override
    public void run(){
        int time;
        while(true){
            time = (r.nextInt(5)+1)*1000; //Dormira de 1 a 5 segundos
            estanco.obtenerIngredientes(miIngrediente);
            try{
                Thread.sleep(time);
                System.out.println("El fumador "+ miIngrediente + " ha acabado de fumar.");
            }catch (InterruptedException ex) {
                System.out.println(ex);
            }
        }
    }
}
