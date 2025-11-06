#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"
#include <errno.h>

// Search TODO to find the locations where code needs to be completed

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

static void * thread_main(void * p_arg)
{
    // TODO
    thread_arg_t *arg = (thread_arg_t *)p_arg;
    TMatrix *m = arg->m;
    TMatrix *n = arg->n;
    TMatrix *t = arg->t;

    unsigned int rows = m->nrows;
    unsigned int id = arg->id;
    unsigned int base = rows / NUM_THREADS;
    unsigned int rem  = rows % NUM_THREADS;

    unsigned int start = id * base + (id < rem ? id : rem);
    unsigned int count = base + (id < rem ? 1U : 0U);
    unsigned int end   = start + count;

    for (unsigned int i = start; i < end; i++) {
        for (unsigned int j = 0; j < n->ncols; j++) {
            TElement sum = (TElement)0;
            for (unsigned int k = 0; k < m->ncols; k++) {
                sum += m->data[i][k] * n->data[k][j];
            }
            t->data[i][j] = sum;
        }
    }
    return NULL;
}

/* Return the sum of two matrices.
 *
 * If any pthread function fails, report error and exit. 
 * Return NULL if anything else is wrong.
 *
 * Similar to mulMatrix, but with multi-threading.
 */
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->ncols != n->nrows )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, n->ncols);
    if (t == NULL)
        return t;

    // TODO
    pthread_t tids[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        args[i].id = i;
        args[i].m = m;
        args[i].n = n;
        args[i].t = t;
        int rv = pthread_create(&tids[i], NULL, thread_main, &args[i]);
        if (rv != 0) {
            errno = rv;
            perror("pthread_create");
            freeMatrix(t);
            exit(EXIT_FAILURE);
        }
    }

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        int rv = pthread_join(tids[i], NULL);
        if (rv != 0) {
            errno = rv;
            perror("pthread_join");
            freeMatrix(t);
            exit(EXIT_FAILURE);
        }
    }
    return t;
}