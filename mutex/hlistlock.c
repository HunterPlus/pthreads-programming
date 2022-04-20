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

struct foo *foo_alloc(int id)	/* allocate the object */
{
	struct foo	*fp;
	int		idx;
	
	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->count = 1;
		fp->id = id;
		if (pthread_mutex_init(&fp->lock, NULL) != 0) {
			free(fp);
			return (NULL);
		}
		idx = HASH(id);
		pthread_mutex_lock(&hashlock);
		fp->next = fh[idx];
		fh[idx] = fp;
		pthread_mutex_lock(&fp->lock);
		pthread_mutex_unlock(&hashlock);
		/* ... continue initialization ... */
		pthread_mutex_unlock(&fp->lock);
	}
	return (fp);
}

void foo_hold(struct foo *fp)	/* add reference to the object */
{
	pthread_mutex_lock(&hashlock);
	fp->count++;
	pthread_mutex_unlock(&hashlock);
}

struct foo *foo_find(int id)	/* find an existing object */
{
	struct foo *fp;
	
	pthread_mutex_lock(&hashlock);
	for (fp = fh[HASH(id)]; fp != NULL; fp = fp->next) {
		if (fp->id == id) {
			fp->count++;
			break;
		}
	}
	pthread_mutex_unlock(&hashlock);
	return (fp);
}

void foo_rele(struct foo *fp)	/* release a reference to the object */
{
	struct foo	*tfp;
	int		idx;
	
	pthread_mutex_lock(&hashlock);
	if (--fp->count == 0) {	/* last reference, remove from the list */
		idx = HASH(fp->id);
		tfp = fh[idx];
		if (tfp == fp) {
			fh[idx] = fp->next;
		} else {
			while (tfp->next != fp)
				tfp = tfp->next;
			tfp->next = fp->next;
		}
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_destroy(&fp->lock);
		free (fp);
	} else {
		pthread_mutex_unlock(&hashlock);
	}
}
