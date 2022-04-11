/* Multiple Producers, Multiple Consumers */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NBUFF		10
#define MAXNTHREADS	100

int 	nitems, nproducers, nconsumers;

struct {
	int	buff[NBUFF];
	int	nput;
	int	nputval;
	int	nget;
	int	ngetval;
	sem_t	mutex, nempty, nstored;
} shared;

void *produce(void *), *consume(void *);

int main(int argc, char *argv[])
{
}
