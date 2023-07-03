#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

/* Entrada/Saída */
Par* processaDados(){ // ok
    FILE *arquivo = fopen(FILE_NAME, "r");
    if(arquivo == NULL)return NULL;

    Par *pares = (Par*) malloc(NUM_LINES * sizeof(Par));
    if(pares == NULL){
        fclose(arquivo);
        return NULL;
    }

    int i = 0; // contador para as linhas
    char linha[100];

    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        sscanf(linha, "%d", &(pares[i].valor));
        pares[i].linha = i;

        i++;
    }

    fclose(arquivo);
    return pares;
}

// imprime a linha expecífica do arquivo
void imprimirLinha(FILE *arquivo, int linha){ // ok
  char buffer[100];

  fseek(arquivo, (SIZE_DATA_LINE * linha), SEEK_SET); // desloca o ponteiro do arquivo até a linha

  fgets(buffer, sizeof(buffer), arquivo);

  printf("%s\n", buffer);

  rewind(arquivo); // volta o ponteiro até o início
}

// itera sobre o arquivo procurando a linha com indice
int imprimirLinear(FILE *arquivo, int id){ // ok
  char buffer[100];

  int h = 0;

  while(fgets(buffer, sizeof(buffer), arquivo) != NULL){
    sscanf(buffer, "%d", &h);

    if(h == id) { // se encontrou
      printf("%s\n", buffer);
      rewind(arquivo);
      return 1;
    }
  }

  return 0; // se não encontrou
}

/* Árvore */
No* criarNo(){ // ok
    No *novoNo = (No*) malloc(sizeof(No));
    if(novoNo == NULL){
        printf("Alocação de memoria falhou!\n");
        return NULL;
    }

    novoNo->n = 0;
    novoNo->folha = 1;

    for(int i = 0; i < MAX_KEYS + 1; i++){
        novoNo->filhos[i] = NULL;
    }

    return novoNo;

}

void split(No *pai, int indice, No *filho){ // ok
    No *novoNo = criarNo();
    novoNo->folha = filho->folha;
    novoNo->n = MAX_KEYS / 2;

    for(int i = 0; i < novoNo->n; i++){
        novoNo->chaves[i] = filho->chaves[i + MAX_KEYS / 2];
    }

    if(!filho->folha){
        for(int i = 0; i < (MAX_KEYS / 2) + 1; i++){
            novoNo->filhos[i] = filho->filhos[i + MAX_KEYS / 2];
        }
    }

    filho->n = MAX_KEYS / 2;

    for(int i = pai->n; i > indice; i--){
        pai->filhos[i + 1] = pai->filhos[i];
    }

    pai->filhos[indice + 1] = novoNo;

    for(int i = pai->n - 1; i >= indice; i--){
        pai->chaves[i + 1] = pai->chaves[i];
    }

    pai->chaves[indice] = filho->chaves[MAX_KEYS / 2 - 1];
    pai->n++;

}

void inserirNo(No *no, Par chave){ // ok
    int i = no->n - 1;

    if(no->folha){
        while(i >= 0 && chave.valor < no->chaves[i].valor){
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }

        no->chaves[i + 1] = chave;
        no->n++;

    }else{
        while(i >= 0 && chave.valor < no->chaves[i].valor){
            i--;
        }

        i++;

        if(no->filhos[i]->n == MAX_KEYS){
            split(no, i, no->filhos[i]);

            if(chave.valor > no->chaves[i].valor){
                i++;
            }

        }
        inserirNo(no->filhos[i], chave);

    }
}

void inserirBTree(BTree *arv, Par chave){ // ok
    No *raiz = arv->raiz;

    if(raiz->n == MAX_KEYS){
        No *novoNo = criarNo();

        novoNo->folha = 0;
        novoNo->filhos[0] = raiz;
        arv->raiz = novoNo;
        
        split(novoNo, 0, raiz);
        
        inserirNo(novoNo, chave);

    }else{
        inserirNo(raiz, chave);
    
    }
}

void imprimirNoBtree(No *no){ // ok
    for(int i = 0; i < no->n; i++){
        printf("Valor: %d, linha: %d\n", no->chaves[i].valor, no->chaves[i].linha);
    }

    if(!no->folha){
        for(int i = 0; i <= no->n; i++){
            imprimirNoBtree(no->filhos[i]);
        }
    }
}

void imprimirBTree(BTree *arv){ // ok
    if(arv->raiz != NULL){
        imprimirNoBtree(arv->raiz);
    }
}

No* pesquisarNo(No* no, int valor){ // ok
    int i = 0;

    while(i < no->n && valor > no->chaves[i].valor){
        i++;
    }

    if(i < no->n && valor == no->chaves[i].valor){
        return no;
    }

    if(no->folha)return NULL;

    return pesquisarNo(no->filhos[i], valor); // se não achou, repete para o filho
}

