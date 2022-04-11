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
	int	nvalue;
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
}
