#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include "mat.h"
#include "memlog.h" 
#include "msgassert.h"  


// Definicoes de operacoes a serem testadas
enum minhas_ops {UNDEF, OPSOMAR, OPMULTIPLICAR, OPTRANSPOR, FLAG};


// Variaveis globais utilizadas  
static enum minhas_ops opescolhida;
char* arquivo1;
char* arquivo2;
char* arquivo_res;
char* arquivo_registro;
int  regmem;
int flagP;


void uso(){
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
    
  fprintf(stderr,"main\n");
  fprintf(stderr,"\t-s \t (somar matrizes) \n");
  fprintf(stderr,"\t-m \t (multiplicar matrizes) \n");
  fprintf(stderr,"\t-t <arq> (transpor matriz)\n");
  fprintf(stderr,"\t-1 <arq> (arquivo com matriz 1) \n");
  fprintf(stderr,"\t-2 \t (arquivo com matriz 2) \n");
  fprintf(stderr,"\t-o \t (matriz para resultado) \n");
  fprintf(stderr,"\t-p <arq> (arquivo de registro de acesso)\n");
  fprintf(stderr,"\t-l \t (ativa registro de acessos a memoria)\n");
}



void parse_args(int argc,char ** argv){
// Descricao: le as opcoes da linha de comando e inicializa variaveis
// Entrada: argc e argv
// Saida: opescolhida, regmem, arquivo1, arquivo2, arquivo_res, arquivo_de_registro

// Variaveis externas do getopt
  extern char *optarg;
  extern int optind;

// Variavel auxiliar
  int c;

// Inicializacao variaveis globais para opcoes
  opescolhida = UNDEF;
  regmem = UNDEF;
  arquivo_registro = NULL;
  arquivo1 = NULL;
  arquivo2 = NULL;
  flagP = UNDEF;


// Leitura da linha de comando
  while ((c = getopt(argc, argv, "smtp:1:2:o:l")) != EOF)
    switch(c) {


    case 's':
      avisoAssert(opescolhida==UNDEF,"Mais de uma operacao escolhida");
      opescolhida = OPSOMAR;
      break;


    case 'm':
      avisoAssert(opescolhida==UNDEF,"Mais de uma operacao escolhida");
      opescolhida = OPMULTIPLICAR;
    break;


    case 't':
      avisoAssert(opescolhida==UNDEF,"Mais de uma operacao escolhida");
      opescolhida = OPTRANSPOR;
    break;


    case '1': 
      arquivo1 = optarg;
    break;


    case '2': 
      arquivo2 = optarg;
    break;


    case 'o': 
      arquivo_res = optarg;
    break;


    case 'p': 
      arquivo_registro = optarg;
      flagP = FLAG;
    break;


    case 'l': 
      regmem = 1;
    break;


    default:
      uso();
      exit(1);
    break;
  }

// Verificacao da consistencia das opcoes
    erroAssert(opescolhida>UNDEF,"Obrigatorio escolher uma operacao");

}









int main(int argc, char ** argv){
// Descricao: programa principal para execucao de operacoes de matrizes 
// Entrada: argc e argv
// Saida: dependente da operacao escolhida


// Ate 3 matrizes sao utilizadas, dependendo da operacao
  tipo_matriz A, B, C;


// Avaliar linha de comando
  parse_args(argc,argv);


// Iniciar registro de acesso
  if (flagP == FLAG){
    iniciaMemLog(arquivo_registro);
  }


// Ativar registro de acesso, se solicitado
  if (regmem){
    ativaMemLog();
  } else {
    desativaMemLog();
  }


// Ponteiros para arquivos de leitura
  FILE* arquivo_de_leitura1;
  FILE* arquivo_de_leitura2;
  FILE* arquivo_de_resultado;


// Abertura do arquivo1, o m1.txt, para leitura
  arquivo_de_leitura1 = fopen(arquivo1, "r+");
  erroAssert(arquivo_de_leitura1 != NULL, "Arquivo 1 nao pode ser aberto");


//Abertura do arquivo2, o m2.txt, para leitura, apenas se nao for escolhida a operacao de transposicao
  if(opescolhida != OPTRANSPOR){
    arquivo_de_leitura2 = fopen(arquivo2, "r+");
    erroAssert(arquivo_de_leitura2 != NULL, "Arquivo 2 nao pode ser aberto");
  }
  

//Abertura do arquivo_res, o m3.txt, para armazenar o resultado
  arquivo_de_resultado = fopen(arquivo_res, "w"); 
  erroAssert(arquivo_de_resultado != NULL, "Arquivo de resultado nao pode ser aberto");


// Execucao dependente da operacao escolhida
    switch (opescolhida){

        case OPSOMAR:
// Le matrizes A e B respectivamente dos arquivos -1 e -2 passados na linha de comando
// As matrizes sao somadas e seu resultado guardado em uma matriz C
// Imprime-se a matriz C no arquivo -o passado como parametro
// As matrizes sao desalocadas

            defineFaseMemLog(0);
            CriaPreencheMatriz(arquivo_de_leitura1, &A);
            CriaPreencheMatriz(arquivo_de_leitura2, &B);

            defineFaseMemLog(1);
            acessaMatriz(&A, flagP);
            acessaMatriz(&B, flagP);
            somaMatrizes(&A, &B, &C, flagP);

            defineFaseMemLog(2);
            imprimeMatriz(arquivo_de_resultado, &C, flagP);
            destroiMatriz(&A);
            destroiMatriz(&B);
            destroiMatriz(&C);

    break;



        case OPMULTIPLICAR:
// Le matrizes A e B nos arquivos respectivamente dos argumentos -1 e -2 passados na linha de comando
// As matrizes sao multiplicadas e seu resultado guardado em uma matriz C
// Imprime-se a matriz C no arquivo do argumento -o passado na linha de comando

          defineFaseMemLog(0);
          CriaPreencheMatriz(arquivo_de_leitura1, &A);
          CriaPreencheMatriz(arquivo_de_leitura2, &B);

          defineFaseMemLog(1);
          acessaMatriz(&A, flagP);
          acessaMatriz(&B, flagP);
          multiplicaMatrizes(&A, &B, &C, flagP);

          defineFaseMemLog(2);
          imprimeMatriz(arquivo_de_resultado, &C, flagP);
          destroiMatriz(&A);
          destroiMatriz(&B);
          destroiMatriz(&C);   

    break;




        case OPTRANSPOR:
// Le matriz A no arquivo respectivamento do argumento -1
// Transpoe a matriz
// Imprime-se a matriz transposta no arquivo do argumento-o passado na linha de comando
// OBS: acessar a matriz antes de transpor e recomendado para "aquecer o cash"

          defineFaseMemLog(0);
          CriaPreencheMatriz(arquivo_de_leitura1, &A);

          defineFaseMemLog(1);
          acessaMatriz(&A, flagP);
          transpoeMatriz(&A, flagP);

          defineFaseMemLog(2);
          imprimeMatriz(arquivo_de_resultado, &A, flagP);
          destroiMatriz(&A);

    break;

    default:
// Nao deve ser executado, pois ha um erroAssert em parse_args
    uso();
	  exit(1);
  }


// Conclui registro de acesso fechando arquivos
    if (flagP == FLAG){
        finalizaMemLog();
    }



// Fechamento dos arquivos abertos
  fclose(arquivo_de_leitura1);
  if(opescolhida != OPTRANSPOR){
    fclose(arquivo_de_leitura2);
  }
  fclose(arquivo_de_resultado);


  return 0;
}