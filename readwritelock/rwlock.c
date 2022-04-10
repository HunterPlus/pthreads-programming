#include "rwlock.h"

int pthread_rwlock_init(pthread_rwlock_t *rw, pthread_rwlockattr_t *attr)
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

int pthread_rwlock_destroy(pthread_rwlock_t *rw)
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

int pthread_rwlock_rdlock(pthread_rwlock_t *rw)
{
	int	result;
	
	if (rw->rw_magic != RW_MAGIC)
		return (EINVAL);
	
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return result;
	
	while (rw->rw_refcound < 0 || rw->rw_nwaitwriters > 0) {
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

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rw)
{
	int	result;
	
	if (rw->rw_magic != RW_MAGIC)
		return (EINVAL);
	
	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return (result);
	
	if (rw->rw_refcount < 0 || rw->rw_nwaitrwiters > 0)
		result = EBUSY;
	else
		rw->rw_refcount++;
	pthread_mutex_unlock(&rw->rw_mutex);
	
	return (result);
}

int pthread_rwlock_wrlock(pthread_rwlock_t *rw)
{
	int	result;
	
	if (rw->rw_magic != RW_MAGIC)
		return (EINVAL);
	
	if ((result = pthread_mutex_lock(&rw_mutex)) != 0)
		return (result);
	
	while (rw->refcount != 0) {
		rw->rwnwaitwriters++;
		result = pthread_cond_wait(rw->rw_condwriters, &rw->rw_mutex);
		rw->rwnwaitwriters--;
		if (result != 0)
			break;
	}
	if (result == 0)
		rw->rw_refcount = -1;
	pthread_mutex_unlock(&rw->rw_mutex);
	
	return (result);
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rw)
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

int pthread_rwlock_unlock(pthread_rwlock_t *rw)
{
}
