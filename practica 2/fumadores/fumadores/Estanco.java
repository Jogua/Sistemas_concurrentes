/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package fumadores;

import monitor.AbstractMonitor;
import monitor.Condition;

/**
 *
 * @author Jose
 */
public class Estanco extends AbstractMonitor{
   private boolean hayIngredientes;
   private int ingredienteProducido;
   private Condition colaFumador[], colaEstanquero;
   
   public Estanco(){
       //Creamos las colas condición, 1 para cada fumador y otra para el estanquero.
       colaFumador=new Condition[3];
       for (int i=0; i<3; i++){
           colaFumador[i]=makeCondition();
       }
       colaEstanquero = makeCondition();
       hayIngredientes=false;
   }
   public void obtenerIngredientes (int ingr){
       enter();
       if (!hayIngredientes || ingredienteProducido!=ingr){
           colaFumador[ingr].await(); //Cada fumador espera su ingrediente
       }
       System.out.println("El fumador " + ingr + " ha cogido el ingrediente y empieza a fumar.");
       hayIngredientes=false;
       colaEstanquero.signal(); //Despierta al estanquero para que produzca un nuevo ingrediente
       leave();
   }
   
   public void producirIngrediente(int ingr){
       enter();
       if (hayIngredientes){
           colaEstanquero.await();//Si hay ingredientes espera hasta que el fumador lo retire.
       }
       ingredienteProducido=ingr;
       hayIngredientes=true;
       System.out.println("El estanquero ha producido el ingrediente " + ingr);
       colaFumador[ingredienteProducido].signal();//Despierta al fumador que tiene el ingrediente producido
       leave();
   }
}
