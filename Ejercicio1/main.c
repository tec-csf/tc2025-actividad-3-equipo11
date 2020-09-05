/*
	Fernando Garrote de la Macorra A01027503
	Alejandra Nissan Leizorek A01024682
	
	Actividad 3 Ejercicio 1
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int contProceso;

typedef struct proceso{
	int id;
	int tiempoCPU;
	int grupo;
} proceso_t;


//Decidimos no usar un apuntador a proceso_t ya que se dificultaba mucho desencolar y volver a encolar ya que al cambiar las referencias se perdían los valores del proceso_t. 
typedef struct node{
	proceso_t proc;
	struct node *siguiente;
}nodo_t;



nodo_t * inicio=NULL;


//Funciones para lista ligada
void encolar(int, int, int);
proceso_t sacarPrimero();
void UnCore(proceso_t, int);


int main(){
	int numeroProcesos=0, numeroCPU=0, numeroGrupos=0, numProcesosEnGrupo=0, acumulador=0, cont=0, quantum = 4, i=0, k = 30, num=0; 	

	inicio=malloc(sizeof(nodo_t));
	inicio->siguiente=NULL;
	contProceso=0;
	
	//Pidiendo datos
	printf("Favor de ingresar los siguientes datos: \n");
	printf("Numero total de procesos a planificar: \n");
	scanf("%d", &numeroProcesos);
	printf("Numero de CPUs: \n");
	scanf("%d", &numeroCPU);
	printf("Numero de grupos: \n");
	scanf("%d", &numeroGrupos);

	srand(time(NULL));
	
	//For que recorre grupos
	for(int contGrupo= 1; contGrupo<=numeroGrupos; contGrupo++){
		printf("Numero de procesos en el grupo %d: \n", contGrupo);
		scanf("%d", &numProcesosEnGrupo);
		acumulador+= numProcesosEnGrupo;
		//Ayuda a controlar que no se puedan meter más procesos de los mencionados al inicio
		if(acumulador>numeroProcesos){
			acumulador-=numProcesosEnGrupo;
			numProcesosEnGrupo = numeroProcesos-acumulador;
		}
		//For que asigna grupos a procesos
		if(numProcesosEnGrupo>0){
		for(int j=0; j<numProcesosEnGrupo; j++){
			cont++;
			num = rand()%k;	
			encolar(cont, num, contGrupo);
		}
		}
	}

	printf("Procesos: \n");
	nodo_t *aux=inicio;
	printf("ID: %d Grupo: %d Tiempo de CPU: %d\n", inicio->proc.id, inicio->proc.grupo, inicio->proc.tiempoCPU);
	while(aux->siguiente!=NULL){
		aux=aux->siguiente;
		printf("ID: %d Grupo: %d Tiempo de CPU: %d\n", aux->proc.id, aux->proc.grupo, aux->proc.tiempoCPU);
	}

	i=0;

	//Aqui comienza el scheduler
	printf("-------------------------------------------------------------------------------------------------\n");
	while(contProceso>0){
		//Divide los procesos en las CPU existentes, intentando poner juntos a los grupos para eficientizar
		for(int j=0; j<numeroCPU; j++){
			if(contProceso>0){
				proceso_t temp=sacarPrimero();
				printf("Corriendo proceso %d del grupo %d en el core %d\n", temp.id, temp.grupo, j+1);
				UnCore(temp, quantum);
			}
		}
		//Esta línea representa el final del quantum, muestra lo que ocurre en cada CPU durante ese quantum
		printf("-------------------------------------------------------------------------------------------------\n");
	}

	free(inicio);
}

//Esta función simula lo que ocurre en un solo core de procesamiento
void UnCore(proceso_t temp, int quantum){
	int i = 0, num=0;

			while(temp.tiempoCPU!=0 && i<quantum){
				//Esto sirve para simular si algún procesos se bloque por IO (se hace aleatoriamiente)
				num = rand()%100;
				if(num<=5){
				printf("Proceso %d del grupo %d fue bloqueado por IO\n", temp.id, temp.grupo);	
				break;		
				}
				i++;	
				temp.tiempoCPU--;	
			}
			if(temp.tiempoCPU==0){
			 printf("Proceso %d del grupo %d ha terminado su ejecucion\n", temp.id, temp.grupo);			
			}
			else{
				printf("Proceso %d del group %d fue enconlado de nuevo con tiempo restante de %d\n", temp.id, temp.grupo, temp.tiempoCPU);
				encolar(temp.id, temp.tiempoCPU, temp.grupo);
			}	
}

void encolar(int id, int tiempoCPU, int grupo){
	nodo_t *aux=inicio;
	
	if(contProceso==0){
		inicio->proc.id=id;
		inicio->proc.tiempoCPU=tiempoCPU;
		inicio->proc.grupo=grupo;
	}else{
		while(aux->siguiente!=NULL){
			aux=aux->siguiente;
		}
		aux->siguiente=malloc(sizeof(nodo_t));
		aux->siguiente->proc.tiempoCPU=tiempoCPU;
		aux->siguiente->proc.id=id;
		aux->siguiente->proc.grupo=grupo;
		aux->siguiente->siguiente=NULL;
	}
	contProceso++;
}


proceso_t sacarPrimero(){
	proceso_t regreso;
	if(contProceso==1){
		regreso=inicio->proc;
	}else{
	
		nodo_t *nextProc=inicio->siguiente;	
		regreso=inicio->proc;	
		free(inicio);
		inicio=nextProc;
	}
	contProceso--;
	return regreso;
}

//Para el código de la cola nos basamos en un código que realizamos en la materia de sistemas operativos
