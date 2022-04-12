/* Multiple Buffers */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define	NBUFF	8

struct {				/* data shared by producer and consumer */
	struct {
		char data[BUFFSIZ];	/* a buffer */
		ssize_t	n;		/* count of #bytes in the buffer */
	} buff[NBUFF];			/* NBUFF of these buffers/counts */
	sem_t	mutex, nempty, nstored;	/* semaphores, not pointers */
} shared;

