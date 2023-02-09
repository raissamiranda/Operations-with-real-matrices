#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"mat.h"
#include"msgassert.h"
#include"memlog.h"


// Macro que realiza swap sem variavel auxiliar
#define ELEMSWAP(x,y) (x+=y,y=x-y,x-=y)

void criaMatriz(tipo_matriz *matriz, int dx, int dy){
// Descricao: cria matriz com dimensoes dx X dy
// Entrada: matriz, dx, dy
// Saida: matriz

// Verifica se as dimensoes da matriz sao validas
    erroAssert(dx>0,"Dimensao invalida/nula");
    erroAssert(dy>0,"Dimensao invalida/nula");

// Inicializa as dimensoes da matriz
    matriz->dimensao_x = dx;
    matriz->dimensao_y = dy;

// Aloca a matriz dinamicamente
    matriz->mat = (double**)malloc(dx*sizeof(double));

    for(int i=0; i<dx; i++){
        matriz->mat[i] = (double*)malloc(dy*sizeof(double));
    }

//Verifica se a alocacao dinamica falhou
    erroAssert(matriz->mat != NULL, "Falha na alocacao");

}



void CriaPreencheMatriz(FILE* arquivo_de_leitura, tipo_matriz *matriz){
// Descricao: cria e preenche a matriz com elementos lidos de um arquivo 
// Entrada: arquivo_de_leitura e matriz
// Saida: arquivo_de_leitura

// Verifica se o arquivo pode ser aberto
    erroAssert(arquivo_de_leitura != NULL,"Erro: arquivo nao pode ser aberto");

// Usa os dois primeiros elementos do arquivo para criar uma matriz com as devidas dimensoes
    int dx, dy;
    fscanf(arquivo_de_leitura, "%d %d", &dx, &dy);
    matriz->dimensao_x = dx;
    matriz->dimensao_y = dy;

// Verifica se as dimensoes da matriz sao validas
    erroAssert(dx>0,"Dimensao invalida/nula");
    erroAssert(dy>0,"Dimensao invalida/nula");

// Aloca a matriz dinamicamente
    matriz->mat = (double**)malloc(dx*sizeof(double));

    for(int i=0; i<dx; i++){
        matriz->mat[i] = (double*)malloc(dy*sizeof(double));
    }

//Verifica se a alocacao dinamica falhou
    erroAssert(matriz->mat != NULL, "Falha na alocacao");

// Preenche um vetor auxiliar com os elementos lidos no restante do arquivo
// Verifica se  o numero de elementos do arquivo corresponde ao predefinidos nas dimensoes
// (essa verificacao tambem exclui casos com elementos nao double que nao serao lidos)
    double *vetor_aux = (double*)malloc(dx*dy*sizeof(double));
    int contador=0;
 
    while(!feof(arquivo_de_leitura)){
        fscanf(arquivo_de_leitura, "%lf", &vetor_aux[contador]);
        contador++;
    }

// Preenche a matriz com os elementos do vetor auxiliar
    int k=0;
        for(int i = 0; i<dx; i++){
            for(int j=0; j<dy; j++){
                matriz->mat[i][j] = vetor_aux[k];
                k++;
            }
        }

// Desaloca o vetor utilizado
    free(vetor_aux);
}



void inicializaMatrizNula(tipo_matriz *matriz, int flag){
// Descricao: inicializa matriz com valores nulos 
// Entrada: matriz
// Saida: matriz

// Inicializa todos os elementos da matriz com o valor 0
    for(int i=0; i<matriz->dimensao_x; i++){
        for(int j=0; j<matriz->dimensao_y; j++){
            matriz->mat[i][j] = 0;
            if (flag == 4){
                escreveMemLog((long int)(&(matriz->mat[i][j])),sizeof(double));
            }
        }
    }
}



void inicializaMatrizAleatoria(tipo_matriz *matriz, int flag){
// Descricao: inicializa matriz com valores aleatorios
// Entrada: matriz 
// Saida: matriz

// Inicializa todos os elementos da matriz com valores aleatorios entre 0 e 100
    for(int i=0; i<matriz->dimensao_x; i++){
            for(int j=0; j<matriz->dimensao_y; j++){
                matriz->mat[i][j] = rand()%100;
                if (flag == 4){
                    escreveMemLog((long int)(&(matriz->mat[i][j])),sizeof(double));
                }
            }
    }
}



double acessaMatriz(tipo_matriz * matriz, int flag){
// Descricao: acessa matriz para fins de registro de acesso
// Entrada: matriz
// Saida: matriz

    double aux, s=0.0;

// Acessa todas as posicoes da matriz
    for(int i=0; i<matriz->dimensao_x; i++){
        for(int j=0; j<matriz->dimensao_y; j++){
             aux = matriz->mat[i][j];
             s += aux;
             if (flag == 4){
                leMemLog((long int)(&(matriz->mat[i][j])), sizeof(double));
             }
        }
    }
    return s;
}




void imprimeMatriz(FILE* arquivo, tipo_matriz *matriz, int flag){
// Descricao: imprime a matriz no arquivo passado como parametro
// Entrada: mat
// Saida: impressao na saida padrao (stdout)
    fprintf(arquivo, "%d %d\n", matriz->dimensao_x, matriz->dimensao_y);
    for(int i=0; i<matriz->dimensao_x; i++){
        for(int j=0; j<matriz->dimensao_y; j++){
             fprintf(arquivo, "%.5f ", matriz->mat[i][j]);
             if(flag == 4){
                leMemLog((long int)(&(matriz->mat[i][j])),sizeof(double));
             }
        }
    fprintf(arquivo, "\n");
    }
    fprintf(arquivo, "\n");
}



