#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 3
sem_t mutex;
sem_t to_use;
sem_t used;
int total = 0;
int p = 0; // produced to buffer
int c = 0; // consumed from ubffer
int value = 10;
void *producer(void *arg)
{
  // wait
  sem_wait(&to_use);
  sem_wait(&mutex);

  printf("\nEntered..\n");

  p++;
  printf("Producer: %d", total);

  printf("\nJust Exiting...\n");
  sem_post(&mutex);
  sem_post(&used);
  return NULL;
}

void *consumer(void *arg)
{
  // wait

  sem_wait(&used);
  sem_wait(&mutex);
  printf("\nEntered..\n");

  c++;
  total++;
  // critical section
  sleep(5);

  printf("Consumer: %d", total);
  // signal
  printf("\nJust Exiting...\n");
  sem_post(&mutex);
  sem_post(&to_use);
  return NULL;
}

void *scanf1(void *arg)
{
  printf("Escreva um numero: ");
  scanf("%d", &value);
  return NULL;
}

int main()
{
  sem_init(&mutex, 0, 1);
  sem_init(&to_use, 0, N);
  sem_init(&used, 0, 0);
  pthread_t t1, t2, t3;
  pthread_create(&t1, NULL, scanf1, NULL);

  while (true)
  {

    pthread_create(&t2, NULL, producer, NULL);
    pthread_create(&t3, NULL, consumer, NULL);
    sem_wait(&mutex);
    if (total >= 6)
      break;
    printf("%d\n", total);
    sem_post(&mutex);
    sleep(2);
  }

  pthread_join(t1, NULL);
  printf("\n%d\n", value);
  sem_destroy(&mutex);
  sem_destroy(&to_use);
  sem_destroy(&used);
  return 0;
}