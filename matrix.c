#include "c.h"

struct mul_arg {
    int     *m1;    /*  1 * K vector    */
    int     *m2;    /*  K * N matrix    */
    int     *to;    /*  1 * N vector    */
    int     K;      
    int     N;    
};

void *mul_worker(void *arg);
void vector_mul(int *v1, int *v2, int K, int N, int *to);
void printmtx(int *mtx, int m, int n);

int nthreads = 1;
int main()
{
    int m1[2][2] = {{3, 5}, {4, 7}};
    int m2[2][3] = {{6, 2, 7}, {9, 3, 8}}; 
    int m3[2][3];

    struct mul_arg *v;
    thr_pool_t  *pool;
    
    pool = thr_pool_create(nthreads);
    for (int i = 0; i < 2; i++) {
        v = malloc(sizeof(*v));
        v->m1 = m1[i] + 0;
        v->m2 = &m2[0][0];
        v->to = m3[i] + 0;
        v->K = 2;
        v->N = 3;
        //mul_worker(v);
        thr_pool_queue(pool, mul_worker, v);
    }
    
    thr_pool_wait(pool);
    printmtx(&m3[0][0], 2, 3);
    
    thr_pool_destroy(pool);
    return 0;
}

void *mul_worker(void *arg)
{
    struct mul_arg *v = arg;
    
    for (int j = 0; j < v->N; j++)
        vector_mul(v->m1, v->m2 + j, v->K, v->N, v->to + j);
    free(v);
}

void vector_mul(int *v1, int *v2, int K, int N, int *to)
{
    int sum = 0;
    
    for (int i = 0; i < K; i++)
        sum += v1[i] * v2[i * N];
    *to = sum;
}

void printmtx(int *mtx, int m, int n)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) 
            printf("%-4d", *(mtx + i * n + j));
        printf("\n");
    }    
}