void escreveElemento(tipo_matriz *matriz, int x, int y, double v){
// Descricao: atribui o valor v ao elemento (x,y) de matriz
// Entrada: matriz, x, y, v
// Saida: matriz

// Verifica se o elemento procurado esta dentro das dimensoes da matriz
    erroAssert(x>0 && x<matriz->dimensao_x,"Indice invalido");
    erroAssert(y>0 && y<matriz->dimensao_y,"Indice invalido");

// Atribuicao do valor v ao elemento (x,y)
    matriz->mat[x][y] = v;
    escreveMemLog((long int)(&(matriz->mat[x][y])),sizeof(double));
}



double leElemento (tipo_matriz *matriz, int x, int y){
// Descricao: retorna o elemento (x,y) de matriz 
// Entrada: matriz, x, y
// Saida: matriz[x][y]

// Verifica se o elemento procurado esta dentro das dimensoes da matriz
    erroAssert(x>0 && x<matriz->dimensao_x,"Indice invalido");
    erroAssert(y>0 && y<matriz->dimensao_y,"Indice invalido");

    //leMemLog((long int)(&(matriz->mat[x][y])),sizeof(double));
// Retorna o elemento identificado
    return (matriz->mat[x][y]);
}




void copiaMatriz(tipo_matriz *matriz, tipo_matriz *copia_matriz){
// Descricao: faz uma copia de matriz em copia_matriz
// Entrada: matriz
// Saida: copia_matriz

// Cria a copia_matriz com as dimensoes de matriz
    criaMatriz(copia_matriz, matriz->dimensao_x, matriz->dimensao_y);

// Copia cada elemento da matriz
    for(int i=0; i<matriz->dimensao_x; i++){
        for(int j=0; j<matriz->dimensao_y; j++){
            copia_matriz->mat[i][j] = matriz->mat[i][j];
            leMemLog((long int)(&(matriz->mat[i][j])),sizeof(double));
            escreveMemLog((long int)(&(copia_matriz->mat[i][j])),sizeof(double));
        }
    }
} 




void somaMatrizes(tipo_matriz *A, tipo_matriz *B, tipo_matriz *C, int flag){
// Descricao: soma as matrizes A e B e armazena o resultado em C
// Entrada: A, B
// Saida: C

// Verifica se as dimensoes de a e b sao iguais
    erroAssert(A->dimensao_x==B->dimensao_x,"Dimensoes incompativeis");
    erroAssert(A->dimensao_y==B->dimensao_y,"Dimensoes incompativeis");

// Cria a matriz C com as mesmas dimensoes de A e B e inicializa seus elementos com 0 por segurança
    criaMatriz(C, A->dimensao_x, A->dimensao_y);
    inicializaMatrizNula(C, flag);

// Soma elemento a elementos de A e B e armazena em C
    for(int i=0; i<A->dimensao_x; i++){
        for(int j=0; j<B->dimensao_y; j++){
            C->mat[i][j] = A->mat[i][j] + B->mat[i][j];
            if(flag == 4){
                leMemLog((long int)(&(A->mat[i][j])),sizeof(double));
                leMemLog((long int)(&(B->mat[i][j])),sizeof(double));
                escreveMemLog((long int)(&(C->mat[i][j])),sizeof(double));
            }
        }
    }
}





void multiplicaMatrizes(tipo_matriz *A, tipo_matriz *B, tipo_matriz *C, int flag){
// Descricao: multiplica as matrizes A e B e armazena o resultado em C
// Entrada: A,B
// Saida: C

// Verifica se as dimensoes de a e b sao compativeis
    erroAssert(A->dimensao_y==B->dimensao_x,"Dimensoes incompativeis");

// Cria a matriz C com as mesmas dimensoes de A e B e inicializa seus elementos com 0 por segurança
    criaMatriz(C, A->dimensao_x, B->dimensao_y);
    inicializaMatrizNula(C, flag);

// Multiplica A e B e armazena o resultado em C
    for (int i=0; i<C->dimensao_x;i++){
        for (int j=0; j<C->dimensao_y;j++){
            for (int k=0; k<A->dimensao_y;k++){
                C->mat[i][j] += A->mat[i][k]*B->mat[k][j];
                if(flag == 4){
                    leMemLog((long int)(&(A->mat[i][k])),sizeof(double));
                    leMemLog((long int)(&(B->mat[k][j])),sizeof(double));
                    escreveMemLog((long int)(&(C->mat[i][j])),sizeof(double));
                }
            }
        }
    }
}



void transpoeMatriz(tipo_matriz *A, int flag){
// Descricao: transpoe a matriz A
// Entrada: A
// Saida: A

// Captura a maior dimensao entre as dimensoes x e y de A
  int maior_dimensao = (A->dimensao_x > A->dimensao_y? A->dimensao_x : A->dimensao_y);

// Faz a transposicao como se a matriz fosse quadrada
  for (int i=0; i<maior_dimensao; i++){
    for(int j=i+1; j<maior_dimensao; j++){
      ELEMSWAP((A->mat[i][j]),(A->mat[j][i]));
      if (flag == 4){
        escreveMemLog((long int)(&(A->mat[i][j])),sizeof(double));
        escreveMemLog((long int)(&(A->mat[j][i])),sizeof(double));
      }
    }
  }

// Inverte as dimensoes da matriz transposta
    ELEMSWAP(A->dimensao_x, A->dimensao_y);
}





void destroiMatriz(tipo_matriz *A){
// Descricao: libera a memoria alocada dinamicamente para a matriz A
// Entrada: A
// Saida: A

// Verifica se a matriz ja nao foi desalocada
    avisoAssert(A->mat != NULL,"Matriz já foi destruida");

// Desaloca a memoria
    for(int i=0; i<A->dimensao_x; i++){
        free(A->mat[i]);
    }
    free(A->mat);
} 





