#include "c.h"

static job_t *get_job(thr_pool_t *pool)
{
    job_t   *job;
    
    if ((job = pool->head) != NULL) {
        pool->head = job->next;
        if (job == pool->tail)
            pool->tail = NULL;
    }
    return job;
}

static void *worker(void *arg)
{
    thr_pool_t  *pool = arg;
    job_t       *job;
    
    while (1) {
        pthread_mutex_lock(&pool->mutex);
        while (pool->head == NULL && !pool->stop) 
            pthread_cond_wait(&pool->workcond, &pool->mutex);
        if (pool->stop)
            break;
        pool->active++;
        job = get_job(pool);
        pthread_mutex_unlock(&pool->mutex);
        
        if (job != NULL) {
            job->func(job->arg);
            free(job);
        }
        
        pthread_mutex_lock(&pool->mutex);
        pool->active--;
        if (!pool->stop && pool->active == 0 && pool->head == NULL)
            pthread_cond_signal(&pool->waitcond);
        pthread_mutex_unlock(&pool->mutex);
    }
    pool->nthreads--;
    pthread_cond_signal(&pool->waitcond);
    pthread_mutex_unlock(&pool->mutex);
    return NULL;
}

thr_pool_t *thr_pool_create(size_t num)
{
    thr_pool_t  *pool;
    pthread_t   tid;
    
    pool = calloc(1, sizeof(*pool));
    pool->nthreads = (num <= 0) ? 2 : num;
    
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->workcond, NULL);
    pthread_cond_init(&pool->waitcond, NULL);
    
    for (int i = 0; i < num; i++) {
        pthread_create(&tid, NULL, worker, pool);
        pthread_detach(tid);
    }
    return pool;
}

void thr_pool_destroy(thr_pool_t *pool)
{
    job_t   *job;
    
    pthread_mutex_lock(&pool->mutex);
    for (job = pool->head; job != NULL; job = pool->head) {
        pool->head = job->next;
        free(job);
    }
    pool->stop = 1;
    pthread_cond_broadcast(&pool->workcond);
    pthread_mutex_unlock(&pool->mutex);
    
    thr_pool_wait(pool);
    
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->workcond);
    pthread_cond_destroy(&pool->waitcond);
    
    free(pool);
}

void thr_pool_wait(thr_pool_t *pool)
{
    pthread_mutex_lock(&pool->mutex);
    
    while (!pool->stop && pool->active || pool->stop && pool->nthreads)
        pthread_cond_wait(&pool->waitcond, &pool->mutex);
    pthread_mutex_unlock(&pool->mutex);
}

int thr_pool_queue(thr_pool_t *pool, void *(*func)(void *), void *arg)
{
    job_t   *job;
    
    if ((job = malloc(sizeof(*job))) == NULL) {
        fprintf(stderr, "thr_pool_queue: malloc error.\n");
        exit(1);
    }
    job->next = NULL;
    job->func = func;
    job->arg = arg;
    
    pthread_mutex_lock(&pool->mutex);
    if (pool->head == NULL)
        pool->head = job;
    else
        pool->tail->next = job;
    pool->tail = job;
    pthread_cond_broadcast(&pool->workcond);
    pthread_mutex_unlock(&pool->mutex);
    return 0;
}