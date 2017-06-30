#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define True 1
#define False 0

int restriccion6(int** matriz, int rondaActual, int busActual, int valorInstanciado){
    // chequea cumplimiento de la restriccion 6
    // restriccion 6: si un itinerario termina, no se puede reiniciar
    if (valorInstanciado == 0 || rondaActual == 0){
        return True; // instanciar con 0 nunca entrega problemas con la rest6, y la primera ronda puede tener cualquier valor
    }
    else if (matriz[rondaActual - 1][busActual] == 0){
        return False; // si el movimiento actual es un movimiento, el anterior no puede ser estar quieto
    }
    else {
        return True;
    }
}

int restriccion7(int** matriz, int* personasPunto, int nRondas, int nBuses, int nPuntos, int capacidadBuses, int rondaActual, int busActual, int valorInstanciado){
    // chequea cumplimiento de la restriccion 7
    // restriccion 7: no debe quedar gente en ningun refugio
    int puntoOrigen = ((valorInstanciado - 1) % nPuntos);
    int i, j, punto, sumaParcial, instanciacionesFaltantes;
    for (punto = 0; punto < nPuntos; punto++){ // para cada punto se recorre la matriz entera
        sumaParcial = 0;
        for (i = 0; i < nRondas; i++){
            for (j = 0; j < nBuses; j++){
                if (matriz[i][j] != 0 && (matriz[i][j] - 1) % nPuntos == punto){
                    sumaParcial += capacidadBuses; // se cuenta cuanta gente salio en bus del punto
                }
            }
        }
        if (valorInstanciado != 0 && puntoOrigen == punto){
            sumaParcial += capacidadBuses; // se considera si el movimiento actual saca gente del punto
        }
        instanciacionesFaltantes = (nRondas * nBuses) - (rondaActual * nBuses + busActual + 1);
        if (sumaParcial + capacidadBuses * instanciacionesFaltantes < personasPunto[punto]){
            return False; // y si no es mayor o igual que toda la gente que habia en principio, la solucion es invalida
            // tambien sirve que a futuro todos los recursos se destinen a este punto
        }
    }
    return True;
}

int restriccion8(int** matriz, int* capacidadRefugio, int nRondas, int nBuses, int nPuntos, int capacidadBuses, int rondaActual, int busActual, int valorInstanciado){
    // chequea cumplimiento de la restriccion 8
    // restriccion 8: no se puede sobrepasar la capacidad de un refugio
    if (valorInstanciado == 0){
        return True;
    }
    int refugioDestino = (valorInstanciado - 1) / nPuntos;
    int sumaParcial = 0;
    int i, j;
    for (i = 0; i < nRondas; i++){
        for (j = 0; j < nBuses; j++){
            if ((matriz[i][j] - 1) / nPuntos == refugioDestino && matriz[i][j] != 0){
                sumaParcial += capacidadBuses; // sumar un bus por cada movimiento en la matriz que involucre el refugio de destino
            }
        }
    }
    if (sumaParcial + capacidadBuses > capacidadRefugio[refugioDestino]){
        return False; // la restriccion se viola si el movimiento actual sobrepasa la capacidad del refugio
    }
    return True;
}

