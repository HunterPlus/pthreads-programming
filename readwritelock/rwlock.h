#include <stdio.h>
#include <errno.h>
#include <pthread.h>

typedef struct {
	pthread_mutex_t	rw_mutex;
	pthread_cond_t	rw_condreaders;
	pthread_cond_t	rw_condwriters;
	int 	rw_magic;	/* for error checking */
	int	rw_nwaitreaders;
	int 	rw_nwaitwriters;
	int	rw_refcount;	/* -1 if writer has the lock, else readers holding the lock */
} rwlock_t;

#define RW_MAGIC	0x19283746

#define RWLOCK_INITIALIZER	{ PTHREAD_MUTEX_INITIALIZER, 	\
		PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER,	\
		RW_MAGIC, 	0,	0,	0 }		
typedef int rwlockattr_t;	/* dummy, not supported */

int rwlock_init(rwlock_t *, rwlockattr_t *);
int rwlock_destroy(rwlock_t *);
int rwlock_rdlock(rwlock_t *);
int rwlock_tryrdlock(rwlock_t *);
int rwlock_wrlock(rwlock_t *);
int rwlock_trywrlock(rwlock_t *);
int rwlock_unlock(rwlock_t *);
