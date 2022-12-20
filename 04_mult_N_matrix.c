#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 7 

int mat1[N][N];
int mat2[N][N]; 
int result[N][N];

typedef struct {
  int i;
  int j; 
} parameters;

void *multiply(void *param) {
  parameters *p = (parameters *)param;
  int sum = 0;

  for (int k = 0; k < N; k++) {
    sum += mat1[p->i][k] * mat2[k][p->j];
  }

  result[p->i][p->j] = sum;
  pthread_exit(0);
}

int main() {
  FILE *fp1 = fopen("mat1.txt", "r");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      fscanf(fp1, "%d", &mat1[i][j]);
    }
  }
  fclose(fp1);

  FILE *fp2 = fopen("mat2.txt", "r");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      fscanf(fp2, "%d", &mat2[i][j]);
    }
  }
  fclose(fp2);

  pthread_t threads[N][N];
  parameters params[N][N];

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      params[i][j].i = i;
      params[i][j].j = j;
      pthread_create(&threads[i][j], NULL, multiply, &params[i][j]);
    }
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      pthread_join(threads[i][j], NULL);
    }
  }

  printf("Resultado da multiplicação:\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d ", result[i][j]);
    }
    printf("\n");
  }

  return 0;
}
