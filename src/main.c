#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "btree.h"

int main(){
    int op = 0, elem;
    BTree arv;
    arv.raiz = criarNo();
    FILE *f = fopen(FILE_NAME, "r");

    Par *pares = processaDados();

    do{
        printf("\nMenu: ");
        printf("\n1 - Criar índice;"
               "\n2 - Procurar elementos;"
               "\n3 - Remover registro;"
               "\n4 - Sair.\n"
               "\nOpção: ");
        scanf("%d",&op);

        if(op == 1){
            printf("Processando...\n");
            sleep(1);

            for(int i = 0; i < NUM_LINES; i++) {
              inserirBTree(&arv, pares[i]);
            }
        }

        if(op == 2){
          printf("Insira o índice do pokémon: ");
          scanf("%d", &elem);
          printf("\n");
          
          procurarPokemon(arv, elem, f);
        }

        if(op == 3){
          printf("Insira o índice do pokémon: ");
          scanf("%d", &elem);

          if(elem <= 0 || elem > NUM_LINES){
            printf("\nÍndice inválido!\n");
            continue;
          }

          excluirChaveBtree(&arv, elem);
          printf("Pokémon excluído com sucesso!\n");
        }
    }
    while(op != 4);


    free(pares);
    fclose(f);
    return 0;
}
