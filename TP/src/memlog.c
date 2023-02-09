#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "memlog.h"
#include "msgassert.h"


// Constantes definindo os estados de registro
static enum atividade {MLINATIVO, MLATIVO} atv;


// Tornam mais eficientes as funcoes leMemLog e escreveMemLog porque antes de chama-las, verificamos se esta fazendo o registro dos acessos a memoria
// Essa substituicao e feita no modulo chamador, no mat.c
// Evita que leMemLog seja chamada desnecessariamente
#define LEMEMLOG(pos, tam, id) \
    ((void) ((ml.ativo==MLATIVO)? leMemLog(pos,tam,id):0))

#define ESCREVEMEMLOG(pos, tam, id) \
    ((void) ((ml.ativo==MLATIVO)? escreveMemLog(pos,tam.id):0))



// Variavel estatica de registro de acessos
static memlog_tipo ml;





void clkDifMemLog(struct timespec t1, struct timespec t2, struct timespec * res){
// Descricao: calcula a diferenca entre t2 e t1, que e armazenada em res
// Entrada: t1, t2
// Saida: res


// A estrutura de dados timespec possui 2 campos: _sec e _nsec respectivamente segundos e nanosegundos
// Ajuste necessario, utilizando um segundo de tv_sec
// Se o tempo de t2 for menor que o tempo de t1, é necessario que 1 segundo se transforme em 1000000000 de
// nanosegundos e seja acrescentado a t2 para que o valor de tv_nsec nao seja negativo
  if (t2.tv_nsec < t1.tv_nsec){
    res-> tv_nsec = 1000000000+t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec-1;
  } else {
// Nao e necessario ajuste
    res-> tv_nsec = t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec;
  }
}




int iniciaMemLog(char * nome){
// Descricao: inicializa o registro de acessos, abrindo o arquivo nome
// Entrada: nome
// Saida: nao tem


// Escolhe modo do relogio
  ml.clk_id = CLOCK_MONOTONIC;


// Abre arquivo de registro e verifica se foi aberto corretamente
  ml.arquivo_registro = fopen(nome,"wt");
  erroAssert(ml.arquivo_registro != NULL,"Cannot open memlog output");


// Captura o tempo inicial do registro e armazena em tp
// A funcao clock_gettime retorna 0 em casos de sucesso e -1 em falhas
  struct timespec tp;
  int result = clock_gettime(ml.clk_id,&tp);

// Inicializa os valores do inittime do ml com o tempo inicial capturado em tp
  ml.inittime.tv_sec = tp.tv_sec;
  ml.inittime.tv_nsec = tp.tv_nsec;


// Inicializa variaveis do TAD
  ml.count = 1;
  ml.ativo = MLATIVO;
  ml.fase = 0;


// Imprime registro inicial no arquivo log marcando o tempo de inicio em segundos e nanosegundos 
  /*int retprint = fprintf(ml.arquivo_registro,"I\tCount: %ld\tTime: %ld.%ld\n", 
                         ml.count,tp.tv_sec,tp.tv_nsec);*/
  int retprint = fprintf(ml.arquivo_registro,"I %ld %ld.%ld\n", 
                         ml.count,tp.tv_sec,tp.tv_nsec);

// Verificacao se o arquivo realmente teve o tempo inicial printado
// A funcao fprintf retorna o numero de bytes printados e um valor negativo quando ocorre algum erro
  erroAssert(retprint>=0,"Nao foi possivel escrever registro");


// Retorna a variavel de identificacao de sucesso ou falha de captura do tempo (0 e -1, respectivamente)
  return result;
}





int ativaMemLog(){
// Descricao: ativa o registro de acessos
// Entrada: nao tem
// Saida: MLATIVO

// ml.ativo passa a ter valor 1
  ml.ativo = MLATIVO;
  return MLATIVO;
}





int desativaMemLog(){
// Descricao: desativa o registro de acessos
// Entrada: nao tem
// Saida: MLINATIVO

// ml.ativo passa a ter valor 0
  ml.ativo = MLINATIVO;
  return MLINATIVO;
}





int defineFaseMemLog(int f){
// Descricao: define a fase de registro de acessos 
// Entrada: f
// Saida: valor de f

// ml.fase passa a ter valor f escolhido
  ml.fase = f;
  return f;
}






