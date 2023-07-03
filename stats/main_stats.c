// by Diego Reis

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/btree.h"

#define SEARCH_NUM 30
#define NUM_ELEM 10000

/* 
Esse programa compara o tempo de pesquisa usando busca linear
e utilizando os indices da b-tree
*/
int main(){
    srand(time(NULL));

    double meanL = 0.0, meanB = 0.0, time_spent, maxL = 0, minL = 0, maxB = 0, minB = 0; 
    int randN;

    Par *pares = processaDados();
    FILE *arquivo = fopen(FILE_NAME, "r");

    BTree arv;
      arv.raiz = criarNo();

    // popula a B-tree
    for(int i = 0; i < 10000; i++)insertBTree(&arv, pares[i]);

    printf("Busca Linear\n===========================\n");

    // calcula a media, valor max e min para 30 buscas aleatorias
    for(int i = 0; i < SEARCH_NUM; i++) {
      randN = rand() % NUM_ELEM + 1;

      clock_t begin = clock();
      imprimirLinear(arquivo, randN);
      clock_t end = clock();

      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      meanL += time_spent;

      if(time_spent > maxL)maxL = time_spent;

      if(time_spent < minL)minL = time_spent;

    }


    printf("Busca B-Tree\n===========================\n");

    // calcula a media, valor max e min para 30 buscas aleatorias
    for(int i = 0; i < SEARCH_NUM; i++) {
      randN = rand() % NUM_ELEM + 1;

      clock_t begin = clock();
      procuraPokemon(arv, randN, arquivo);
      clock_t end = clock();

      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      meanB += time_spent;

      if (time_spent > maxB)maxB = time_spent;

      if (time_spent < minB)minB = time_spent;
    }


    printf("Busca Linear\n===========================\n");
    printf("Tempo médio da busca Linear: %f\n", (double) meanL / SEARCH_NUM);
    printf("Menor tempo da busca Linear: %f\n", minL);
    printf("Maior tempo da busca Linear: %f\n\n", maxL);


    printf("Busca BTree\n===========================\n");
    printf("Tempo médio da busca Btree: %f\n", (double) meanB / SEARCH_NUM);
    printf("Menor tempo da busca Btree: %f\n", minB);
    printf("Maior tempo da busca Btree: %f\n", maxB);


    fclose(arquivo);
    return 0;
}

