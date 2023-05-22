/*
    Implementação de Árvore Binária de Adelson Velsky e Landis
    Por: Rafael Renó Corrêa
    Última atualização: 19 de maio de 2023
*/

#include <stdlib.h>
#include <stdio.h>

#include "avl.h"

struct no{
    int chave;
    struct no *esq; // filho da esquerda (< raiz)
    struct no *dir; // filho da direita (> raiz)
    struct no *pai; // raiz do nó (> || < raiz)
    int fb; // fator de balanceamento
};

struct avl{
    struct no *sentinela; // nó para guardar o endereço em memória da árvore. Chave sempre -1000
    int qtd; // guarda a quantidade total de nós da árvore
};

avl *criaArvore(){
    // iniciando o ponteiro para a árvore
    avl *arv = (avl*) malloc(sizeof(avl));
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
        }else return NULL;
    }else return NULL;

    return arv;
}

int insereNo(avl *arv, int valor){
    if(arv == NULL)return 0;
    no *novoNo = (no*) malloc(sizeof(no));
    // nó a ser inserido
    if(novoNo == NULL)return 0;
    
    novoNo->dir = NULL;
    novoNo->esq = NULL;
    novoNo->pai = NULL;
    novoNo->chave = valor;
    novoNo->fb = 0; // é folha!

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
        arv->qtd++;
        
        return 1;
        // se houver apenas um elemento, a função atualizaFB_insercao() é impossível!
    }
	
    arv->qtd++;
    // incrementa em um a quantidade de elementos!
    
    atualizaFB_insercao(arv, novoNo);

    return 1;
}

int removeNo(avl *arv, int valor){
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

        atualizaFB_remocao(arv, atual->pai, valor);

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

        atualizaFB_remocao(arv, atual->pai, valor);

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

        atualizaFB_remocao(arv, anterior, sucessor->chave);

        free(sucessor);
        sucessor = NULL;
    }

    arv->qtd--;

    return 1;
}

void imprimeOrdem(no *raiz){
    if(raiz == NULL)return;
    if(raiz->pai == NULL){
        //imprimePreOrdem(raiz->esq); nunca ocorrerá!valor
        printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
        imprimeOrdem(raiz->dir);
    }
}

void imprimePreOrdem(no *raiz){
    if(raiz == NULL)return;
    if(raiz->pai == NULL){
        //imprimePreOrdem(raiz->esq); nunca ocorrerá
        imprimePreOrdem(raiz->dir);
    }else if(raiz->pai != NULL){ // senão for a sentinela
        printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
        imprimePreOrdem(raiz->esq);
        imprimePreOrdem(raiz->dir);
    }
}

no *getRaiz(avl *arv){
    return arv->sentinela;
}

int getNumElementos(avl *arv){
    return arv->qtd;
}

int processaCarga(avl *arv, char *nomeArquivo, int tipo){
    int ok; // feedback
    int flag = 0;
    int valor;
    FILE *arquivo = fopen(nomeArquivo, "r");
    if(arquivo == NULL)return -2;
    if(tipo == 1){ // chama insereNo()
        if(arv == NULL)return 0; // não havia árvore
        while(fscanf(arquivo, "%d", &valor) == 1)insereNo(arv, valor);
        // enquanto houverem elementos para serem lidos, os insere na árvore
        return 1; // sucedeu
    }else if(tipo == 2){
        while(fscanf(arquivo, "%d", &valor) == 1){
            ok = removeNo(arv, valor);
            if(ok == 0)return 0; // a árvore estava vazia
            if(ok == -1)flag++; // incrementa a flag para caso algum elemento não existia na árvore
        }
        // enquanto houverem elementos para serem lidos, os remove da árvore
        if(flag > 0)return 0;
        return 1; // sucedeu
    }
    fclose(arquivo);
}

void rotacaoDir(no *noDesbal){
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

void rotacaoEsq(no *noDesbal){
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

//verifica a estratégia de balanceamento do nó e ajusta o fator de balanceamento
void balanceamento(avl *arv, no *noDesbal){
    int fbNeto;
    int fbFilho;
    no *filho;
    if(noDesbal->fb == -2){
        filho = noDesbal->esq;
        if(filho->fb == 1){
            // rotação dupla para a direita
            fbNeto = filho->dir->fb;
            rotacaoEsq(filho);
            rotacaoDir(noDesbal);
            // ajusta o fb
            if(fbNeto == 0){
                noDesbal->fb = 0;
                filho->fb = 0;
            }else if(fbNeto == -1){
                noDesbal->fb = 1;
                filho->fb = 0;
                noDesbal->pai->fb = 0;
            }else{
                noDesbal->fb = 0;
                filho->fb = -1;
                noDesbal->pai->fb = 0;
            }
        }
        else{
            // rotação simples para a direita
            fbFilho = filho->fb;
            rotacaoDir(noDesbal);
            if(fbFilho != 0){
                // ajusta o fb
                filho->fb = 0;
                noDesbal->fb = 0;
            }else{
                filho->fb = 1;
                noDesbal->fb = -1;
            }
        }
    }
    if(noDesbal->fb == 2){
        filho = noDesbal->dir;
        if(filho->fb == -1){
            // rotação dupla para a esquerda
            fbNeto = filho->esq->fb;
            rotacaoDir(filho);
            rotacaoEsq(noDesbal);
            // ajusta o fb
            if(fbNeto == 0){
                noDesbal->fb = 0;
                filho->fb = 0;
            }else if(fbNeto == -1){
                noDesbal->fb = 0;
                filho->fb = 1;
                noDesbal->pai->fb = 0;
            }else{
                noDesbal->fb = -1;
                filho->fb = 0;
                noDesbal->pai->fb = 0;
            }
        }
        else{
            // rotação simples para a esquerda
            int fbFilho = filho->fb;
            rotacaoEsq(noDesbal);
            if(fbFilho != 0){
                // ajusta o fb
                filho->fb = 0;
                noDesbal->fb = 0;
            }else{
                filho->fb = 0;
                noDesbal->fb = 0;
            }
        }
    }
}

void atualizaFB_insercao(avl *arv, no *novoNo){
    no *aux = novoNo;
    do{
        if(aux->chave < aux->pai->chave){ // filho à esquerda
            aux->pai->fb--;
        }else{ // filho à direita
            aux->pai->fb++;
        }
        aux = aux->pai;
    }while((aux->pai != arv->sentinela) && (aux->fb != 0) && (aux->fb != 2) && (aux->fb != -2));
    if((aux->fb == 2) || (aux->fb == -2))balanceamento(arv, aux);
}

void atualizaFB_remocao(avl *arv, no *pai, int chave){
    no *aux = pai;
    do{
        if(chave < aux->chave){ // filho à esquerda
            aux->pai->fb++;
        }else{ // filho à direita
            aux->pai->fb--;
        }
        aux = aux->pai;
    }while((aux->pai != arv->sentinela) && (aux->fb != 0) && (aux->fb != 2) && (aux->fb != -2));
    if((aux->fb == 2) || (aux->fb == -2)){ // desbalanceou
        balanceamento(arv, aux);
        if((aux->pai != arv->sentinela) && (aux->pai->fb == 0)){
            atualizaFB_remocao(arv, aux->pai->pai, chave);
        }
    }
}