int leMemLog(long int pos, long int tam){
// Descricao: registra acesso de leitura de tam bytes na posicao pos 
// Entrada: pos,tam
// Saida: resultado da obtencao do relogio


// Verifica se registro esta ativo
  if (ml.ativo == MLINATIVO) return 0;


// Captura tempo atual e armazena em tp
// Result tera valor 1 para caso de sucesso e 0 para falhas
  struct timespec tp, tdif;
  int result = clock_gettime(ml.clk_id,&tp);


// Calcula a diferencao entre tempo atual e o tempo inicial e armazena em tdif
  clkDifMemLog(ml.inittime,tp,&tdif);


// Atualiza contador
  ml.count++;


// Imprime registro no arquivo log com a fase, contador, diferenca do tempo atual para o inicial em seg e nanoseg, pos, tam
  int retprint = fprintf(ml.arquivo_registro,"L\tFase: %d\tCount: %ld\tTimeDif: %ld.%ld\tPos: %ld\tTam: %ld\n",
                         ml.fase, ml.count, tdif.tv_sec, tdif.tv_nsec, pos, tam); 
                   

// Verificacao se o arquivo realmente teve o registro printado
// A funcao fprintf retorna o numero de bytes printados e um valor negativo quando ocorre algum erro 
  erroAssert(retprint>=0,"Nao foi possivel escrever registro");


// Retorna a variavel de identificacao de sucesso ou falha de captura do tempo (0 e -1, respectivamente)
  return result;
}






int escreveMemLog(long int pos, long int tam){
// Descricao: registra acesso de escrita de tam bytes na posicao pos
// Entrada: pos, tam
// Saida: resultado da obtencao do relogio


// Verifica se registro esta ativo
  if (ml.ativo == MLINATIVO) return 0;


// Captura tempo atual e armazena em tp
  struct timespec tp,tdif;
  int result = clock_gettime(ml.clk_id,&tp);


// Calcula a diferenca do tempo atual e o inicial e armazena em tdif, para economia de armazenamento
  clkDifMemLog(ml.inittime,tp,&tdif);


// Atualiza contador
  ml.count++;


// Imprime registro no arquivo log com a fase, contador, diferenca do tempo atual para o inicial em seg e nanoseg, pos, tam
  int retprint = fprintf(ml.arquivo_registro,"E\tFase: %d\tCount: %ld\tTimeDif: %ld.%ld\tPos: %ld\tTam: %ld\n",
                         ml.fase, ml.count, tdif.tv_sec, tdif.tv_nsec, pos, tam);


// Verificacao se o arquivo realmente teve o registro printado
// A funcao fprintf retorna o numero de bytes printados e um valor negativo quando ocorre algum erro 
  erroAssert(retprint>=0,"Nao foi possivel escrever registro");


// Retorna a variavel de identificacao de sucesso ou falha de captura do tempo (0 e -1, respectivamente)  
  return result;
}





int finalizaMemLog(){
// Descricao: finaliza o registro de acessos a memoria
// Entrada: nao tem
// Saida: resultado da obtencao do relogio


// Captura o tempo atual e armazena em tp
  struct timespec tp, tdif;
  int result = clock_gettime(ml.clk_id,&tp);

// Calcula a diferenca com tempo inicial e armazena em tdif, para economia de armazenamento
  clkDifMemLog(ml.inittime, tp, &tdif);

// Atualiza contador
  ml.count++;


// Imprime registro final no arquivo log com contador, tempo final e diferenca do tempo final e inicial, em seg e nanoseg
  int retprint = fprintf(ml.arquivo_registro,"F\tCount: %ld\tTime: %ld.%.9ld\tTimeDif: %ld.%9ld\n",
                         ml.count, tp.tv_sec, tp.tv_nsec, tdif.tv_sec, tdif.tv_nsec);
             

// Verificacao se o arquivo realmente teve o registro printado
// A funcao fprintf retorna o numero de bytes printados e um valor negativo quando ocorre algum erro 
  erroAssert(retprint>=0,"Nao foi possivel escrever registro");


// Fecha arquivo de registro
  int retclose = fclose(ml.arquivo_registro);

// Verificacao se o arquivo foi fechado
// A funcao fclose retorna 0 quando arquivo foi fechado com sucesso e EOF quando ha falhas
  erroAssert(retclose == 0,"Nao foi possivel fechar o arquivo de registro");


// Atualiza variavel de estado para 0 (inativo)
  ml.ativo = MLINATIVO;

// Retorna a variavel de identificacao de sucesso ou falha de captura do tempo (0 e -1, respectivamente) 
  return result;
}

