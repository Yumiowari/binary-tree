typedef struct no no;
typedef struct arvore arvore;

// Função que cria e retorna uma estrutura do tipo árvore
arvore *criaArvore();

// Função que insere um elemento na árvore
// Retorna 1 se a inserção foi realizada com sucesso
// Retorna 0 se não
int insereNo(arvore *arv, int valor);

// Função que remove um elemento da árvore implementando a remoção por cópia usando o sucessor
// Retorna 1 se o elemento foi removido
// Retorna 0 se a árvore estiver vazia
// Retorna -1 caso o elemento a ser removido não esteja na árvore
int removeNo(arvore *arv, int valor);

// Função que imprime a árvore em ordem
void imprimeOrdem(no *raiz);

// Função que imprime a árvore em pré-ordem
void imprimePreOrdem(no *raiz);

// Função que retorna a raiz da árvore
no *getRaiz(arvore *arv);

// Função que retorna a quantidade de elementos na árvore
int getNumElementos(arvore *arv);

// Função que lê um arquivo com números inteiros e carrega na árvore
void processaCarga(arvore *arv, char *nomeArquivo);