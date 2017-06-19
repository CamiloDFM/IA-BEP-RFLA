#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    
    // abrir el archivo con la instancia
    
    if (argc != 2){
        printf("Este programa necesita recibir el nombre del archivo de la instancia como parametro (y nada mas!). Abortando...");
        return 0;
    }
    
    FILE* instancia = fopen(argv[1], "r");
    
    // los datos de la instancia estan en el nombre de archivo
    
    int nEstaciones, nPuntos, nRefugios, nBuses;
    sscanf(argv[1], "InstanciasBEP/InstanceBEP-%d-%d-%d-%d.txt", &nEstaciones, &nPuntos, &nRefugios, &nBuses);
    
    // leer las capacidades
    
    // buses
    int skip, capacidadBuses;
    fscanf(instancia, "%d: %d\n", &skip, &capacidadBuses);
    
    // estaciones
    int* busesEstacion = (int*)malloc(sizeof(int)*nEstaciones);
    int i;
    fscanf(instancia, "%d: ", &skip);
    for(i = 0; i < nEstaciones; i++){
        fscanf(instancia, "%d", &(busesEstacion[i]));
    }
    
    // puntos
    int personasTotales;
    fscanf(instancia, "\n%d: %d: ", &skip, &personasTotales);
    
    
    // test
    printf("%d\n", personasTotales);
    
	return 0;
}
