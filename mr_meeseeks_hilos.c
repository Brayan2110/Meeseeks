#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

int contador = 0;
int nn = 0;
int ii = 0;
long int pp;
bool estado;
bool paso = false;
long int padre;
long int padre2;
int dificultad;
char *tarea;
static clock_t tiempo;

//pid_t pid;
static sem_t semaforo;

void estadotarea(){
	printf("Tarea completada por el Meeseek: (%ld,%ld,%d,%d)\n",pp, padre2, nn, ii);
}

//threat resolver
void* resolvertarea(void *arg){
	int n = nn+0;
	int i = ii+0;
	long int papa = pp; 
	int cantidadhijos = 0;
	printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%ld,%ld,%d,%d)\n",pthread_self(), papa, n, i);
	sleep(1);
	tiempo = clock();
	sem_post(&semaforo);
	while(true){
		sem_wait(&semaforo);
		if(estado == true){
			if(pthread_self() == padre){
				while(contador!=0){
					sleep(1);
				}
			}
			printf("Chao: (%ld,%ld,%d,%d)\n",pthread_self(), papa, n, i);
			sleep(1);
			sem_post(&semaforo);
			wait(0);
			contador--;
			pthread_exit(NULL);
		}
		else{
			if(paso == false && (double)(clock()-tiempo)/CLOCKS_PER_SEC < 0.001000){
				int numero = rand() % 100;
				if(numero < dificultad){
					estado = true;
					nn = n;
					ii = i;
					padre2 = papa;
					pp = pthread_self();
					printf("Tarea completa, soy el Mr. Meeseeks: (%ld,%ld,%d,%d)\n",pthread_self(), papa, n, i);
					sleep(1);
					sem_post(&semaforo);
				}
				else{
					printf("No lo logre: (%ld,%ld,%d,%d)\n",pthread_self(), papa, n, i);
					
				}
				sleep(1);
				sem_post(&semaforo);
				sleep(1);
			}
			else{
				paso=true;
				nn = n+1;
				pp = pthread_self();
				ii = cantidadhijos+0;
				cantidadhijos++;
				pthread_t hilo;
				contador++;
				pthread_create(&hilo, NULL, resolvertarea, NULL);
				paso=false;
			}
		}
		
	}
	return NULL;
	
}

void * crearpadre(){
	padre = pthread_self();
	resolvertarea(NULL);
}

int main()
{
	srand(time(NULL));
	tarea = (char *)malloc(100);
	printf("Bienvenido al programa de Mr. Meeseeks\n\n");
	char letra;
	while(true){
		estado = false;
		sem_init(&semaforo, 0, 0);
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
				pp = pthread_self();
				//hilo crear padre
				pthread_t hiloCrearPadre;
				pthread_create(&hiloCrearPadre,NULL,crearpadre,NULL);
				pthread_join(hiloCrearPadre, NULL);
				estadotarea();
				
			}
			else{
				dificultad = (rand() % 101);
				printf("La dificultad es: %d\n", dificultad);
				printf("Asigne una tarea al Mr. Meeseek: ");
				scanf("%100[^\n]", tarea);
				getchar();
				printf("La tarea es: %s\n", tarea);
				pp = pthread_self();
				//hilo crear padre
				pthread_t hiloCrearPadre;
				pthread_create(&hiloCrearPadre,NULL,crearpadre,NULL);
				pthread_join(hiloCrearPadre, NULL);
				estadotarea();
			}
		}
	}
	return 0;
}
