#include "rwlock.h"

int rwlock_init(rwlock_t *rw, rwlockattr_t *attr)
{
	int	result;
	
	if (attr != NULL)
		return (EINVAL);	/* not supported */
	
	if ((result = pthread_mutex_init(&rw->rw_mutex, NULL)) != 0)
		goto err1;
	if ((result = pthread_cond_init(&rw->rw_condreaders, NULL)) != 0)
		goto err2;
	if ((result = pthread_cond_init(&rw->rw_condwriters, NULL)) != 0)
		goto err3;
	rw->rw_nwaitreaders = 0;
	rw->rw_nwaitwriters = 0;
	rw->rw_refcount = 0;
	rw->rw_magic = RW_MAGIC;
	
	return 0;
	
err3:
	pthread_cond_destroy(&rw->rw_condreaders);
err2:
	pthread_mutex_destroy(&rw->rw_mutex);
err1:
	return result;		/* an errno value */
}

int rwlock_destroy(rwlock_t *rw)
{
	if (rw->rw_magic != RW_MAGIC)
		return (EINVAL);
	if (rw->rw_refcount != 0 || rw->rw_nwaitreaders != 0 || rw->rw_nwaitwriters != 0)
		return (EBUSY);
	
	pthread_mutex_destroy(&rw->rw_mutex);
	pthread_cond_destroy(&rw->rw_condreaders);
	pthread_cond_destroy(&rw->rw_condwriters);
	rw->rw_magic = 0;
	
	return 0;
}

int rwlock_rdlock(rwlock_t *rw)
{
	int	result;
	
	if (rw->rw_magic != RW_MAGIC)
		return (EINVAL);
	
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return result;
	
	while (rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0) {
		rw->rw_nwaitreaders++;
		result = pthread_cond_wait(&rw->rw_condreaders, &rw->rw_mutex);
		rw->rw_nwaitreaders--;
		if (result != 0)
			break;
	}
	if (result == 0)
		rw->rw_refcount++;
	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}

int rwlock_tryrdlock(rwlock_t *rw)
{
	int	result;
	
	if (rw->rw_magic != RW_MAGIC)
		return (EINVAL);
	
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return (result);
	
	if (rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0)
		result = EBUSY;
	else
		rw->rw_refcount++;
	pthread_mutex_unlock(&rw->rw_mutex);
	
	return (result);
}

int rwlock_wrlock(rwlock_t *rw)
{
	int	result;
	
	if (rw->rw_magic != RW_MAGIC)
		return (EINVAL);
	
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return (result);
	
	while (rw->rw_refcount != 0) {
		rw->rw_nwaitwriters++;
		result = pthread_cond_wait(&rw->rw_condwriters, &rw->rw_mutex);
		rw->rw_nwaitwriters--;
		if (result != 0)
			break;
	}
	if (result == 0)
		rw->rw_refcount = -1;
	pthread_mutex_unlock(&rw->rw_mutex);
	
	return (result);
}

int rwlock_trywrlock(rwlock_t *rw)
{
	int 	result;
	
	if (rw->rw_magic != RW_MAGIC)
		return (EINVAL);
	
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return (result);

	if (rw->rw_refcount != 0)
		return (EBUSY);
	else
		rw->rw_refcount = -1;
	pthread_mutex_unlock(&rw->rw_mutex);
	
	return (result);
}

int rwlock_unlock(rwlock_t *rw)
{
	int	result;
	
	if (rw->rw_magic != RW_MAGIC)
		return (EINVAL);
	
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return (result);
	
	if (rw->rw_refcount > 0)
		rw->rw_refcount--;
	else if (rw->rw_refcount == -1)
		rw->rw_refcount = 0;
	else
		fprintf(stderr, "rwlock_unlock() err: rw_refcount = %d\n", rw->rw_refcount);
	
	if (rw->rw_nwaitwriters > 0) {
		if (rw->rw_refcount == 0)
			result = pthread_cond_signal(&rw->rw_condwriters);
	} else if (rw->rw_nwaitreaders > 0)
		result = pthread_cond_broadcast(&rw->rw_condreaders);
	
	pthread_mutex_unlock(&rw->rw_mutex);
	return (result);
}
