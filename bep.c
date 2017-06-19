#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    
    // abrir el archivo con la instancia
    
    if (argc != 2){
        printf("Este programa necesita recibir el nombre del archivo de la instancia como parametro (y nada mas!). Abortando...");
        return 0;
    }
    
    // FILE* instancia = fopen(argv[1], "r");
    int nEstaciones, nPuntos, nRefugios, nBuses;
    sscanf(argv[1], "InstanciasBEP/InstanceBEP-%d-%d-%d-%d.txt", &nEstaciones, &nPuntos, &nRefugios, &nBuses);
    printf("nEstaciones: %d, nPuntos: %d, nRefugios: %d, nBuses: %d\n", nEstaciones, nPuntos, nRefugios, nBuses);
    
	return 0;
}
