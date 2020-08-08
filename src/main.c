#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

int num_primo(int x){ /* Função que determina se número é primo */
  int i, num_dividores;

  num_dividores = 0;

  for (i=1;i<=x;i++){
    if (x % i == 0){
      num_dividores++; /* Caso o numero seja divisivel pelo índice i, incrementa número de divisores */
    }
  }

  if (num_dividores == 2){ /* Número tem 2 divisores (1 e ele mesmo) */
    return 1;
  }else{
    return 0;
  }
}

int main() {
  pid_t filho;
  int numero;
  char caracter;
  int num_processos = 0;
  int i,aux;

  aux = 0;

  /* Definir flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Criar area de memoria compartilhada */
  int *qtd_primos;
  qtd_primos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  (*qtd_primos)=0;

  while (aux == 0){
    if (num_processos < 4){ /* Se o numero de processor é menor que 4, realiza a leitura da entrada e cria um processo */
      scanf("%d%c", &numero, &caracter);
      filho = fork();
      if (filho == 0) {
        (*qtd_primos) += num_primo(numero); /* Na execução do processo filho verifica se número é primo */
        exit(0); /* Sai do processo após verificação */
      }
      num_processos++;

      if (caracter == '\n'){
        aux = 1;
        break;
      }
    }else {
      waitpid(-1, NULL, 0); /* Caso o número de processor aberto seja 4, aguarda até um dos processos terminar */
      num_processos--;
    }
  }

  for(i=0;i<4;i++){
    waitpid(-1, NULL, 0); /* Aguarda todos os processos encerrarem  */
  }

  printf("%d\n", *qtd_primos);

  return 0;
}
