#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_FILOSOFOS 10

pthread_mutex_t garfos[NUM_FILOSOFOS];

void pegar_garfos(int id) {
  pthread_mutex_lock(&garfos[id]);
  pthread_mutex_lock(&garfos[(id + 1) % NUM_FILOSOFOS]);
}

void colocar_garfos(int id) {
  pthread_mutex_unlock(&garfos[id]);
  pthread_mutex_unlock(&garfos[(id + 1) % NUM_FILOSOFOS]);
}

void *filosofo(void *id) {
  int *i = (int *) id;

  while (1) {
    printf("Filósofo %d está pensando.\n", *i);
    sleep(1);

    pegar_garfos(*i);

    printf("Filósofo %d está comendo.\n", *i);
    sleep(3);

    colocar_garfos(*i);
    printf("Filósofo %d deixou de comer.\n", *i);
    sleep(3);
  }

  return NULL;
}

int main() {
  pthread_t filosofos[NUM_FILOSOFOS];
  int i;
  int *id;

  for (i = 0; i < NUM_FILOSOFOS; i++) {
    pthread_mutex_init(&garfos[i], NULL);
  }

  for (i = 0; i < NUM_FILOSOFOS; i++) {
    id = malloc(sizeof(int));
    *id = i;
    pthread_create(&filosofos[i], NULL, filosofo, (void *) id);
  }

  for (i = 0; i < NUM_FILOSOFOS; i++) {
    pthread_join(filosofos[i], NULL);
  }

  return 0;
}
