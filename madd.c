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

    unsigned int rows = arg->m->nrows;
    unsigned int cols = arg->m->ncols;
    unsigned int id   = arg->id;

    /* split rows as evenly as possible */
    unsigned int base      = rows / NUM_THREADS;
    unsigned int rem       = rows % NUM_THREADS;
    unsigned int start_row = id * base + (id < rem ? id : rem);
    unsigned int row_count = base + (id < rem ? 1u : 0u);

    /* process a single contiguous block for better cache/SIMD */
    TElement *tptr = &arg->t->data[start_row][0];
    TElement *mptr = &arg->m->data[start_row][0];
    TElement *nptr = &arg->n->data[start_row][0];

    unsigned int count = row_count * cols;
    for (unsigned int k = 0; k < count; ++k) {
        tptr[k] = mptr[k] + nptr[k];
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
        args[i].m  = m;
        args[i].n  = n;
        args[i].t  = t;
        int rc = pthread_create(&threads[i], NULL, thread_main, &args[i]);
        if (rc != 0) {
            /* Spec: if a pthread function fails, report error and exit */
            fprintf(stderr, "pthread_create failed (rc=%d)\n", rc);
            freeMatrix(t);
            exit(EXIT_FAILURE);
        }
    }

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "pthread_join failed (rc=%d)\n", rc);
            freeMatrix(t);
            exit(EXIT_FAILURE);
        }
    }
    return t;
}