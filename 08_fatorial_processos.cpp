#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

using namespace std;

// Função que calcula o fatorial de um número
int fatorial(int n)
{
  int result = 1;
  for (int i = n; i > 1; i--)
  {
    result *= i;
  }
  return result;
}

int main()
{
  // Pede ao usuário para digitar o número para o qual deseja calcular o fatorial
  // e o número de processos a serem criados
  int n, num_processes;
  cout << "Digite o número para o qual deseja calcular o fatorial: ";
  cin >> n;
  cout << "Digite o número de processos a serem criados: ";
  cin >> num_processes;

  // Calcula o intervalo de cálculo para cada processo
  int interval = n / num_processes;

  // Cria a área de memória compartilhada para armazenar o resultado final
  int shmid = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
  if (shmid < 0)
  {
    perror("Erro ao criar a área de memória compartilhada");
    return 1;
  }

  // Obtém um ponteiro para a área de memória compartilhada
  int *result = (int *)shmat(shmid, NULL, 0);
  if (result == (int *)-1)
  {
    perror("Erro ao vincular a área de memória compartilhada");
    return 1;
  }

  // Inicializa o resultado final como 1
  *result = 1;

  // Cria os processos filhos
  for (int i = 0; i < num_processes; i++)
  {
    pid_t pid = fork();
    if (pid < 0)
    {
      perror("Erro ao criar o processo filho");
      return 1;
    }
    else if (pid == 0)
    {
      // Processo filho: realiza o cálculo do fatorial
      int start = i * interval + 1;
      int end = (i + 1) * interval;
      if (i == num_processes - 1)
      {
        end = n; // último processo calcula até o final
      }
      int part_result = fatorial(end) / fatorial(start - 1);
      cout << "Processo " << i << ": fatorial(" << start << ".." << end << ") = " << part_result << endl;

      // Obtém o semáforo para sincronizar o acesso à área de memória compartilhada
      int semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
      if (semid < 0)
      {
        perror("Erro ao criar o semáforo");
        return 1;
      }
      semctl(semid, 0, SETVAL, 1);

      // Atualiza o resultado final na área de memória compartilhada
      struct sembuf sops;
      sops.sem_num = 0;
      sops.sem_op = -1;
      sops.sem_flg = 0;
      semop(semid, &sops, 1);
      *result *= part_result;
      sops.sem_op = 1;
      semop(semid, &sops, 1);

      // Finaliza o processo filho
      return 0;
    }
  }

  // Processo pai: espera todos os processos filhos terminarem e imprime o resultado final
  for (int i = 0; i < num_processes; i++)
  {
    wait(NULL);
  }
  cout << "Resultado final: " << *result << endl;

  // Desvincula a área de memória compartilhada e a remove
  shmdt(result);
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
