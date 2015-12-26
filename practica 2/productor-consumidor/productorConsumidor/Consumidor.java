/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package productorConsumidor;

/**
 *
 * @author Jose
 */
class Consumidor implements Runnable {

    private Buffer bb;
    int veces;
    int numC;
    Thread thr;

    public Consumidor(Buffer pbb, int pveces, int pnumC) {
        bb = pbb;
        veces = pveces;
        numC = pnumC;
        thr = new Thread(this, "consumidor " + numC);
    }

    @Override
    public void run() { 
        {
            for (int i = 0; i < veces; i++) {
                double item = bb.consumir();
                System.out.println(thr.getName() + ", consumiendo " + item);
            }
        }
    }
}
