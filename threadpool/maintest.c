#include "c.h"
#include <unistd.h>

int nthreads = 4;
int nitems = 100;

void *worker(void *arg)
{
    int *val = arg;
    int old = *val;
    
    *val += 1000;
    printf("tid = %ld, old = %d, val = %d\n", pthread_self(), old, *val);
    
    if (*val % 2)
        usleep(100000);
}

int main()
{
    thr_pool_t  *pool;
    int         *vals;

    pool = thr_pool_create(nthreads);
    vals = calloc(nitems, sizeof(*vals));
    for (int i = 0; i < nitems; i++) {
        vals[i] = i;
        thr_pool_queue(pool, worker,vals+i);
    }
    
    thr_pool_wait(pool);
    for (int i = 0; i < nitems; i++)
        printf("%d\n", vals[i]);
    free(vals);
    thr_pool_destroy(pool);
    
    return 0;
}

