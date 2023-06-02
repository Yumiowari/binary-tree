/*
    Implementação de Árvore Binária Rubro Negra
    Por: Rafael Renó Corrêa
    Última atualização: 2 de junho de 2023
*/

#include <stdlib.h>
#include <stdio.h>

#include "rubroNegra.h"

struct no{
    int chave;
    struct no *esq; // filho da esquerda (< raiz)
    struct no *dir; // filho da direita (> raiz)
    struct no *pai; // raiz do nó (> || < raiz)
    char cor; // 'V' ou 'P' (vermelha ou preta, respectivamente)
};

struct rb{
    struct no *sentinela; // nó para guardar o endereço em memória da árvore. Chave sempre -1000
    int qtd; // guarda a quantidade total de nós da árvore
};

rb *criaArvore(){
    // iniciando o ponteiro para a árvore
    rb *arv = (rb*) malloc(sizeof(rb));
    if(arv != NULL){
    	// iniciando a sentinela (ponteiro do tipo nó)
	no *sen = (no*) malloc(sizeof(no));
	if(sen != NULL){
        	arv->sentinela = sen;
        	arv->qtd = 0; // a sentinela não é um elemento da árvore.
        	sen->esq = NULL;
        	sen->dir = NULL;
        	sen->pai = NULL;
        	sen->chave = -1000;
            sen->cor = 'P'; // a sentinela é sempre preta
        }else return NULL;
    }else return NULL;

    return arv;
}

int insereNo(rb *arv, int valor){
    if(arv == NULL)return 0;
    no *novoNo = (no*) malloc(sizeof(no));
    // nó a ser inserido
    if(novoNo == NULL)return 0;

    novoNo->dir = NULL;
    novoNo->esq = NULL;
    novoNo->pai = NULL;
    novoNo->chave = valor;
    novoNo->cor = 'V';
    
    if(arv->sentinela->dir != NULL){
    	no *atual = arv->sentinela->dir; // dentro do condicional é mais inteligente!
    	no *anterior = atual->pai;
    	// auxiliares para percorrer a árvore
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
            anterior->esq = novoNo;
            novoNo->pai = anterior;
            // se for maior que a raiz
        }else{
            anterior->dir = novoNo;
            novoNo->pai = anterior;
            // se for menor
        }
    }else{ // inserir na árvore vazia acontece em apenas um caso!
    	// se nenhum elemento foi inserido
        arv->sentinela->dir = novoNo;
        // o nó será o pai
        novoNo->pai = arv->sentinela;
        // o pai do nó será a sentinela
        novoNo->cor = 'P'; // a raiz é sempre preta
        arv->qtd++;
        
        return 1;
        // se houver apenas um elemento, a função atualizaFB_insercao() é impossível!
    }
	
    // ocorreu vermelho filho de vermelho
    if(novoNo->pai->cor == 'V')balanceamentoInsercao(arv, novoNo);

    arv->qtd++;
    // incrementa em um a quantidade de elementos!

    return 1;
}

int removeNo(rb *arv, int valor){
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
        if(valor < atual->chave){
            atual = atual->esq;
        }else atual = atual->dir;
    }

    if(atual == NULL)return -1; // se terminou numa folha

    if(atual->esq == NULL && atual->dir == NULL){
    // se o nó a ser removido não tiver filhos
        if(anterior == NULL){
            arv->sentinela = NULL;
        }else if(atual == anterior->esq){
            anterior->esq = NULL;
        }else anterior->dir = NULL;

        free(atual);
        atual = NULL;

    }else if(atual->esq == NULL || atual->dir == NULL){ // deve ocorrer depois da condição acima!
    // se houver apenas um filho
        no *filho;
        if(atual->esq == NULL){
            filho = atual->dir;
            filho->pai = anterior;
        }else{
            filho = atual->esq;
            filho->pai = anterior;
        }
        if(anterior == NULL){
            arv->sentinela = filho;
        }else if(atual == anterior->esq){
            anterior->esq = filho;
        }else anterior->dir = filho;

        free(atual);
        atual = NULL;

    }else{
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
    }

    // balanceamento na remoção por fazer

    arv->qtd--;

    return 1;
}

