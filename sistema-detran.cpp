#include <stdio.h>

#include <string.h>

#include <locale.h>

#include <stdlib.h>

#include <iostream>

struct reg {
  char nome[50], motivo[50], valor[15], status;

};
int tamanho(FILE * );
void cadastrar(FILE * );
void consultar(FILE * );
void gerararqtxt(FILE * );
void excluir(FILE * );

int main() {
  setlocale(LC_ALL, "");
  int op;
  FILE *arq;

  if ((arq = fopen("dados.dat", "rb+")) == NULL) {
    if ((arq = fopen("dados.dat", "wb+")) == NULL) {
      printf("Falha ao abrir o arquivo.\n");
    }
  }

  do {
    system("cls");
    printf("--------------| SISTEMA DETRAN - MG |--------------\n");
    printf("\n1 - Cadastrar Infrator");
    printf("\n2 - Consultar Situação do Motorista");
    printf("\n3 - Gerar Lista de Infratores");
    printf("\n4 - Inativar Cadastro");
    printf("\n5 - Finalizar\n");
    printf("\n--------------| Infratores Totais: %d |-------------\n\n", tamanho(arq));

    printf("Digite uma opção acima: ");
    scanf("%d", & op);

    switch (op) {
    case 1:
      cadastrar(arq);
      break;
    case 2:
      consultar(arq);
      break;
    case 3:
      gerararqtxt(arq);
      break;
    case 4:
      excluir(arq);
      break;
    case 5:
      system("cls");
      printf("--------------| Sistema Encerrado |--------------");
      fclose(arq);
      break;
    }
  } while (op != 5);

}

void cadastrar(FILE * arq) {
  system("cls");
  reg detran;
  char confirma;

  fflush(stdin);

  printf("--------| Identificador de Cadastro: %d |--------\n\n", tamanho(arq) + 1);

  printf("Nome: ");
  gets(detran.nome);

  printf("Motivo: ");
  gets(detran.motivo);

  printf("Valor da Multa: ");
  gets(detran.valor);

  printf("\nCadastrar? (s/n) ");
  scanf("%c", & confirma);

  detran.status = ' ';

  if (toupper(confirma) == 'S') {
    printf("\n\ngravando....\n");
    fseek(arq, 0, SEEK_END);
    fwrite( & detran, sizeof(reg), 1, arq);
  }

}

void consultar(FILE * arq) {
  system("cls");
  reg detran;
  int nr;
  printf("--------------| Consulta pelo código |--------------.\n");
  printf("Informe o Codigo: ");
  scanf("%d", & nr);

  if ((nr <= tamanho(arq)) && (nr > 0)) {
    fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
    fread( & detran, sizeof(reg), 1, arq);
    if (detran.status == ' ') {
    	printf("\n");
		printf("Nome..... %s", detran.nome);
    	printf("\nMotivo... %s", detran.motivo);
    	printf("\nValor.... %s\n\n", detran.valor);
    }
  } else {
    printf("Código Informado Inesistente.\n");
  }
  system("pause");
}

void gerararqtxt(FILE * arq) {
  system("cls");
  char nomearq[20];
  printf("Nome do arquivo: ");
  scanf("%s", nomearq);
  strcat(nomearq, ".txt");

  FILE * arqtxt = fopen(nomearq, "w");

  if (!arqtxt) {
    printf("Não foi possivel crirar este arquivo.");
  }

  fprintf(arqtxt, "cadastros inativados são identificados com * na frente do valor\n\n\n\n");
  fprintf(arqtxt, "- - - - - - - - - - - - - - - - - - - - - - - - - | Detran-MG | - - - - - - - - - - - - - - - - - - - - - - - - -\n");
  int nr;
  reg detran;
  for (nr = 0; nr < tamanho(arq); nr++) {
    fseek(arq, nr * sizeof(reg), SEEK_SET);
    fread( & detran, sizeof(reg), 1, arq);
    fprintf(arqtxt, "\nNome: %-28s Motivo: %-32s Valor: R$ %s  %c\n", detran.nome, detran.motivo, detran.valor, detran.status);
    fprintf(arqtxt, "------------------------------------------------------------------------------------------\n");
  }
  fprintf(arqtxt, "\n- - - - - - - - - - - - - - - - - - - - - - - - - | Detran-MG | - - - - - - - - - - - - - - - - - - - - - - - - -\n");
  fclose(arqtxt);
}

void excluir(FILE * arq) {
  system("cls");
  reg detran;
  char confirma;
  int nr;

  printf("\nCodigo de Identificador a Inativar: ");
  scanf("%d", & nr);
  if ((nr <= tamanho(arq)) && (nr > 0)) {
    fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
    fread( & detran, sizeof(reg), 1, arq);
    if (detran.status == ' ') {
      printf("\nNome......:%s", detran.nome);
      printf("\nMotivo..:%s", detran.motivo);
      printf("\nValor....:%s\n", detran.valor);
      printf("\nConfirmar Inativação: <s/n>\n");
      getchar();
      scanf("%c", & confirma);

      if (toupper(confirma) == 'S') {
        printf("\nInativnado...\n\n");
        fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
        detran.status = '*';
        fwrite( & detran, sizeof(reg), 1, arq);
      }
    } else
      printf("Código Inexistente! \n");
  } else {
    printf("\nCódigo de Registro Invalido!\n");
  }
  system("pause");
}

int tamanho(FILE * arq) {
  fseek(arq, 0, SEEK_END);
  return ftell(arq) / sizeof(reg);
}
