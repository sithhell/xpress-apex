#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"apex.h"

#define N 10000000
#define MAX_THREADS 32

#if defined(__GNUC__)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#else
#define ALIGNED_(x) __declspec(align(x))
#endif

double openmp_reduction(double* x, double* y)
{
  double sum=0.0;
  #pragma omp parallel
  {
   int i;
   #pragma omp for reduction( + : sum )
   for (i = 0; i < N; i++) {
     #pragma omp atomic
     sum += (x[i] * y[i]);
   }

  }
  return sum;
}

double true_sharing(double* x, double* y)
{
  double sum=0.0;
  #pragma omp parallel
  {
   int i;
   #pragma omp for
   for (i = 0; i < N; i++) {
     #pragma omp atomic
     sum += (x[i] * y[i]);
   }

  }
  return sum;
}

double false_sharing(double* x, double* y)
{
  double sum=0.0;
  double sum_local[MAX_THREADS] = {0};
  #pragma omp parallel
  {
   int me = omp_get_thread_num();

   int i;
   #pragma omp for
   for (i = 0; i < N; i++) {
     sum_local[me] = sum_local[me] + (x[i] * y[i]);
   }

   #pragma omp atomic
   sum += sum_local[me];
  }
  return sum;
}

double no_sharing(double* x, double* y)
{
  double sum=0.0;
  ALIGNED_(128) double sum_local[MAX_THREADS] = {0};
  #pragma omp parallel
  {
   int me = omp_get_thread_num();

   int i;
   #pragma omp for
   for (i = 0; i < N; i++) {
     sum_local[me] = sum_local[me] + (x[i] * y[i]);
   }

   #pragma omp atomic
   sum += sum_local[me];
  }
  return sum;
}

void my_init(double* x)
{
  double randval = 1.0 + (((double)(rand())) / RAND_MAX);
  #pragma omp parallel
  {
   int i;
   #pragma omp for
   for (i = 0; i < N; i++) {
     x[i] = randval;
   }
  }
}

int main(int argc, char** argv)
{
  static double x[N];
  static double y[N];
  apex_init_args(argc, argv, "openmp test");
  apex_set_node_id(0);
  printf("Initializing...\n"); fflush(stdout);
  my_init(x);
  printf("Initializing...\n"); fflush(stdout);
  my_init(y);

  double result = 0.0;
#if 1
  printf("True sharing...\n"); fflush(stdout);
  result = true_sharing(x, y);
  printf("Result: %f\n", result);

  printf("Reduction sharing...\n"); fflush(stdout);
  result = openmp_reduction(x, y);
  printf("Result: %f\n", result);

  printf("False sharing...\n"); fflush(stdout);
  result = false_sharing(x, y);
  printf("Result: %f\n", result);
#endif

  printf("No Sharing...\n"); fflush(stdout);
  result = no_sharing(x, y);
  printf("Result: %f\n", result);

  apex_finalize();
  return 0;
}
