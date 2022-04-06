#include "c.h"

static struct job_arg *wrap_arg(int *va, int *vb, int *to, int K, int N)
{
    struct job_arg *arg = malloc(sizeof(*arg));
    
    arg->va = va;
    arg->vb = vb,
    arg->to = to;
    arg->K  = K;
    arg->N  = N;
        
    return arg;
}

static void *thr_vector_mul(void *job_arg)
{
    struct job_arg *arg = job_arg;
    int sum = 0;
    
    for (int i = 0; i < arg->K; i++)
        sum += arg->va[i] * arg->vb[i * arg->N];
    *arg->to = sum;
    free(arg);
}

void thr_mtx_mul(int *a, int *b, int *c, int M, int K, int N, thr_pool_t *pool)
{
    int i, j;
    struct job_arg *arg;
    
    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++) {
            arg = wrap_arg(a + i * K, b + j, c + i * N + j, K, N);
            //thr_vector_mul(arg);
            thr_pool_queue(pool, thr_vector_mul, arg);   
        }
    return ;
}
