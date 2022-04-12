/* Multiple Buffers */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define	NBUFF	8

struct {				/* data shared by producer and consumer */
	struct {
		char data[BUFSIZ];	/* a buffer */
		ssize_t	n;		/* count of #bytes in the buffer */
	} buff[NBUFF];			/* NBUFF of these buffers/counts */
	sem_t	mutex, nempty, nstored;	/* semaphores, not pointers */
} shared;

int	fd;				/* input file to copy to stdout */
void	*produce(void *), *consume(void *);

int main(int argc, char *argv[])
{
	pthread_t tid_produce, tid_consume;
	
	if (argc != 2) {
		fprintf(stderr, "usage: mycat <pathname>");
		exit(1);
	}
	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "file open error.\n");
		exit(1);
	}
	
	sem_init(&shared.mutex, 0, 1);
	sem_init(&shared.nempty, 0, NBUFF);
	sem_init(&shared.nstored, 0, 0);
	
	pthread_create(&tid_produce, NULL, produce, NULL);	/* read thread */
	pthread_create(&tid_consume, NULL, consume, NULL);	/* write thread */
	
	pthread_join(tid_produce, NULL);
	pthread_join(tid_consume, NULL);
	
	sem_destroy(&shared.mutex);
	sem_destroy(&shared.nempty);
	sem_destroy(&shared.nstored);
	
	return 0;
}

void *produce(void *arg)
{
	int	i;
	
	for (i = 0; ;) {
		sem_wait(&shared.nempty);
		
		sem_wait(&shared.mutex);
			/* critical region */
		sem_post(&shared.mutex);
		
		shared.buff[i].n = read(fd, shared.buff[i].data, BUFSIZ);
		if (shared.buff[i].n == 0) {
			sem_post(&shared.nstored);
			return NULL;
		}
		if (++i >= NBUFF)
			i = 0;
		sem_post(&shared.nstored);		
	}
}

void *consume(void *arg)
{
	int	i;
	
	for (i = 0; ;) {
		sem_wait(&shared.nstored);
		
		sem_wait(&shared.mutex);
			/* critical region */
		sem_post(&shared.mutex);
		
		if (shared.buff[i].n == 0)
			return NULL;
		write(1, shared.buff[i].data, shared.buff[i].n);
		if (++i >= NBUFF)
			i = 0;
		
		sem_post(&shared.nempty);
	}
}
