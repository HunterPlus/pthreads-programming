#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NBUFF	10

int 	nitems;
struct {
	int	buff[NBUFF];
	sem_t	mutex, nempty, nstored;
} shared;

void *produce(void *), *consume(void *);

int main(int argc, char *argv[])
{
}
