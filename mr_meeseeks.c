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

static int *cantidad;
static bool *estado;
bool paso = false;
int padre;
int dificultad;
char *tarea;
clock_t tiempo;
int n=0;
int i=0;
int cantidadhijos=0;

pid_t pid;
static sem_t *semaforo;

void estadotarea(){
	int valn;
	int valpid;
	int valppid;
	int nn;
	int ii;
	valn = open("pipepid",O_RDONLY);
	read(valn,&valpid,sizeof(nn));
	
	valn = open("pipeppid",O_RDONLY);
	read(valn,&valppid,sizeof(ii));

	valn = open("pipen",O_RDONLY);
	read(valn,&nn,sizeof(ii));

	valn = open("pipei",O_RDONLY);
	read(valn,&ii,sizeof(ii));
	wait(0);
        printf("Tarea completada por el Meeseek: (%d,%d,%d,%d)\n",valpid,valppid,nn,ii);
}

void crearmeeseek(){
	int pp = getpid();
	pid = fork();
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
		sleep(1);
	}
	paso=false;
}

void resolvertarea(){
	while(true){
		sem_wait(semaforo);
		if(*estado == true){
			printf("Chao: (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);
			sleep(1);
			sem_post(semaforo);
			wait(0);
			kill(getpid(),1);
		}
		else if(paso == false && (double)(clock()-tiempo)/CLOCKS_PER_SEC < 0.001000){
			int numero = rand() % (dificultad + 1);
			*cantidad = *cantidad - numero;
			if(*cantidad < 0){
				sleep(1);
				*estado = true;
				printf("Tarea completa, soy el Mr. Meeseeks: (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);
				sleep(1);
				sem_post(semaforo);
				//pipe
				int pid;
				int ppid;
				int nn;
				int ii;
				int varpid;
				int varppid;
				mkfifo("pipepid",0666);
				pid = open("pipepid",O_WRONLY);
				varpid = getpid();
				write(pid,&varpid,sizeof(n));
				mkfifo("pipeppid",0666);
				ppid = open("pipeppid",O_WRONLY);
				varppid = getppid();
				write(ppid,&varppid,sizeof(n));
				mkfifo("pipen",0666);
				nn = open("pipen",O_WRONLY);
				write(nn,&n,sizeof(n));

				mkfifo("pipei",0666);
				ii = open("pipei",O_WRONLY);
				write(ii,&i,sizeof(n));
				//pipe
			}
			else{
				printf("No lo logre: (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);
				
			}
			sleep(1);
			sem_post(semaforo);
			sleep(1);
		}
		else{
			paso=true;
			crearmeeseek();
		}
		
	}
	
}

void crearpadre(){
	padre = getpid();
	pid = fork();
	if(padre != getpid()){
		printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d,%d,%d,%d)\n",getpid(), getppid(), n, i);
		sleep(1);
		padre = getpid();
		tiempo = clock();
		resolvertarea();
	}
	else{
		estadotarea();
	}
	
}

int main()
{
	srand(time(NULL));
	cantidad = mmap(NULL, sizeof *cantidad, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*cantidad = 300;
	estado = mmap(NULL, sizeof *estado, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	semaforo = mmap(NULL, sizeof *semaforo, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	tarea = (char *)malloc(100);
	printf("Bienvenido al programa de Mr. Meeseeks\n\n");
	char letra;
	while(true){
		*estado = false;
		sem_init(semaforo, 1, 1);
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
				dificultad = (rand() % 101);
				printf("La dificultad es: %d\n", dificultad);
				printf("Asigne una tarea al Mr. Meeseek: ");
				scanf("%100[^\n]", tarea);
				getchar();
				printf("La tarea es: %s\n", tarea);
				padre = getpid();
				crearpadre();
			}
		}
	}
	return 0;
}
