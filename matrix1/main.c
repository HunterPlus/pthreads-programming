#include "c.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define MAXDIMS 1000

void printmtx(double *mtx, int m, int n);

int nthreads = 4;
int main(int argc, char *argv[])
{
    int nrows, ndims, ncols;
    double *mtx1, *mtx2, *mtx3;
    clock_t start, end;
    
	if (argc != 4) {
		fprintf(stderr, "arguments error.\n");
		exit(1);
	}    
    nrows = min(atoi(argv[1]), MAXDIMS);
    ndims = min(atoi(argv[2]), MAXDIMS);
    ncols = min(atoi(argv[3]), MAXDIMS);
    mtx1 = mtx_create(nrows, ndims);
    mtx2 = mtx_create(ndims, ncols);
    mtx3 = mtx_create(nrows, ncols);
    
    mtx_rand(mtx1, nrows * ndims);
    mtx_rand(mtx2, ndims * ncols);
/*
    printf("matrix 1:\n");
    printmtx(mtx1, nrows, ndims);
    printf("\nmatrix 2:\n");
    printmtx(mtx2, ndims, ncols);
 */    
    start = clock();
    mtx_mul(mtx1, mtx2, mtx3, nrows, ndims, ncols);
    end = clock();
    printf("\nmatrix 3:\tuse time %ld\n", end - start);
    //printmtx(mtx3, nrows, ncols);
    
    thr_pool_t  *pool;
    pool = thr_pool_create(nthreads);
    start = clock();
    thr_mtx_mul(mtx1, mtx2, mtx3, nrows, ndims, ncols, pool);
    end = clock();
    thr_pool_wait(pool);
    
    printf("\nmatrix 3 with multi threads:\tuse time %ld\n", end - start);
    //printmtx(mtx3, nrows, ncols);

    thr_pool_destroy(pool);
	free(mtx1);
	free(mtx2);
	free(mtx3);
    
    return 0;
}

void printmtx(double *mtx, int m, int n)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) 
            printf("%9.4f", *(mtx + i * n + j));
        printf("\n");
    }
    printf("\n");
}
