#include "c.h"

void printmtx(double *mtx, int m, int n);

int nthreads = 1;
int main()
{
    double m1[2][2] = {{3, 5}, {4, 7}};
    double m2[2][3] = {{6, 2, 7}, {9, 3, 8}}; 
    double m3[2][3] = {};

    thr_pool_t  *pool;
    pool = thr_pool_create(nthreads);
    
    thr_mtx_mul(&m1[0][0], &m2[0][0], &m3[0][0], 2, 2, 3, pool);
    thr_pool_wait(pool);
    printmtx(&m3[0][0], 2, 3);
    printf("\n\n");
    
    m1[0][0] = 4;
    m2[0][0] = 8;
    thr_mtx_mul(&m1[0][0], &m2[0][0], &m3[0][0], 2, 2, 3, pool);
    thr_pool_wait(pool);
    printmtx(&m3[0][0], 2, 3);
    
    thr_pool_destroy(pool);
    
    return 0;
}

void printmtx(double *mtx, int m, int n)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) 
            printf("%9.4f", *(mtx + i * n + j));
        printf("\n");
    }    
}

