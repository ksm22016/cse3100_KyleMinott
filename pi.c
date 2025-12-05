#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N_THREADS 2
#define SAMPLE_SIZE 10000000

typedef struct thread_arg_tag {
  int id;
  int count;
  int total;
} thread_arg_t;

double rand_double(unsigned int *s) { return (rand_r(s) / (double)RAND_MAX); }

void *thread_main(void *thread_arg) {}

int main(int argc, char *argv[]) {

  int n = SAMPLE_SIZE;
  int n_threads = N_THREADS;
  
  int total_count = 0;

  if (argc > 1) {
    n = atoi(argv[1]);
  }
  if (argc > 2) {
    n_threads = atoi(argv[2]);
  }

  pthread_t thread_arr[n_threads];
  thread_arg_t thread_args[n_threads];

  pthread_create (&thread_arr[n_threads], NULL);
  double pi = 4.0 * total_count / (double)n;

  pthread_join (&thread_arr[n_threads], NULL);
  printf("pi = %.2f\n", pi);

  return 0;
}