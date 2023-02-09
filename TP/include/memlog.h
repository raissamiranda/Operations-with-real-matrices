#ifndef MEMLOGH
#define MEMLOGH

#include <stdio.h>
#include <time.h>

typedef struct memlog{

// O arquivo onde os registros vao ser colocados
	FILE * arquivo_registro;

// Estruturas relativas a contagem de tempo
	clockid_t clk_id;
	struct timespec inittime;

// Contador de quantos eventos foram gerados
	long count; 

// Define a fase
	int fase;

// Define se esta ativo ou nao (eficiencia)
	int ativo;

} memlog_tipo;


// Prototipo de funcoes uteis para o registro de acesso
int iniciaMemLog(char * nome);
int ativaMemLog();
int desativaMemLog();
int defineFaseMemLog(int f);
int leMemLog(long int pos, long int tam);
int escreveMemLog(long int pos, long int tam);
int finalizaMemLog();

#endif