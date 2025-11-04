#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

/* the main function of threads */
static void * thread_main(void * p_arg)
{
    // TODO
    thread_arg_t *arg = (thread_arg_t *)p_arg;

    TMatrix *m = arg->m;
    TMatrix *n = arg->n;
    TMatrix *t = arg->t;

    unsigned int rows = m->nrows;
    unsigned int cols = m->ncols;
    unsigned int id   = arg->id;

    unsigned int base = rows / NUM_THREADS;
    unsigned int rem  = rows % NUM_THREADS;

    unsigned int start = id * base + (id < rem ? id : rem);
    unsigned int count = base + (id < rem ? 1u : 0u);
    unsigned int end   = start + count;

    for (unsigned int i = start; i < end; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            t->data[i][j] = m->data[i][j] + n->data[i][j];
        }
    }
    return NULL;
}

/* Return the sum of two matrices. The result is in a newly creaed matrix. 
 *
 * If a pthread function fails, report error and exit. 
 * Return NULL if something else is wrong.
 *
 * Similar to addMatrix, but this function uses 2 threads.
 */
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;

    // TODO
    pthread_t threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        args[i].id = i;
        args[i].m = m;
        args[i].n = n;
        args[i].t = t;
        int rc = pthread_create(&threads[i], NULL, thread_main, &args[i]);
        if (rc != 0) {
            perror("pthread_create");
            freeMatrix(t);
            exit(EXIT_FAILURE);
        }
    }

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            perror("pthread_join");
            freeMatrix(t);
            exit(EXIT_FAILURE);
        }
    }
    return t;
}