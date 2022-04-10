#include "rwlock.h"

int pthread_rwlock_init(pthread_rwlock_t *rw, pthread_rwlockattr_t *attr)
{
	int	result;
	
	if (attr != NULL)
		return (EINVAL);	/* not supported */
	
	if ((result = pthread_mutex_init(rw->rw_mutex, NULL)) != 0)
		goto err1;
	if ((result = pthread_cond_init(rw->rw_condreaders, NULL)) != 0)
		goto err2;
	if ((result = pthread_cond_init(rw->rw_condwriters, NULL)) != 0)
		goto erre;
	
}
