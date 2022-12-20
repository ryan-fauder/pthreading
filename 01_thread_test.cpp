#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>

using namespace std;

int N = 1000;

void * add_(void * ptr);
void * sub_(void * ptr);
void * mul_(void * ptr);


int main()
{
  pthread_t thread1, thread2;
  string message1 = "Thread 1";
  string message2 = "Thread 2";
  int iret1, iret2;

  /* Create independent threads each of which will execute function */

  iret1 = pthread_create(&thread1, NULL, mul_, &message1[0]);
  iret2 = pthread_create(&thread2, NULL, sub_, &message2[0]);

  pthread_join(thread2, NULL);
  pthread_join(thread1, NULL);

  printf("%d \n", N);

  printf("Thread 1 returns: %d\n", iret1);
  printf("Thread 2 returns: %d\n", iret2);
  return 0;
}

void * add_(void * ptr)
{
  N += 100;
  printf("%s :", (char *) ptr);
  printf("%d \n", N);
  return NULL;
}

void * mul_(void * ptr)
{
  N *= 2;
  printf("%s: ", (char *) ptr);
  printf("%d \n", N);
  return NULL;
}

void * sub_(void * ptr)
{
  int value;
  scanf("%d", &value);
  printf("%s: ", (char *) ptr);
  N -= value;
  return NULL;
}