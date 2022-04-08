#include "c.h"

static struct job_arg *wrap_arg(double *va, double *vb, double *to, int K, int N)
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
    double sum = 0;
    
    for (int i = 0; i < arg->K; i++)
        sum += arg->va[i] * arg->vb[i * arg->N];
    *arg->to = sum;
    free(arg);
}

/* a: M x K,  b: K x N,  c: M x N */
void thr_mtx_mul(double *a, double *b, double *c, int M, int K, int N, thr_pool_t *pool)
{
    int i, j;
    struct job_arg *arg;
    
    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++) {
            arg = wrap_arg(a + i * K, b + j, c + i * N + j, K, N);
            thr_pool_queue(pool, thr_vector_mul, arg);   
        }
    return ;
}
void mtx_mul(double *a, double *b, double *c, int M, int K, int N)
{
    int i, j;
    struct job_arg *arg;
    
    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++) {
            arg = wrap_arg(a + i * K, b + j, c + i * N + j, K, N);
            thr_vector_mul(arg);
        }
    return ;
}


void *mtx_create(int nrows, int ncols)
{
    double *mtx;
    
    mtx = calloc(nrows * ncols, sizeof(*mtx));
    if (mtx == NULL) {
        fprintf(stderr, "matrix create error.\n");
        exit(1);
    }
    return mtx;
}
void mtx_rand(double *mtx, int n)
{
    int mode, num;
    
    mode = 10000;
    srand(time(0));
    while (--n >= 0) {
        num = rand() % mode;
        mtx[n] = 1.0 * num / mode;
    }
}
