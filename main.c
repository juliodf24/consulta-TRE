#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consulta.h"

enum ListaComandos{
    CONCATENAR,
    BUSCAR,
    RESUMIR,
    AJUDA,
    VERSAO
};

int encontrarComando(char *comando){
    if (strcmp(comando, comando_buscar) == 0) {
        return BUSCAR;
    } else if (strcmp(comando, comando_versao) == 0) {
        return VERSAO;
    } else {
        return AJUDA;
    }
}

void concatenar() { printf("Concatenar\n"); exit(0);}
void buscar() { printf("Buscar\n"); exit(0);}
void resumir() { printf("Resumir\n"); exit(0);}
void ajuda() { printf("Ajuda\n"); exit(0);}
void versao() { printf("Versao\n"); exit(0);}

int main(int argc, char *argv[]) {

    if(argc < 1){
        ajuda();
        exit(0);
    }

    int comando = encontrarComando(argv[1]);

    switch (comando)
    {
    case BUSCAR:
        buscar();
        break;
    case AJUDA:
        ajuda();
        break;
    case VERSAO:
        versao();
        break;
    
    default:
        break;
    }

    return 0;
}
