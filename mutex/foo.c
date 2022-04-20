#include <stdlib.h>
#include <pthread.h>

struct foo {
	int	count;
	pthread_mutex_t mutex;
	int	id;
	/* ... more stuff hear ... */	
};
struct foo *foo_alloc(int id)	/* alloc the object */
{
	struct foo *fp;
	
	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->count = 1;
		fp->id = id;
		if (pthread_mutex_init(&fp->mutex, NULL) != 0) {
			free(fp);
			return (NULL);
		}
		/* ... continue initialization ... */
	}
	return (fp);
}
void foo_hold(struct foo *fp)	/* add a reference to the object */
{
	pthread_mutex_lock(&fp->mutex);
	fp->count++;
	pthread_mutex_unlock(&fp->mutex);
}
void foo_rele(struct foo *fp)	/* release a reference to the object */
{
	pthread_mutex_lock(&fp->mutex);
	if (--fp->count == 0) {		/* last reference */
		pthread_mutex_unlock(&fp->mutex);
		pthread_mutex_destroy(&fp->mutex);
		free(fp);
	} else 
		pthread_mutex_unlock(&fp->mutex);
}
