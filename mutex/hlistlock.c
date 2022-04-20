#include <stdlib.h>
#include <pthread.h>

#define NHASH	29
#define HASH(id)	(((unsigned long)id)%NHASH)

struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
	int	count;		/* protected by hashlock */
	pthread_mutex_t	lock;
	int	id;
	struct foo *next;	/* protected by hashlock */
	/* ... more stuff here ... */
};
