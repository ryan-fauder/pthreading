#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_FILES 10
using namespace std;

typedef struct
{
  int thread_id;
  string filename;
  int max_value;
} thdata;

thdata create_thdata(int thread_id){
  thdata data;
  data.thread_id = thread_id;
  data.filename = "vetor_" + to_string(thread_id) + ".txt";
  data.max_value = 0;
  return data;
}


void *find_max_value(void *threadarg)
{
  thdata *data;
  data = (thdata *) threadarg;

  int max_value = 0;
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
    max_value = max(max_value, value);
  }

  file_io.close();

  data->max_value = max_value;
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  int qtt_files = NUM_FILES;
  pthread_t threads[NUM_FILES];
  thdata thread_data_array[NUM_FILES];
  
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

  int max_value = 0;
  for (int i = 0; i < qtt_files; i++)
  {
    if (thread_data_array[i].max_value > max_value)
    {
      max_value = thread_data_array[i].max_value;
    }
  }

  printf("O maior valor encontrado foi: %d\n", max_value);

  return 0;
}
