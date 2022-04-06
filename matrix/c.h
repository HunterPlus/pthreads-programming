#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct thr_pool thr_pool_t;
typedef struct job job_t;
struct job {
    job_t   *next;
    void    *(*func)(void *);
    void    *arg;
};

thr_pool_t *thr_pool_create(size_t num);
void thr_pool_wait(thr_pool_t *pool);
void thr_pool_destroy(thr_pool_t *pool);
int thr_pool_queue(thr_pool_t *pool, void *(*func)(void *), void *arg);

struct thr_pool {
    pthread_mutex_t mutex;
    pthread_cond_t  workcond;
    pthread_cond_t  waitcond;
    int             active;
    job_t           *head;
    job_t           *tail;
    int             nthreads;
    int             stop;
};

/***********************************************************
 *matrix header
 * 
 */
 
struct job_arg {
    int     *va;        /* 1 * K vector     */
    int     *vb;        /* K * 1 vector     */
    int     *to;        /* vector mul result    */
    int     K;
    int     N;          /* vector vb's matrix cols  */
};
void thr_mtx_mul(int *, int *, int *, int, int, int, thr_pool_t *);