void procurarPokemon(BTree arv, int valor, FILE *arquivo){ // ok
    No *achou = pesquisarNo(arv.raiz, valor);

    if(achou != NULL){ // se encontrou
        for(int i = 0; i < achou->n; i++){
            if(achou->chaves[i].valor == valor){
                Par *par = &(achou->chaves[i]);
                imprimirLinha(arquivo, par->linha);
                break;
            }
        }

    }else{ // se não encontrou
        printf("Não existe nó com chave %d na árvore!\n", valor);
    
    }
}

Par merge(No *no, int indice){ // ok
    No *filhoEsq = no->filhos[indice];
    No *filhoDir = no->filhos[indice + 1];

    filhoEsq->chaves[filhoEsq->n] = no->chaves[indice];
    filhoEsq->n++;

    for(int i = 0; i < filhoDir->n; i++){
        filhoEsq->chaves[filhoEsq->n + i] = filhoDir->chaves[i];
    }

    if(!filhoEsq->folha){
        for(int i = 0; i <= filhoDir->n; i++){
            filhoEsq->filhos[filhoEsq->n + i] = filhoDir->filhos[i];
        }
    
    }

    for(int i = indice; i < no->n - 1; i++){
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }

    no->n--;

    free(filhoDir);

    return no->chaves[indice];
}

void ajustaFilho(No *no, int indice){ // ok
    No *filhoEsq = no->filhos[indice - 1];
    No *filhoDir = no->filhos[indice + 1];

    if(indice > 0 && filhoEsq->n > MIN_KEYS){
        No *filho = no->filhos[indice];
        for(int i = filho->n; i > 0; i--){
            filho->chaves[i] = filho->chaves[i - 1];
        }
        filho->chaves[0] = no->chaves[indice - 1];
        no->chaves[indice - 1] = filhoEsq->chaves[filhoEsq->n - 1];

        if(!filho->folha){
            for(int i = filho->n + 1; i > 0; i--){
                filho->filhos[i] = filho->filhos[i - 1];
            }
            filho->filhos[0] = filhoEsq->filhos[filhoEsq->n];
        }

        filhoEsq->n--;
        filho->n++;

    }else if(indice < no->n && filhoDir->n > MIN_KEYS){
        No *filho = no->filhos[indice];
        filho->chaves[filho->n] = no->chaves[indice];
        no->chaves[indice] = filhoDir->chaves[0];

        if(!filho->folha)filho->filhos[filho->n + 1] = filhoDir->filhos[0];

        for(int i = 0; i < filhoDir->n - 1; i++){
            filhoDir->chaves[i] = filhoDir->chaves[i + 1];
        }

        if(!filhoDir->folha){
            for (int i = 0; i < filhoDir->n; i++) {
                filhoDir->filhos[i] = filhoDir->filhos[i + 1];
            }
        }

        filhoDir->n--;
        filho->n++;

    }else{
        if(indice > 0){
            merge(no, indice - 1);

        } else {
            merge(no, indice);

        }
    }
}

void excluirChaveBtree(BTree *arv, int valor){ // ok
    if(arv->raiz == NULL){
        printf("B-tree está vazia!\n");

        return;
    }

    excluirChaveNo(arv->raiz, valor);

    if(arv->raiz->n == 0) {
        No *antigaRaiz = arv->raiz;
        arv->raiz = arv->raiz->filhos[0];
        free(antigaRaiz);
    }
}

void excluirChaveNo(No *no, int valor){
    int i = 0;

    while(i < no->n && valor > no->chaves[i].valor){
        i++;
    }

    if(i < no->n && valor == no->chaves[i].valor){
        if(no->folha){
            for(int j = i; j < no->n - 1; j++){
                no->chaves[j] = no->chaves[j + 1];
            }

            no->n--;

        }else{
            No *filhoEsq = no->filhos[i];
            No *filhoDir = no->filhos[i + 1];

            if(filhoEsq->n >= MIN_KEYS){
                No *predecessor = filhoEsq;
                while(!predecessor->folha){
                    predecessor = predecessor->filhos[predecessor->n];
                }
                no->chaves[i] = predecessor->chaves[predecessor->n - 1];

                excluirChaveNo(filhoEsq, predecessor->chaves[predecessor->n - 1].valor);

            }else if(filhoDir->n >= MIN_KEYS) {
                No *sucessor = filhoDir;
                while (!sucessor->folha) {
                    sucessor = sucessor->filhos[0];
                }
                no->chaves[i] = sucessor->chaves[0];

                excluirChaveNo(filhoDir, sucessor->chaves[0].valor);

            }else{
                no->chaves[i] = merge(no, i);

                excluirChaveNo(no->filhos[i], valor);
            }
        }

    }else{
        No *filho = no->filhos[i];

        if(filho->n == MIN_KEYS){
            ajustaFilho(no, i);

        }

        excluirChaveNo(filho, valor);
    }
}
