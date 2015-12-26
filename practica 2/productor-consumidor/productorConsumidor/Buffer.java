/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package productorConsumidor;

import monitor.AbstractMonitor;
import monitor.Condition;

/**
 *
 * @author Jose
 */
public class Buffer extends AbstractMonitor {

    private int numSlots = 0, cont = 0;
    private double[] buffer = null;
    private Condition escritura = makeCondition();
    private Condition lectura = makeCondition();

    public Buffer(int p_numSlots) {
        numSlots = p_numSlots;
        buffer = new double[numSlots];
    }

    public void producir(double valor){
        enter();
        if(cont==numSlots){
            escritura.await();
        }
        buffer[cont]=valor;
        cont++;
        lectura.signal();
        leave();
    }
    
    public double consumir(){
        double valor;
        enter();
        if(cont==0){
            lectura.await();
        }
        cont--;
        valor=buffer[cont];
        escritura.signal();
        leave();
        return valor;
    }
}