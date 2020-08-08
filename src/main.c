#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

int num_primo(int x){
  int i, resultado;

  resultado = 0;

  for (i=1;i<=x;i++){
    if (x % i == 0){
      resultado++;
    }
  }

  if (resultado == 2){
    return 1;
  }else{
    return 0;
  }
}

int main() {
  pid_t filho[4];
  int numero;
  char caracter;
  int num_processos = 0;
  int i,aux;

  /* Definir flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Criar area de memoria compartilhada */
  int *b;
  b = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  if ((long int)b==-1) printf("Erro de alocacao!\n");
  (*b)=0;

  while (aux == 0){
    while (num_processos < 4){
      scanf("%d%c", &numero, &caracter);
      printf("num=%d caracter=%c\n", numero, caracter);

      filho[num_processos] = fork();
      printf("eita:%d\n", num_processos);
      if (filho[num_processos] == 0) {
        /* Esta parte do codigo executa no processo filho */
        printf("num=%d num_processo=%d\n", numero, num_processos);
        (*b) += num_primo(numero);
        printf("qtd=%d\n", *b);
        exit(0);
      }
      num_processos++;

      if (caracter == '\n'){
        aux = 1;
        break;
      }
    }
    for(i=0;i<4;i++){
      waitpid(filho[num_processos], NULL, 0);
      num_processos = 0;
    }
  }

  for(i=0;i<4;i++){
    printf("ENCERA\n");
    waitpid(filho[num_processos], NULL, 0);
  }

  printf("%d\n", *b);

  return 0;
}
