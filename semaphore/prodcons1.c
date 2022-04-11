/* Multiple Producers, One Consumer */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define	NBUFF		10
#define	MAXNTHREADS	100
#define min(a,b)	(((a) < (b)) ? (a) : (b))

int 	nitems, nproducers;
struct {
	int	buff[NBUFF];
	int	nput;
	int	nputval;
	sem_t	mutex, nempty, nstored;
} shared;

void	*produce(void *), *consume(void *);

int main(int argc, char *argv[])
{
	int	i, count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume;
	
	if (argc != 3) {
		fprintf(stderr, "usage prodcons1 <#items> <#producers>\n");
		exit(1);
	}
	nitems = atoi(argv[1]);	
	nproducers = min(atoi(argv[2]), MAXNTHREADS);
	
	sem_init(&shared.mutex, 0, 1);
	sem_init(&shared.nempty, 0, NBUFF);
	sem_init(&shared.nstored, 0, 0);
	
	for (i = 0; i < nproducers; i++) {
		count[i] = 0;
		pthread_create(&tid_produce[i], NULL, produce, &count[i]);
	}
	pthread_create(&tid_consume, NULL, consume, NULL);
	
	for (i = 0; i < nproducers; i++) {
		pthread_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);
	}
	pthread_join(tid_consume, NULL);
	
	sem_destroy(&shared.mutex);
	sem_destroy(&shared.nempty);
	sem_destroy(&shared.nstored);
	
	return 0;
}

void *produce(void *arg)
{
	while (1) {
		sem_wait(&shared.nempty);	/* wait for at least 1 slot */
		sem_wait(&shared.mutex);
		
		if (shared.nput >= nitems) {
			sem_post(&shared.nempty);
			sem_post(&shared.mutex);
			return NULL;
		}
		shared.buff[shared.nput % NBUFF] = shared.nputval;
		shared.nput++;
		shared.nputval++;
		
		sem_post(&shared.mutex);
		sem_post(&shared.nstored);	/* 1 more stored item */
		*((int *) arg) += 1;
	}
}

void *consume(void *arg)
{
	int	i;
	
	for (i = 0; i < nitems; i++) {
		sem_wait(&shared.nstored);	/* wait for at least 1 stored item */
		sem_wait(&shared.mutex);
		
		if (shared.buff[i % NBUFF] != i)
			printf("error: buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
		
		sem_post(&shared.mutex);
		sem_post(&shared.nempty);	/* 1 more empty slot */
	}
	return NULL;
}
