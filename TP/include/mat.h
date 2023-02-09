
typedef struct matriz{

// Valores inteiros com as dimensoes x e y da matriz
    int dimensao_x, dimensao_y;

// Ponteiro duplo que representara a matriz
    double** mat;

} tipo_matriz; 


// Prototipo de funcoes uteis para fazer operacoes com matrizes
    void criaMatriz(tipo_matriz *mat, int dx, int dy);
    void CriaPreencheMatriz(FILE* arquivo_de_leitura, tipo_matriz *matriz);
    void inicializaMatrizNula(tipo_matriz *matriz, int flag);
    void inicializaMatrizAleatoria(tipo_matriz *matriz, int flag);
    double acessaMatriz(tipo_matriz *matriz, int flag);
    void imprimeMatriz(FILE* arquivo, tipo_matriz *matriz, int flag);
    void escreveElemento(tipo_matriz *matriz, int x, int y, double v);
    double leElemento (tipo_matriz* mat, int x, int y);
    void copiaMatriz(tipo_matriz *matriz, tipo_matriz *copia_matriz);
    void somaMatrizes(tipo_matriz *A, tipo_matriz *B, tipo_matriz *C, int flag);
    void multiplicaMatrizes(tipo_matriz *A, tipo_matriz *B, tipo_matriz *C, int flag);
    void transpoeMatriz(tipo_matriz *A, int flag);
    void destroiMatriz(tipo_matriz *A); 