#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>


typedef int bool;
#define true 1
#define false 0

bool estado = false;
int padre;
int dificultad;
char *tarea;
clock_t tiempo;
int n=0;
int i=0;
int cantidadhijos=0;

sem_t semaforo;


void estadotarea(){
	
}

void crearmeeseek(){
	fork();
	if(getpid() != padre){
		//pasar i
		//pasar n
		printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d,%d)\n",getpid(), getppid());
	}
	if(grtpid() == padre){
		cantidadhijos++;
	}
	tiempo=clock();
}

void resolvertarea(){
	while(estado == false){
		if(getpid() != padre || (double)(clock()-tiempo)/CLOCKS_PER_SEC < 0.001000){
			//printf("Tiempo es: %f\n",(double)(clock()-tiempo)/CLOCKS_PER_SEC);
			//semaforo
			int numero = rand() % 100;
			if(numero < dificultad){
				estado = true;
				printf("Lo logre: (%d,%d)\n",getpid(), getppid());
				kill(getpid(),1);
				//crear pipe, indicarle al padre 
				//se logro la tarea
				//matar meeseeks
			}
			else{
				printf("No lo logre: (%d,%d)\n",getpid(), getppid());
			}
			sleep(1);
		}
		else{
			crearmeeseek();
		}
	}
	
}

void crearpadre(){
	padre = getpid();
	fork();
	if(padre != getpid()){
		printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d,%d)\n",getpid(), getppid());
		padre = getpid();
		tiempo = clock();
		resolvertarea();
	}
	if(padre != getpid()){
		estadotarea();
	}
	
}

int main()
{
	tarea = (char *)malloc(100);
	sem_init(&semaforo, 0, 1);
	pid_t pid;
	//printf("El principal es: %d\n", getpid());
	printf("Bienvenido al programa de Mr. Meeseeks\n\n");
	char letra;
	while(true){
		printf("Si desea asignar una tarea a un Mr. Meeseek pulse la letra m y pulse enter: ");
		scanf("%c", &letra);
		getchar();
		if(letra=='m'){
			printf("Desea asignar la dificultad de la tarea (pulse la letra s y luego enter para si) si se asigna otra letra se asignara aleatoriamente: ");
			scanf("%c", &letra);
			getchar();
			if(letra=='s'){
				bool paso = false;
				while(!paso){
					printf("Asigne la dificultad de la tarea (numero del 0 al 100): ");
					scanf("%d", &dificultad);
					getchar();
					if(dificultad >=0 && dificultad <=100){
						paso = true;
					}
				}
				printf("Asigne una tarea al Mr. Meeseek: ");
				scanf("%100[^\n]", tarea);
				getchar();
				printf("La tarea es: %s\n", tarea);
				padre = getpid();
				crearpadre();
			}
			else{
				//modo automatico
			}
		}
	}
	return 0;
}
