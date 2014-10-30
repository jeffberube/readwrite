/*
 * @Author:	Jeff Berube
 * @Title:	Reader Writer	
 *
 * @Usage	./readwrite
 *
 * @Description:	Implements a solution to the reader writer problem
 * 			with reader bias using semaphores.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define NUM_READERS	4
#define NUM_WRITERS	1

int readcount = 0;
sem_t x, wsem;

/*
 * Reader 
 * 
 * Thread function. Simulates a reader.
 *
 */

void *reader(void *param) {

	/* Initialize variables */
	int threadnum = (int)param;

	/* Seed random number generator */
	srand(time(NULL));

	while (1) {

		/* Sleep before trying to read */
		usleep(threadnum * 3000000);

		/* Down on reader count semaphore and update reader count */
		sem_wait(&x);
		readcount++;

		/* If we're first reader, lock writing semaphore */
		if (readcount == 1) sem_wait(&wsem);

		/* Release reader count semaphore so other readers can come in */
		sem_post(&x);

		/* Display status, sleep and display status again */
		printf("Reader %d: Inside critical section. Reading.\n", threadnum);
		usleep(threadnum * 1500000);
		printf("Reader %d: Done reading. Exiting critical section.\n", threadnum);

		/* Down on reader count semaphore and update reader count */
		sem_wait(&x);
		readcount--;

		/* If we were last reader and there is no readers after us, release
		 * writer semaphore
		 */
		if (readcount == 0) sem_post(&wsem);

		/* Release reader count semaphore */
		sem_post(&x);

	}

	return NULL;

}

/*
 * Writer
 *
 * Thread function. Simulates a write.
 *
 */

void *writer(void *param) {

	/* Variables */
	int threadnum = (int)param;

	/* Seed random number generator */
	srand(time(NULL));

	while (1) {
		
		/* Sleep before writing again */
		usleep(threadnum * 3000000);

		/* Acquire writer semaphore */
		sem_wait(&wsem);

		/* Display status, sleep and display status again */
		printf("Writer %d: Inside critical section. Writing.\n", threadnum);
		usleep(threadnum * 1500000);
		printf("Writer %d: Done writing. Exiting critical section.\n", threadnum);

		/* Release writer semaphore */
		sem_post(&wsem);

	}

	return NULL;
}

int main (int argc, char *argv[]) {

	printf("readwrite started with %d readers and %d writers.\n", NUM_READERS,
			NUM_WRITERS);

	/* Create reader and writer threads */
	pthread_t *threads = malloc((NUM_READERS + NUM_WRITERS) * sizeof(pthread_t));

	/* Thread variables */
	pthread_t tid;
	pthread_attr_t attr;

	/* Init semaphores */
	sem_init(&x, 0, 1);
	sem_init(&wsem, 0, 1);

	/* Initiate and create thread */
	pthread_attr_init(&attr);

	int i = 0;

	/* Create reader threads */
	while (i < NUM_READERS) {
		
		pthread_create(threads + i, &attr, reader, (void *)i + 1);
		i++;

	}

	i = 0;
	
	/* Create writer threads */
	while (i < NUM_WRITERS) {
	
		pthread_create(threads + i + NUM_READERS, &attr, writer, (void *)i + 1);
		i++;

	}

	i = 0;

	/* Wait on threads to finish. Never happens but program exits if we don't. */
	while (i < (NUM_WRITERS + NUM_READERS)) {
	
		pthread_join(*(threads + i), NULL);
		i++;

	}

	return  0;

}
