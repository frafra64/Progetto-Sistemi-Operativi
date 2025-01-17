//Authors: Francesco Donato Martiello & Mattia Vizza

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

//Constants
#define SEATS 3
#define STUDENTS 5

//Definition of type boolean (cause in C doesn't exists)
typedef enum{FALSE, TRUE} boolean;

//Global variables (integer&boolean)
int nextTA = 0;
int waitingStudents = 0;
int position = 0;
boolean sleeping = FALSE;
int seats[3];

//Global variables (semaphore&pthread)
sem_t semStudents;
sem_t semTA;
pthread_mutex_t mutex;

//Prototypes 
void *threadStudent(void *studentID);
void *threadTA();


int main(int argc, char *argv[]) {
	int totalStudents;
	printf("\n\t***Esecuzione del programma \"Teaching Assistant\"***\n\n");
	sleep(1);
	int studentsIDs[totalStudents];
	pthread_t students[totalStudents];
	pthread_t TA;
	sem_init(&semStudents, 0, 0);
	sem_init(&semTA, 0, 1);
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&TA, NULL, threadTA, NULL);	
	for(int i = 0; i < totalStudents; i++) {
		studentsIDs[i] = i + 1;
		pthread_create(&students[i], NULL, threadStudent, (void*)&studentsIDs[i]);
	}
	pthread_join(TA, NULL);
	for(int i = 0; i < totalStudents; i++) {
		pthread_join(students[i], NULL);
	}
	return 0;
}


void *threadStudent(void *studentID) {
	int id = *(int*)studentID;
	while(1) {
		int time = rand() % 5;
		printf("\tLo studente %d sta programmando. Tempo residuo: %d sec...\n", id, time);
		sleep(time);
		pthread_mutex_lock(&mutex);
		if(waitingStudents < SEATS) {
			seats[position] = id;
			waitingStudents++;
			position = (position + 1) % SEATS;
			printf("Lo studente %d prende posto in corridoio in attesa che l'assistente si liberi.\n", id);
			printf("\tStudenti in attesa: %d.\n", waitingStudents);
			pthread_mutex_unlock(&mutex);
			sem_post(&semStudents);
			sem_wait(&semTA);
		} else { 
			pthread_mutex_unlock(&mutex);
			printf("Attenzione! Tutte le sedie risultano occupate. Lo studente %d tornera' piu' tardi.\n", id);
		}
	}
}