int main(int argc, char* argv[]){
    
    // abrir el archivo con la instancia
    
    if (argc != 2){
        printf("Este programa necesita recibir el nombre del archivo de la instancia como parametro (y nada mas!). Abortando...\n");
        return 0;
    }
    
    ///////////////////////////////////////////////////
    //                                               //
    //           LECTURA DE LA INSTANCIA             //
    //                                               //
    ///////////////////////////////////////////////////
    
    FILE* instancia = fopen(argv[1], "r");
    
    // leer las capacidades
    
    // buses
    
    int nBuses, capacidadBuses;
    // nBuses: cantidad de buses en juego,
    // capacidadBuses: capacidad máxima de cada bus
    fscanf(instancia, "%d: %d\n", &nBuses, &capacidadBuses);
    
    // estaciones
    int i, nEstaciones;
    // nEstaciones: cantidad de estaciones de partida de buses
    fscanf(instancia, "%d: ", &nEstaciones);
    int* busesEstacion = (int*)malloc(sizeof(int)*nEstaciones);
    // busesEstacion[i]: cantidad de buses que parten en la estacion i
    for (i = 0; i < nEstaciones; i++){
        fscanf(instancia, "%d", &(busesEstacion[i]));
    }
    
    // puntos
    int nPuntos, personasTotales;
    // nPuntos: cantidad de puntos de recogida de personas,
    // personasTotales: cantidad total de gente a ser evacuada
    fscanf(instancia, "\n%d: %d: ", &nPuntos, &personasTotales);
    int* personasPunto = (int*)malloc(sizeof(int)*nPuntos);
    // personasPunto[i]: cantidad de personas a recoger en el punto i
    for (i = 0; i < nPuntos; i++){
        fscanf(instancia, "%d", &(personasPunto[i]));
    }
    
    // refugios
    int nRefugios, capacidadTotal;
    // nRefugios: cantidad de refugios / sumideros para el problema,
    // capacidadTotal: suma de las capacidades máximas de cada refugio
    fscanf(instancia, "\n%d: %d: ", &nRefugios, &capacidadTotal);
    int* capacidadRefugio = (int*)malloc(sizeof(int)*nRefugios);
    // capacidadRefugio[i]: capacidad máxima del refugio i
    for (i = 0; i < nRefugios; i++){
        fscanf(instancia, "%d", &(capacidadRefugio[i]));
    }
    
    // leer distancias entre puntos, estaciones y refugios
    
    // distancias entre estaciones y puntos
    int** distanciasEstacionPunto = (int**)malloc(sizeof(int*)*nEstaciones);
    for (i = 0; i < nEstaciones; i++){
        distanciasEstacionPunto[i] = (int*)malloc(sizeof(int)*nPuntos);
    } 
    // distanciasEstacionPunto[i][j]: distancia entre la estacion inicial i y el punto de recogida j
    
    int j, skip;
    for (i = 0; i < nEstaciones; i++){
        fscanf(instancia, "%d: ", &skip);
        for (j = 0; j < nPuntos; j++){
            fscanf(instancia, "%d", &(distanciasEstacionPunto[i][j]));
        }
    }
    
    // distancias entre puntos y refugios
    int** distanciasPuntoRefugio = (int**)malloc(sizeof(int*)*nPuntos);
    for (i = 0; i < nPuntos; i++){
        distanciasPuntoRefugio[i] = (int*)malloc(sizeof(int)*nRefugios);
    } 
    // distanciasPuntoRefugio[i][j]: distancia entre el punto de recogida i y el refugio j

    for (i = 0; i < nPuntos; i++){
        fscanf(instancia, "%d: ", &skip);
        for (j = 0; j < nRefugios; j++){
            fscanf(instancia, "%d", &(distanciasPuntoRefugio[i][j]));
        }
    }
    
    // cleanup
    fclose(instancia);
    
    ///////////////////////////////////////////////////
    //                                               //
    //           OBTENER SOLUCION INICIAL            //
    //                                               //
    ///////////////////////////////////////////////////
    
    // declarar variables de busqueda: matriz tridimensional de tamanyo nPuntos * nRefugios * nBuses * nRondas
    // enfoque inicial: obtener cantidad de rondas desde la solucion trivial
    // idea: asignar ordenadamente buses a puntos y refugios para establecer una cota superior
    
    // copiar arreglos de puntos y refugios para trabajar con una copia local
    int* auxP = (int*)malloc(sizeof(nPuntos)); 
    for (i = 0; i < nPuntos; i++){
        auxP[i] = personasPunto[i];
    }
    
    int* auxR = (int*)malloc(sizeof(nRefugios));
    for (i = 0; i < nRefugios; i++){
        auxR[i] = capacidadRefugio[i];
    }
    
    int ptrP = 0, ptrR = 0, nRondas = 0;
    
    while (ptrP != nPuntos){
        // simular las rondas y ver que pasa
        for (i = 0; i < nBuses; i++){
            
            // en una ronda asignamos cada bus a llevarse toda la gente que pueda del punto actual y mandarla al refugio actual
            // se procede al siguiente punto/refugio cuando el actual se vacie/llene
            if (capacidadBuses <= auxP[ptrP] && capacidadBuses <= auxR[ptrR]){
                // este paso esta acotado por la capacidad del bus
                auxP[ptrP] -= capacidadBuses;
                auxR[ptrR] -= capacidadBuses;
            }
            
            else if (auxP[ptrP] <= capacidadBuses && auxP[ptrP] <= auxR[ptrR]){
                // este paso esta acotado por la cantidad de gente en el punto
                auxR[ptrR] -= auxP[ptrP];
                auxP[ptrP] = 0;
            }
            else {
                // este paso esta acotado por la capacidad del refugio
                auxP[ptrP] -= auxR[ptrR];
                auxR[ptrR] = 0;
            }
            
            if (auxP[ptrP] == 0){
                // si pasa esto, terminamos de revisar el punto actual
                ptrP++;
                if (ptrP == nPuntos){
                    // si pasa esto, terminamos de evacuar a todo el mundo
                    break;
                }
            }
            
            if (auxR[ptrR] == 0){
                // si pasa esto, el refugio actual se lleno
                ptrR++;
                if (ptrR == nRefugios){
                    // si pasa esto, hay problemas en la instancia
                    printf("Hay mas gente a ser rescatada que espacio en los refugios. Lamentablemente, este programa no esta autorizado a realizar elecciones moralmente complicadas. Abortando...\n");
                    printf("ERROR CODE 3M1Y4: library header <utilitarianism.h> not imported\n");
                    return 0;
                }
            }
        }
        nRondas++;
    }
    
    printf("La solucion trivial dio una cota maxima de %d rondas.\n\n", nRondas);
    
    free(auxP);
    free(auxR);

    ///////////////////////////////////////////////////
    //                                               //
    //             DECLARAR VARIABLES                //
    //                                               //
    ///////////////////////////////////////////////////
    
    // cambio en el modelo con respecto al paper
    // M_rb: en la ronda r el bus b hara el movimiento m, m \in [0, nPuntos*nRefugios] (0 representa no hacer nada)
    int** M = (int**)malloc(sizeof(int*)*nRondas);
    for (i = 0; i < nRondas; i++){
        M[i] = (int*)calloc(nBuses, sizeof(int));
    }    
        
    // la cantidad de variables pasa a ser nRondas * nBuses
    // el espacio de busqueda se mantiene ligeramente igual
    // como fuerzo a cada bus en cada ronda a tomar un solo movimiento, la restriccion 5 del paper se cumple siempre
    
    // una segunda copia para cuando encuentre un optimo
    // cuando haya una opcion cuya funcion objetivo sea mejor que el optimo, vuelco el contenido de las variables aca
    int** movimientosOptimos = (int**)malloc(sizeof(int*)*nRondas);
    for (i = 0; i < nRondas; i++){
        movimientosOptimos[i] = (int*)calloc(nBuses, sizeof(int));
    }
    
    // una tercera copia para trabajar tentativamente al revisar las variables futuras
    // cada vez que actualice la original debo copiar los contenidos a esta
    int** MLA = (int**)malloc(sizeof(int*)*nRondas);
    for (i = 0; i < nRondas; i++){
        MLA[i] = (int*)calloc(nBuses, sizeof(int));
    }
    
    // y dejo el objetivo listo
    
    int objetivoOptimo = 2147483647; // valor maximo para un entero de 4 bytes
    
    ///////////////////////////////////////////////////
    //                                               //
    //                  RESOLUCION                   //
    //                                               //
    ///////////////////////////////////////////////////
    
    // esta cosa es un backtracking con seleccion inteligente de que hacer a continuacion
    // la base es la misma:
    // -> revisar dominio de la variable
    // -> eliminar valores que choquen con una variable anterior
    // -> si no quedan valores en el dominio, volver
    // -> de lo contrario, instanciar la variable con cada valor ordenadamente
    // el RFLA se agrega en la revision del dominio, mirando variables futuras y manteniendo arcoconsistencia
    
    int rondaActual = 0, busActual = 0;
    list* dominioActual;
    // crear dominios para cada variable - matriz no inicializada
    list*** dominios = (list***)malloc(sizeof(list**)*nRondas);
    for (i = 0; i < nRondas; i++){
        dominios[i] = (list**)malloc(sizeof(list*)*nBuses);
    }
    M[rondaActual][busActual] = 0;
    dominios[rondaActual][busActual] = createList();
    for (i = 0; i <= nPuntos * nRefugios; i++){
        appendElement(dominios[rondaActual][busActual], i);
    }
    
    int terminar = False, volver = False; // i wish this was python
    int k, h, sumaBus, tiempoBusMaximo, puntoOrigen, refugioDestino, refugioAnterior, estacionActual, busesTotales, iteraciones = 0, megaIteraciones = 0, posible = True;
    
    while (terminar == False){        
        
        iteraciones++;
        
        if (iteraciones == 10000000){
            printf("sigo funcionando\n");
            iteraciones = 0;
            megaIteraciones += 10;
        }
        
        /*
        printf("variable actual: R%dB%d, matriz tentativa:\n", rondaActual, busActual);
        for(i = 0; i < nRondas; i++){
            for (j = 0; j < nBuses; j++){
                printf("%d ", M[i][j]);
            }
            printf("\n");
        }
        */
        
        if (volver){
            // recien se hizo backtrack, tomar otro elemento del dominio y seguir
            volver = False;
        }
        else {
            // aumentar variable
            busActual++;
            if (busActual == nBuses){
                busActual = 0;
                rondaActual++;
            }
            
            // revisar dominio de la variable
            
            // crear dominio limpio
            dominios[rondaActual][busActual] = createList();
            dominioActual = dominios[rondaActual][busActual]; // legibilidad
            
            
            // revisar restricciones por cada elemento del dominio - si una no se cumple, no se agrega a la lista
            for (i = 0; i <= nPuntos * nRefugios; i++){
                if (!restriccion6(M, rondaActual, busActual, i)){
                    continue;
                }
                if (!restriccion7(M, personasPunto, nRondas, nBuses, nPuntos, capacidadBuses, rondaActual, busActual, i)){
                    continue;
                }
                if (!restriccion8(M, capacidadRefugio, nRondas, nBuses, nPuntos, capacidadBuses, rondaActual, busActual, i)){
                    continue;
                }
                
                // si esta se cumple, revisar si todas las variables futuras tienen alguna instanciacion posible
                
                MLA[rondaActual][busActual] = i;
                
                for (j = rondaActual; j < nRondas; j++){
                    for (k = 0; k < nBuses; k++){
                        if ((j == rondaActual && k <= busActual)){
                            continue;
                        }
                        for (h = 0; h <= nPuntos * nRefugios; h++){
                            posible = False;
                            if (restriccion6(MLA, j, k, h) &&
                                restriccion7(MLA, personasPunto, nRondas, nBuses, nPuntos, capacidadBuses, rondaActual, busActual, h) &&
                                restriccion8(MLA, capacidadRefugio, nRondas, nBuses, nPuntos, capacidadBuses, j, k, h)){
                                posible = True;
                                break;
                            }
                        }
                        if (!posible){
                            break;
                        }
                    }
                }
                MLA[rondaActual][busActual] = 0;
                if (!posible){
                    posible = True;
                    printf("FC anulo un dominio\n");
                    continue;
                }

                appendElement(dominioActual, i); // agregar valor al dominio
            }
        }
        
        if (dominioActual -> len == 0){
            volver = True;
            // limpiar memoria
            deleteList(dominioActual);
            // limpiar variable
            M[rondaActual][busActual] = 0;
            // disminuir variable
            busActual -= 1;
            if (busActual < 0){
                busActual = nBuses - 1;
                rondaActual -= 1;
            }
            // el elemento del dominio que se intento usar es el primero
            // como provoco un fallo, lo saco
            dominioActual = dominios[rondaActual][busActual];
            deleteElement(dominioActual, 0);
            // instanciar otro elemento
        }
        
        if (dominioActual -> len != 0){ // si el dominio de la variable anterior es vacia, se encontrara el problema en la siguiente iteracion
            M[rondaActual][busActual] = at(dominioActual, 0); // instanciar variable con primer elemento del dominio
            MLA[rondaActual][busActual] = M[rondaActual][busActual];
            if (busActual == nBuses - 1 && rondaActual == nRondas - 1){ // si estamos mirando la ultima variable
                volver = True; // no crear una nueva en la siguiente iteracion, solo mirar mas elementos del dominio
                // calcular FO
                tiempoBusMaximo = 0;
                
                for (j = 0; j < nBuses; j++){
                    sumaBus = 0;
                    for (i = 0; i < nRondas && M[i][j] != 0; i++){
                        puntoOrigen = (M[i][j] - 1) % nPuntos;
                        refugioDestino = (M[i][j] - 1) / nPuntos;
                        // sumar tiempo que se demora el bus en llegar al punto
                        if (i == 0){ // primera ronda - sumar tiempo entre estacion y punto
                            estacionActual = 0;
                            busesTotales = 0;
                            for (k = 0; k < nEstaciones; k++){
                                busesTotales += busesEstacion[k];
                                if (j < busesTotales){
                                    estacionActual = k;
                                    break;
                                }
                            }
                            sumaBus += distanciasEstacionPunto[estacionActual][puntoOrigen];
                        }
                        else { // !(primera) ronda - sumar tiempo entre refugio anterior y punto actual
                            refugioAnterior = (M[i-1][j] - 1) / nPuntos;
                            sumaBus += distanciasPuntoRefugio[puntoOrigen][refugioAnterior];
                        }
                        // sumar demora entre punto y refugio
                        sumaBus += distanciasPuntoRefugio[puntoOrigen][refugioDestino];
                    }
                    if (sumaBus > tiempoBusMaximo){
                        tiempoBusMaximo = sumaBus;
                    }
                }
                
                if (tiempoBusMaximo < objetivoOptimo){
                    objetivoOptimo = tiempoBusMaximo;
                    // guardar variables
                    for (i = 0; i < nRondas; i++){
                        for (j = 0; j < nBuses; j++){
                            movimientosOptimos[i][j] = M[i][j];
                        }
                    }
                    printf("Nuevo minimo encontrado!\nMatriz de movimientos:\n");
                    for(i = 0; i < nRondas; i++){
                        for (j = 0; j < nBuses; j++){
                            printf("%d ", M[i][j]);
                        }
                        printf("\n");
                    }
                    printf("Esta solucion tiene una duracion de %d para la evacuacion.\n\n", objetivoOptimo);
                } 
                // ahora se elimina el valor del dominio recien usado y se sigue
                deleteElement(dominioActual, 0);
            }
        }
        else if (busActual == 0 && rondaActual == 0){ // pero si el dominio es vacio, y ademas estamos en la primera variable
            terminar = True; // entonces terminamos de revisar todo el espacio de busqueda
        }
    }
    
    ///////////////////////////////////////////////////
    //                                               //
    //                   SOLUCION                    //
    //                                               //
    ///////////////////////////////////////////////////
    
    printf("#################################\n\nSolucion final:\n");
    
    for(i = 0; i < nRondas; i++){
        printf("En la ronda %d:\n", i + 1);
        for(j = 0; j < nBuses; j++){
            if (movimientosOptimos[i][j] != 0){
                printf("    Bus %d: Punto %d -> Refugio %d\n", j + 1, ((movimientosOptimos[i][j] - 1) % nPuntos) + 1, ((movimientosOptimos[i][j] - 1) / nPuntos) + 1);
            }
        }
    }
    
    printf("Mayor distancia recorrida por un bus: %d unidades.\n", objetivoOptimo);
    printf("%d Miteraciones, %d iteraciones\n", megaIteraciones, iteraciones);
    
	return 0;
}
