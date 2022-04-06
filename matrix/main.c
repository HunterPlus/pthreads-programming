#include "c.h"

void printmtx(int *mtx, int m, int n);

int nthreads = 4;
int main()
{
    int m1[2][2] = {{3, 5}, {4, 7}};
    int m2[2][3] = {{6, 2, 7}, {9, 3, 8}}; 
    int m3[2][3] = {};

    thr_pool_t  *pool;
    pool = thr_pool_create(nthreads);
    
    thr_mtx_mul(&m1[0][0], &m2[0][0], &m3[0][0], 2, 2, 3, pool);
    
    thr_pool_wait(pool);
    thr_pool_destroy(pool);
    
    printmtx(&m3[0][0], 2, 3);
    return 0;
}

void printmtx(int *mtx, int m, int n)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) 
            printf("%-4d", *(mtx + i * n + j));
        printf("\n");
    }    
}
