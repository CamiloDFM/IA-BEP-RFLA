#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    
    // abrir el archivo con la instancia
    
    if (argc != 2){
        printf("Este programa necesita recibir el nombre del archivo de la instancia como parametro (y nada mas!). Abortando...\n");
        return 0;
    }
    
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
    for(i = 0; i < nEstaciones; i++){
        fscanf(instancia, "%d", &(busesEstacion[i]));
    }
    
    // puntos
    int nPuntos, personasTotales;
    // nPuntos: cantidad de puntos de recogida de personas,
    // personasTotales: cantidad total de gente a ser evacuada
    fscanf(instancia, "\n%d: %d: ", &nPuntos, &personasTotales);
    int* personasPunto = (int*)malloc(sizeof(int)*nPuntos);
    // personasPunto[i]: cantidad de personas a recoger en el punto i
    for(i = 0; i < nPuntos; i++){
        fscanf(instancia, "%d", &(personasPunto[i]));
    }
    
    // refugios
    int nRefugios, capacidadTotal;
    // nRefugios: cantidad de refugios / sumideros para el problema,
    // capacidadTotal: suma de las capacidades máximas de cada refugio
    fscanf(instancia, "\n%d: %d: ", &nRefugios, &capacidadTotal);
    int* capacidadRefugio = (int*)malloc(sizeof(int)*nRefugios);
    // capacidadRefugio[i]: capacidad máxima del refugio i
    for(i = 0; i < nRefugios; i++){
        fscanf(instancia, "%d", &(capacidadRefugio[i]));
    }
    
    // leer distancias entre puntos, estaciones y refugios
    
    // distancias entre estaciones y puntos
    int** distanciasEstacionPunto = (int**)malloc(sizeof(int*)*nEstaciones);
    for(i = 0; i < nEstaciones; i++){
        distanciasEstacionPunto[i] = (int*)malloc(sizeof(int)*nPuntos);
    } 
    // distanciasEstacionPunto[i][j]: distancia entre la estacion inicial i y el punto de recogida j
    
    int j, skip;
    for(i = 0; i < nEstaciones; i++){
        fscanf(instancia, "%d: ", &skip);
        for(j = 0; j < nPuntos; j++){
            fscanf(instancia, "%d", &(distanciasEstacionPunto[i][j]));
        }
    }
    
    // distancias entre puntos y refugios
    int** distanciasPuntoRefugio = (int**)malloc(sizeof(int*)*nPuntos);
    for(i = 0; i < nPuntos; i++){
        distanciasPuntoRefugio[i] = (int*)malloc(sizeof(int)*nRefugios);
    } 
    // distanciasPuntoRefugio[i][j]: distancia entre el punto de recogida i y el refugio j

    for(i = 0; i < nPuntos; i++){
        fscanf(instancia, "%d: ", &skip);
        for(j = 0; j < nRefugios; j++){
            fscanf(instancia, "%d", &(distanciasPuntoRefugio[i][j]));
        }
    }
    
    // cleanup
    fclose(instancia);
    
    // declarar variables de busqueda: matriz tetradimensional de tamanyo nPuntos * nRefugios * nBuses * nRondas
    // enfoque inicial: obtener cantidad de rondas desde la solucion trivial
    // idea: asignar ordenadamente buses a puntos y refugios para establecer una cota superior
    
    // copiar arreglos de puntos y refugios para trabajar con una copia local
    int* auxP = (int*)malloc(sizeof(nPuntos)); 
    for(i = 0; i < nPuntos; i++){
        auxP[i] = personasPunto[i];
    }
    int* auxR = (int*)malloc(sizeof(nRefugios));
    for(i = 0; i < nRefugios; i++){
        auxR[i] = capacidadRefugio[i];
    }
    int ptrP, ptrR, nRondas = 0;
    
    while(ptrP != nPuntos){
        // simular las rondas y ver que pasa
        for(i = 0; i < nBuses; i++){
            // en una ronda asignamos cada bus a llevarse toda la gente que pueda del punto actual y mandarla al refugio actual
            // se procede al siguiente punto/refugio cuando el actual se vacie/llene
            if(capacidadBuses <= auxP[ptrP] && capacidadBuses <= auxR[ptrR]){
                // este paso esta acotado por la capacidad del bus
                auxP[ptrP] -= capacidadBuses;
                auxR[ptrR] -= capacidadBuses;
            }
            else if(auxP[ptrP] <= capacidadBuses && auxP[ptrP] <= auxR[ptrR]){
                // este paso esta acotado por la cantidad de gente en el punto
                auxR[ptrR] -= auxP[ptrP];
                auxP[ptrP] = 0;
            }
            else{
                // este paso esta acotado por la capacidad del refugio
                auxP[ptrP] -= auxR[ptrR];
                auxR[ptrR] = 0;
            }
            if(auxP[ptrP] == 0){
                // si pasa esto, terminamos de revisar el punto actual
                ptrP++;
                if (ptrP == nPuntos){
                    // si pasa esto, terminamos de evacuar a todo el mundo
                    break;
                }
            }
            if(auxR[ptrR] == 0){
                // si pasa esto, el refugio actual se lleno
                ptrR++;
                if (ptrR == nRefugios){
                    // si pasa esto, hay problemas en la instancia
                    printf("Hay mas gente a ser rescatada que espacio en los refugios. Lamentablemente, este programa no esta autorizado a realizar elecciones moralmente complicadas. Abortando...\n");
                    printf("ERROR CODE 3M1Y4: library header <utilitarianism.h> not imported\n")
                    return 0;
                }
            }
        }
        nRondas++;
    }
    
    printf("%d\n", nRondas);
    
    free(auxP);
    free(auxR);
    
    int k;
    
    // X_ijbr: el bus b va desde el punto i hacia el refugio j en la ronda r
    int**** X = (int****)malloc(sizeof(int***)*nPuntos);
    for(i = 0; i < nPuntos; i++){
        X[i] = (int***)malloc(sizeof(int**)*nRefugios);
        for(j = 0; j < nRefugios; j++){
            X[i][j] = (int**)malloc(sizeof(int*)*nBuses);
            for(k = 0; k < nBuses; k++){
                X[i][j][k] = (int*)calloc(nRondas, sizeof(int));
            }
        }
    }
    
    // test
    
    int l;
    for(i = 0; i < nPuntos; i++){
        for(j = 0; j < nRefugios; j++){
            for(k = 0; k < nBuses; k++){
                for(l = 0; l < nRondas; l++){
                    printf("%d ", X[i][j][k][l]);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }
    
	return 0;
}
