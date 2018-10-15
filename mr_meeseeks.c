#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

int main()
{
	char *tarea = (char *)malloc(100);
	int dificultad;
	pid_t pid;
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
				
			}
			else{
				//modo automatico
			}
		}
	}
	return 0;
}

void crearmeeseek(pid){
	
}

void resolvertares(dificultad){
	
}

