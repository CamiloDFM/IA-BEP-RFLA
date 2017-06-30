#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define True 1
#define False 0

int restriccion6Global(int** matriz, int nRondas, int nBuses){
    int i, j, terminado;
    for (j = 0; j < nBuses; j++){
        terminado = False;
        for (i = 0; i < nRondas; i++){
            if (matriz[i][j] == 0){
                terminado = True;
            }
            else if (terminado){
                return False;
            }
        }
    }
    return True;
}

int restriccion6Local(int** matriz, int nRondas, int busActual){
    int i, terminado = False;
    for (i = 0; i < nRondas; i++){
        if (matriz[i][busActual] == 0){
            terminado = True;
        }
        else if (terminado){
            return False;
        }
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
    int sumaParcial, k, h, filtrar, sumaBus, tiempoBusMaximo, puntoOrigen, refugioDestino, refugioAnterior, estacionActual, busesTotales, iteraciones = 0;
    
    while (terminar == False){        
        
        iteraciones++;
        
        if (iteraciones == 10000000){
            printf("sigo funcionando\n");
            iteraciones = 0;
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
                if (i != 0){ // si el movimiento actual no es una detencion:
                    // restriccion 6 del paper: si un bus se detiene en una ronda, no puede continuar en la siguiente
                    if ((rondaActual != 0) && (M[rondaActual - 1][busActual] == 0)){ // si no es la primera ronda y el movimiento anterior si era una detencion
                        continue; // entonces hay un tour cortado - no agregar el valor al dominio
                    }

                    // restriccion 8 del paper: no puede superarse la capacidad de los refugios
                    // notar que los archivos de las instancias cumplen el supuesto del paper: las cantidades de evacuados y las capacidades de los refugios son multiplos enteros de las capacidades de los buses, por lo que se cuentan los viajes
                    refugioDestino = ((i - 1) / nPuntos); // analizar a que refugio se esta llevando gente
                    sumaParcial = 0;
                    for (j = 0; j < nRondas; j++){ // por cada ronda anterior
                        for (k = 0; k < nBuses; k++){ // por cada bus posible
                            if ((M[j][k] - 1) / nPuntos == refugioDestino && M[j][k] != 0){ // si es que ese movimiento implico el refugio destino
                                sumaParcial += capacidadBuses; // sumarle un bus lleno de gente al conteo parcial
                            }
                        }
                    }
                    if (sumaParcial + capacidadBuses > capacidadRefugio[refugioDestino]){ // si agregarle otro bus lleno de gente al refugio lo deja por sobre su capacidad maxima
                        continue; // no agregar el valor al dominio
                    }
                }
                if (busActual == nBuses - 1 && rondaActual == nRondas - 1) { // la restriccion 7 no se puede verificar si no se esta instanciando la ultima variable, pues provoca falsos fallos en instanciaciones incompletas
                    // restriccion 7 del paper: al final, toda la gente esperando en los puntos debe haber sido evacuada
                    filtrar = False;
                    puntoOrigen = ((i - 1) % nPuntos); // analizar desde que punto se esta llevando gente
                    
                    for (h = 0; h < nPuntos; h++){
                        sumaParcial = 0;
                        for (j = 0; j < nRondas; j++){ // por cada ronda efectiada
                            for (k = 0; k < nBuses; k++){ // por cada bus posible
                                if (M[j][k] != 0 && (M[j][k] - 1) % nPuntos == h){ // si ese movimiento implico el punto de origen
                                    sumaParcial += capacidadBuses; // sumarle un bus lleno de gente al conteo
                                }
                            }
                        }
                        if (i != 0 && puntoOrigen == h){ // si el punto actual es el origen
                            sumaParcial += capacidadBuses; // sumarle otro bus lleno de gente al conteo
                        }
                        if(sumaParcial < personasPunto[h]){ // si el conteo me dice que no he evacuado a todas las personas en algun punto...
                            filtrar = True;
                            break;
                        }
                    }
                    
                    if (filtrar){
                        continue;
                    }
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
    
	return 0;
}
