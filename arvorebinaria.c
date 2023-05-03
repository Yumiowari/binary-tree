#include <stdlib.h>
#include <stdio.h>

#include "arvorebinaria.h"

/*
    Implementação de árvore binária!
    Rafael Renó Corrêa
    3 de maio de 2023
    2022000403
*/

struct no{
    int chave;
    struct no *esq; // filho da esquerda (< raiz)
    struct no *dir; // filho da direita (> raiz)
    struct no *pai; // raiz do nó (> || < raiz)
};

struct arvore{
    struct no *sentinela; // nó para guardar o endereço em memória da árvore. Chave sempre -1000
    int qtd; // guarda a quantidade total de nós da árvore
};

arvore *criaArvore(){ // OK
    // iniciando o ponteiro para a árvore
    arvore *arv = (arvore*) malloc(sizeof(arvore));
    if(arv != NULL){
    // iniciando a sentinela (ponteiro do tipo nó)
        no *sen = (no*) malloc(sizeof(no));
        sen->esq = NULL;
        sen->dir = NULL;
        sen->pai = NULL;
        sen->chave = -1000;

        arv->sentinela = sen;
        arv->qtd = 0; // a sentinela não é um elemento da árvore.
    }

    return arv;
}

int insereNo(arvore *arv, int valor){ // ERRO AQUI!
    if(arv == NULL)return 0;
    no *n = (no*) malloc(sizeof(no));
    // nó a ser inserido
    
    n->dir = NULL;
    n->esq = NULL;
    n->pai = NULL;
    n->chave=valor;

    no *atual = arv->sentinela;
    no *anterior = NULL;
    // auxiliares para percorrer a árvore

    //if(arv->sentinela->dir == NULL){
    if(arv->qtd == 0){ // mais rápido!
    // se nenhum elemento foi inserido
        arv->sentinela->dir = n;
        // o nó será o pai
        n->pai = arv->sentinela;
        // o pai do nó será a sentinela
        arv->qtd++;
        // incrementa em um a quantidade de elementos!

    }else{
        while(atual != NULL){
        // até que alcance uma folha da árvore
            anterior = atual;
            if(valor < atual->chave){
            // se for inserir um valor maior, vai para direita
                atual = atual->esq;
            }else atual = atual->dir; // se for menor, vai para a esquerda
        }

        if(valor < anterior->chave){
        // nesta instrução, o anterior é o pai da folha encontrada
            anterior->esq = n;
            n->pai = anterior;
            // se for maior que a raiz
        }else{
            anterior->dir = n;
            n->pai = anterior;
            // se for menor
        }

        arv->qtd++;
        // incrementa o nó inserido

    }

    return 1;
}

int removeNo(arvore *arv, int valor){
// em poucas palavras, no caso em que houver filhos, utiliza nós auxiliares para reunir os
// nós soltos para manter a estrutura da árvore após a remoção
    if(arv == NULL)return 0;

    no *atual = arv->sentinela;
    no *anterior = NULL;
    // inicia dois auxiliares para percorrer a árvore

    if(atual == NULL)return 0;

    while(atual != NULL && atual->chave != valor){
    // até que alcance uma folha ou o chave buscada, percorre a árvore em direção à posição.
    // Isto é viável pois, uma vez que os valores foram inseridos ordenadamente na árvore,
    // a busca binária é possível.
        anterior = atual;
        if(valor > atual->chave){
            atual = atual->dir;
        }else atual = atual->esq;
    }

    if(atual == NULL)return -1; // se terminou numa folha

    if(atual->dir == NULL && atual->esq == NULL){
    // se o nó a ser removido não tiver filhos
        if(anterior == NULL){
            arv->sentinela = NULL;
        }else if(atual == anterior->dir){
            anterior->dir = NULL;
        }else anterior->esq = NULL;

        free(atual);
        atual = NULL;

        arv->qtd--;
        // decrementa o nó removido

    }else if(atual->dir == NULL || atual->esq == NULL){ // deve ocorrer depois da condição acima!
    // se houver apenas um filho
        no *filho;
        if(atual->dir == NULL){
            filho = atual->esq;
            filho->pai = anterior;
        }else{
            filho = atual->dir;
            filho->pai = anterior;
        }
        if(anterior == NULL){
            arv->sentinela = filho;
        }else if(atual == anterior->dir){
            anterior->dir = filho;
        }else anterior->esq = filho;

        free(atual);
        atual = NULL;

        arv->qtd--;

    }else if(atual->dir != NULL && atual->esq != NULL){
    // se houver dois filhos
        no *sucessor = atual->dir;
        anterior = atual;
        while(sucessor->esq != NULL){
            anterior = sucessor;
            sucessor = sucessor->esq;
        } 
        atual->chave = sucessor->chave;
        if(anterior->esq == sucessor){
            anterior->esq = sucessor->dir;
            if(sucessor->dir != NULL)sucessor->dir->pai = atual;
        }else{
            anterior->dir = sucessor->dir;
            if(sucessor->dir != NULL)sucessor->dir->pai = atual;
        }

        free(sucessor);
        sucessor = NULL;

        arv->qtd--;

    }

    return 1;
}

void imprimeOrdem(no *raiz){
    if(raiz == NULL)return;
    if(raiz->pai == NULL){
        imprimePreOrdem(raiz->esq);
        imprimePreOrdem(raiz->dir);
    }else{
        imprimeOrdem(raiz->esq);
        printf("%d - %d \n", raiz->chave, raiz->pai->chave);
        imprimeOrdem(raiz->dir);
    }
}

void imprimePreOrdem(no *raiz){
    if(raiz == NULL)return;
    if(raiz->pai == NULL){
        imprimePreOrdem(raiz->esq);
        imprimePreOrdem(raiz->dir);
    }else{
        printf("%d - %d \n", raiz->chave, raiz->pai->chave);
        imprimePreOrdem(raiz->esq);
        imprimePreOrdem(raiz->dir);
    }
}

no *getRaiz(arvore *arv){
    return arv->sentinela;
}

int getNumElementos(arvore *arv){
    return arv->qtd;
}

void processaCarga(arvore *arv, char *nomeArquivo){
    int valor;
    FILE *arquivo = fopen(nomeArquivo, "r");
    while(fscanf(arquivo, "%d", &valor) == 1)insereNo(arv, valor);
    // enquanto houverem elementos para serem lidos, os insere na árvore
    fclose(arquivo);
}