void imprimeOrdem(no *raiz){
    if(raiz == NULL)return;
    if(raiz->pai == NULL){
        //imprimeOrdem(raiz->esq); nunca ocorrerá!
        imprimeOrdem(raiz->dir);
    }else if(raiz->pai != NULL){
        imprimeOrdem(raiz->esq);
        printf("%d: %c -- %d\n", raiz->chave, raiz->cor, raiz->pai->chave);
        imprimeOrdem(raiz->dir);
    }
}

void imprimePreOrdem(rb *arv, no *raiz){
    if(raiz == NULL)return;
    if(raiz->pai == NULL){
        //imprimePreOrdem(raiz->esq); nunca ocorrerá
        imprimePreOrdem(arv, raiz->dir);
    }else if(raiz->pai != NULL){ // senão for a sentinela
        printf("%d: %c -- %d\n", raiz->chave, raiz->cor, raiz->pai->chave);
        imprimePreOrdem(arv, raiz->esq);
        imprimePreOrdem(arv, raiz->dir);
    }
}

no *getRaiz(rb *arv){
    return arv->sentinela;
}

int getNumElementos(rb *arv){
    return arv->qtd;
}

int processaCarga(rb *arv, char *nomeArquivo, int tipo){
    FILE *arquivo = fopen(nomeArquivo, "r");
    int insere;

    if(arquivo == NULL)return -2;
    
    if(tipo == 1){
    // se for inserir
        if(arv == NULL)return 0;

        while(fscanf(arquivo, "%d", &insere) == 1)insereNo(arv, insere);

        return 1;
    }else if(tipo == 2){
    // se for remover
        if(arv->sentinela->dir == NULL)return 0;

        while(fscanf(arquivo, "%d", &insere) == 1)removeNo(arv, insere);
    
        return 1;
    }

    fclose(arquivo);
}

void rotacaoDir(rb *arv, no *noDesbal){
    no *aux = noDesbal->esq;
    noDesbal->esq = aux->dir;
    if(aux->dir != NULL)aux->dir->pai = noDesbal;
    aux->pai = noDesbal->pai;
    if(noDesbal == noDesbal->pai->dir){
        noDesbal->pai->dir = aux;
    }else noDesbal->pai->esq = aux;
    aux->dir = noDesbal;
    noDesbal->pai = aux;
}

void rotacaoEsq(rb *arv, no *noDesbal){
    no *aux = noDesbal->dir;
    noDesbal->dir = aux->esq;
    if(aux->esq != NULL)aux->esq->pai = noDesbal;
    aux->pai = noDesbal->pai;
    if(noDesbal == noDesbal->pai->esq){
        noDesbal->pai->esq = aux;
    }else noDesbal->pai->dir = aux;
    aux->esq = noDesbal;
    noDesbal->pai = aux;
}

void balanceamentoInsercao(rb *arv, no *noDesbal){
    no *tio;
    no *avo = noDesbal->pai->pai;
    char corTio;

    if(noDesbal->chave < avo->chave){
        tio = avo->dir;

        if(tio == NULL){
            corTio = 'P';
        }else corTio = tio->cor;

        if(corTio == 'V'){
            noDesbal->pai->cor = 'P';
            tio->cor = 'P';
            avo->cor = 'V';

            if(avo->pai->cor == 'V')balanceamentoInsercao(arv, avo);
        }else{
            if(noDesbal->pai->dir == noDesbal){
                noDesbal = noDesbal->pai;
                rotacaoEsq(arv, noDesbal);
            }

            noDesbal->pai->cor = 'P';
            avo->cor = 'V';
            rotacaoDir(arv, avo);
        }
        arv->sentinela->dir->cor = 'P';
    }else{
        tio = avo->esq;

        if(tio == NULL){
            corTio = 'P';
        }else corTio = tio->cor;

        if(corTio == 'V'){
            noDesbal->pai->cor = 'P';
            tio->cor = 'P';
            avo->cor = 'V';

            if(avo->pai->cor == 'V')balanceamentoInsercao(arv, avo);
        }else{
            if(noDesbal->pai->esq == noDesbal){
                noDesbal = noDesbal->pai;
                rotacaoDir(arv, noDesbal);
            }
            noDesbal->pai->cor = 'P';
            avo->cor = 'V';
            rotacaoEsq(arv, avo);
        }
        arv->sentinela->dir->cor = 'P';
    }
}


