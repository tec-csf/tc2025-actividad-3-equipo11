/*
	Fernando Garrote de la Macorra A01027503
	Alejandra Nissan Leizorek A01024682
	
	Actividad 3 Ejercicio 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct{
    int min;
    int max;
    int numArchivos;
}urna;

int numeroUrnas = 0;

//Funciones
void recorrerArchivos(char *, urna *);
void imprimir(urna *);

int main(int argc, const char* argv[]){
    char * currentDir;
    int tamanoUrnas = 0, i=0;
    currentDir = (char*)malloc(100*sizeof(char));

    printf("Favor de ingresar el número de urnas: ");
    scanf("%d", &numeroUrnas);

    printf("Favor de ingresar el rango de las urnas en bytes: ");
    scanf("%d", &tamanoUrnas);

    urna * urnas = (urna*)malloc(numeroUrnas*sizeof(urna));

    //Asignación de rangos a las urnas
    urna * aux = urnas;
    urna * final = urnas + numeroUrnas;
    for(;aux<final; ++aux){
        //A la ultima urna le asignamos al valor max -1 para identificar que es mayor que el min
        if(aux==final-1){
            aux->min= i*tamanoUrnas;
            aux->max= -1;            
        }
        else{
            aux->min= i*tamanoUrnas;
            aux->max= (i*tamanoUrnas) + tamanoUrnas -1;
        }
        i++;
        printf("Min: %d Max: %d\n", aux->min, aux->max);
    }

    printf("Favor de ingresar el directorio para comenzar: \n");
    scanf(" %[^\n]", currentDir);

    recorrerArchivos(currentDir, urnas); 

    imprimir(urnas);

    free(currentDir);
    free(urnas);
}

void recorrerArchivos(char * dir, urna * urnas){
    DIR * pointer = NULL;
    urna * aux = urnas;
    urna * final = urnas + numeroUrnas;
    struct dirent *sd;
    struct stat stats;
    char * subDir;
    subDir = (char*)malloc(600*sizeof(char));

    pointer = opendir(dir);
    if(pointer == NULL){
        printf("No se pudo abrir el directorio\n");
        printf("No esta bien el path o encontro un directorio al que no puede acceder por permisos\n");
        imprimir(urnas);
        exit(1);
    }    

    while((sd = readdir(pointer))!=NULL){

        sprintf(subDir, "%s/%s", dir, sd->d_name);
        stat(subDir, &stats);
        //printf("Estamos en la carpeta: %s\n", subDir);
        if(S_ISDIR(stats.st_mode) && strcmp(sd->d_name, "..")!= 0 && strcmp(sd->d_name, ".")!= 0){
            recorrerArchivos(subDir, urnas);
        }
        else{
            if(strcmp(sd->d_name, "..")!= 0 && strcmp(sd->d_name, ".")!= 0){
                aux = urnas;
                final = urnas + numeroUrnas;
                for(;aux<final; ++aux){
                    //printf("Estamos analizando: %s tiene un tamaño de %ld\n", sd->d_name, stats.st_size);
                    if((stats.st_size< aux->max && stats.st_size>= aux->min) || (stats.st_size> aux->min && aux->max==-1)){
                        aux->numArchivos= aux->numArchivos +1;

                    }
                }
            }
        }
    }
    closedir(pointer);
    free(subDir);
}

void imprimir(urna * urnas){
    urna * aux1 = urnas;
    urna * final1 = urnas + numeroUrnas;
    int cont=0;
    printf("Urna   NumArchivo   Histograma\n");
    for(;aux1<final1; ++aux1){
        printf("%d-%d       %d      ", aux1->min, aux1->max, aux1->numArchivos);
        //Aqui se controla el número de puntos, 100 es el máximo que va a pintar
        if(aux1->numArchivos>100){
            cont = 100;
        }
        else{
            cont = aux1->numArchivos;
        }
        for(int i=0; i<cont; i++){
            printf("*");
        }
        printf("\n");
    }   
}

