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


typedef int bool;
#define true 1
#define false 0

bool estado = false;
bool paso = false;
int padre;
int dificultad;
char *tarea;
clock_t tiempo;
int n=0;
int i=0;
int cantidadhijos=0;

sem_t *semaforo;


void estadotarea(){
	
}

void crearmeeseek(){
	int pp = getpid();
	fork();
	if(getpid() == pp){
		int valn;
		int vali;
		mkfifo("pipen",0666);
		valn = open("pipen",O_WRONLY);
		int temp = n+1;
		write(valn,&temp,sizeof(n));
		mkfifo("pipei",0666);
		vali = open("pipei",O_WRONLY);
		cantidadhijos++;
		write(vali,&cantidadhijos,sizeof(cantidadhijos));
		tiempo=clock();
	}
	if(getpid() != pp){
		int valn;
		int vali;
		int nn;
		int ii;
		valn = open("pipen",O_RDONLY);
		read(valn,&nn,sizeof(nn));
		n = nn;
		vali = open("pipei",O_RDONLY);
		read(vali,&ii,sizeof(ii));
		i = ii;
		cantidadhijos=0;
		printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);
		sem_post(semaforo);
	}
	paso=false;
}

void resolvertarea(){
	while(estado == false){
		sem_wait(semaforo);
		if(estado == true){
			printf("Chao: (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);
			sem_post(semaforo);
			kill(getpid(),1);
		}
		else if(paso == false && (double)(clock()-tiempo)/CLOCKS_PER_SEC < 0.001000){
			int numero = rand() % 100;
			if(numero < dificultad){
				estado = true;
				printf("Tarea completa: (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);
				sem_post(semaforo);
				kill(getpid(),1);
			}
			else{
				printf("No lo logre: (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);
			}
			sleep(1);
			sem_post(semaforo);
		}
		else{
			//printf("este paso: (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);  //borrarlo despues
			paso=true;
			crearmeeseek();
		}
		
	}
	
}

void crearpadre(){
	padre = getpid();
	fork();
	if(padre != getpid()){
		printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);
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
	//sem_init(&semaforo, 1, 1);
	semaforo = sem_open("pSem", O_CREAT | O_EXCL, 0644, 1);
	tarea = (char *)malloc(100);
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
