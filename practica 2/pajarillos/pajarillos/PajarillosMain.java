/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package pajarillos;

/**
 *
 * @author Jose
 */
public class PajarillosMain {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        int numGusanosPadre = 0;
        int numPajarillos = 0;
        if(args.length==1){
            numPajarillos=Integer.parseInt(args[0]);
            numGusanosPadre=10;
        }else if(args.length==2){
            numPajarillos=Integer.parseInt(args[0]);
            numGusanosPadre=Integer.parseInt(args[1]);
        }else{
            System.err.println("Hay que pasar dos parámetros:\n\t1. Número de pajarillos.\n\t2. Número de gusanos que repone el padre. (opcional)");
            return;
        }
        Nido nido = new Nido(numGusanosPadre, numPajarillos);
        PajaroPadre pajaroPadre = new PajaroPadre(nido, numGusanosPadre);
        Pajarillos pajarillos[] = new Pajarillos[numPajarillos];
        for (int i=0; i<numPajarillos; i++){
            pajarillos[i]=new Pajarillos(nido, i);
        }
        pajaroPadre.thr.start();
        for(int i=0; i<numPajarillos; i++){
            pajarillos[i].thr.start();
        }
    }
}
