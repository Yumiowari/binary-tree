typedef struct no no;
typedef struct avl avl;

// Função que cria e retorna uma estrutura do tipo árvore balanceada utilizando o algoritmo AVL
avl *criaArvore();

// Função que insere um elemento na árvore na árvore AVL
// Retorna 1 se a inserção foi realizada com sucesso
// Retorna 0 se não foi possível realizar a inserção
int insereNo(avl *arv, int valor);

// Função que remove um elemento da árvore implementando a remoção por cópia usando o sucessor
// Retorna 1 se o elemento foi removido
// Retorna 0 se a árvore estiver vazia
// Retorna -1 caso o elemento a ser removido não esteja na árvore
int removeNo(avl *arv, int valor);

// Função que imprime a árvore em ordem
void imprimeOrdem(no *raiz);

// Função que imprime a árvore em pré-ordem
void imprimePreOrdem(no *raiz);

// Função que retorna a raiz da árvore
no *getRaiz(avl *arv);

// Função que retorna a quantidade de elementos da árvore
int getNumElementos(avl *arv);

// Função que lê um arquivo com números inteiros e carrega na árvore
// Se o tipo for 1, a função deve chamar insereNo()
// Se o tipo for 2, a função deve chamar removeNo()
// Retorna -2 se o arquivo não pôde ser lido
// Se o tipo for 1, insere os elementos na árvore e retorna os valores obtidos com o processamento da inserção
//  Retorna 1 se a inserção foi realizada com sucesso
//  Retorna 0 se não foi possível realizar a inserção
// Se o tipo for 2, remove os elementos da árvore e retorna os valores obtidos com o processamento da remoção
//  Retorna 1 se o elemento foi removido
//  Retorna 0 se a árvore estiver vazia
// Os valores retornados são tratados na main
int processaCarga(avl *arv, char *nomeArquivo, int tipo);

/*********** FUNÇÕES DA AVL **************/

// Função que verifica desbalanceamento na inserção
void atualizaFB_insercao(avl *arv, no *novoNo);

// Função que verifica desbalanceamento na remoção
void atualizaFB_remocao(avl *arv, no *pai, int chaveRemovida);

// Função que faz o balanceamento após a inserção
void balanceamento(avl *arv, no *noDesbal);

// Função que implementa a rotação à direita
void rotacaoDir(no *noDesbal);

// Função que implementa a rotação à esquerda
void rotacaoEsq(no *noDesbal);