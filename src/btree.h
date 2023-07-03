#define FILE_NAME "../data/table.txt" // nome do arquivo de dados

#define NUM_LINES 10000 // número de linhas no arquivo (após cabeçalho)

#define SIZE_DATA_LINE 41 // tamanho de cada linha

#define MAX_KEYS 8 // chaves máximas da árvore

#define MIN_KEYS 4 // chaves mínimas da árvore

typedef struct par{
    int valor;
    int linha;
}Par;

typedef struct no{
    int n;
    Par chaves[MAX_KEYS];
    struct no *filhos[MAX_KEYS + 1];
    int folha; // é folha? 1 - sim, 0 - não
}No;

typedef struct btree{
    No *raiz;
}BTree;


/* Entrada/Saída */
Par* processaDados();

void imprimirLinha(FILE *f, int linha);

int imprimirLinear(FILE *f, int id);

void procurarPokemon(BTree arv, int valor, FILE *arquivo);

/* Árvore */
No* criarNo();

void inserirBTree(BTree *arv, Par chave);

void imprimirBTree(BTree *arv);

//BTreeNode* searchBTree(BTree *tree, int value);

/* Nó */
void split(No *pai, int indice, No *filho);

Par merge(No *no, int indice);

void ajustaFilho(No *no, int indice);

void excluirChaveBtree(BTree *arv, int valor);

void excluirChaveNo(No *no, int valor);

void inserirNo(No *no, Par chave);

void imprimirNoBtree(No *no);

No* pesquisarNo(No* no, int valor);
