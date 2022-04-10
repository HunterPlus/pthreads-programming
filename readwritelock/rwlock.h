typedef struct {
	pthread_mutex_t	rw_mutex;
	pthread_cond_t	rw_condreaders;
	pthread_con_t	rw_condwriters;
	int 	rw_magic;	/* for error checking */
	int	rw_nwaitreaders;
	int 	rw_nwaitwriters;
	int	rw_refcount;	/* -1 if writer has the lock, else readers holding the lock */
} pthread_rwlock_t;
