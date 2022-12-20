#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fstream>
#include <queue>
#include <string>


#define N 3


using namespace std;

sem_t has_work; // 0 -> No work to do;
sem_t buffer; // 0 -> No position in buffer;
sem_t mutex;
queue<string> q;
queue<string> for_work;
fstream file;

void* add_work(void* arg)
{
  string input;
  printf("Insira um arquivo: ");
  while(true){

    cin >> input;
    cin.clear();
    if(input == "0") break;
    sem_wait(&mutex);
    q.push(input);
    printf("Insira um arquivo: ");
    sem_post(&mutex);
    sem_post(&buffer);


  }
  return NULL;
}

void* get_work(void *arg)
{
  while(true){
    sem_wait(&buffer);
    sem_wait(&mutex);
    string filename = q.front();
    q.pop();
    for_work.push(filename);
    cout << "Lets open " << filename << "\n";

    sem_post(&mutex);
    sem_post(&has_work);

  }
  return NULL;
}

void readfile(){
  string text;
  string filename = for_work.front();
  file.open(filename);
  if (file.is_open()) {
    cout << filename << " ";
    cout << "CONTENT -- ";
    file >> text;
    cout << text;
    cout << " --\n";
  }
  for_work.pop();
  file.close();
}

void* work(void* arg){
  int * i = (int *) arg;
  while(true){
    sem_wait(&has_work);
    sem_wait(&mutex);
    cout << "I'm worker " << *i << "\n";
    readfile();
    sem_post(&mutex);
  }
  return NULL;
}

int main()
{ 
    pthread_t dispatcher, reader;
    vector<pthread_t> workers;
    workers.resize(3);

    // File Queue
    q.push("file1.txt");
    q.push("file2.txt");
    q.push("file2.txt");
    q.push("file3.txt");

    // Mutex
    sem_init(&mutex, 0, 1);
    
    sem_init(&buffer, 0, 4);
    sem_init(&has_work, 0, 0);
    vector<int> index = {1, 2, 3};

    pthread_create(&dispatcher,NULL, get_work,NULL);
    for (int i = 0; i < N; i++)
    {
      pthread_create(&workers[i], NULL, work, (void*) &index[i]);
    }
    sleep(2);
    pthread_create(&reader, NULL, add_work, NULL);
    pthread_join(reader, NULL);
    sleep(2);
    sem_destroy(&mutex);
    sem_destroy(&buffer);
    sem_destroy(&has_work);
    return 0;
}