#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define NUM_FILES 10
using namespace std;

int max_value = 0;
sem_t mutex;

typedef struct
{
  int thread_id;
  string filename;
} thdata;

thdata create_thdata(int thread_id){
  thdata data;
  data.thread_id = thread_id;
  data.filename = "vetor_" + to_string(thread_id) + ".txt";
  return data;
}


void *find_max_value(void *threadarg)
{
  thdata *data;
  data = (thdata *) threadarg;
  fstream file_io;
  file_io.open(data->filename);
  if (!file_io.is_open())
  {
    printf("Erro ao abrir o arquivo %s\n", data->filename);
    pthread_exit(NULL);
  }

  int value;
  while (file_io >> value)
  {
    sem_wait(&mutex);
    max_value = max(max_value, value);
    sem_post(&mutex);
  }

  file_io.close();

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  int qtt_files = NUM_FILES;
  pthread_t threads[NUM_FILES];
  thdata thread_data_array[NUM_FILES];
  sem_init(&mutex, 0, 1);

  for (int i = 0; i < qtt_files; i++)
  {
    thread_data_array[i] = create_thdata(i);
    pthread_create(&threads[i], NULL, find_max_value, (void *)&thread_data_array[i]);
  }

  for (int i = 0; i < qtt_files; i++)
  {
    cout << "EM AGUARDO: THREAD " << i << "\n";
    pthread_join(threads[i], NULL);
    cout << "FINALIZEI: THREAD " << i << "\n";
  }

  sem_destroy(&mutex);
  printf("O maior valor encontrado foi: %d\n", max_value);

  return 0;
}
