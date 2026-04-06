#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consulta.h"
#include <time.h>


int concatenar(char* caminhoArquivo, FILE* output, int primeiro){
    char linha[1024];
    FILE *arquivo = fopen(caminhoArquivo, "r");
    if(arquivo == NULL){
        printf("Falha ao abrir o arquivo: %s\n", caminhoArquivo);
        perror("Erro");
        return ERRO;
    }
    if(output == NULL){
        printf("Falha no arquivo de saida!\n");
        perror("Erro");
        fclose(arquivo);
        return ERRO;
    }
    if(!primeiro) fgets(linha, sizeof(linha), arquivo);
    while (fgets(linha, sizeof(linha), arquivo)){
        fwrite(linha, sizeof(char), strlen(linha), output);
    }

    fclose(arquivo);
    return 0;
}

int cmd_help(int argc, char **argv){
    printf("ajuda\n");
};
int cmd_buscar(int argc, char **argv){
    printf("buscar\n");
};
int cmd_versao(int argc, char **argv){
    printf("versão 1.0.0 BETA\n");
};
int cmd_concatenar(int argc, char **argv) {
    
    char arquivos[100][120];
    char nomeSaida[120];
    int qtdarquivos = 0;
    // retirar o "-c"
    argc--;
    argv++;

    for(int i = 0; i < argc; i++){
        if(strcmp("-o",argv[i]) == 0 && i + 1 < argc){
            strcpy(nomeSaida, argv[i+1]);
            break;
        } else{
            if (qtdarquivos < 100) {
                strcpy(arquivos[qtdarquivos], argv[i]);
                qtdarquivos++;
            }else{
                break;
            }
        }
    }

        
    // cria o nome de saida padrão no formato output_Dia-Mês-Ano_Hora-Minutos-Segundos
    if(strlen(nomeSaida) < 2){
        time_t agora = time(NULL);
        struct tm *t = localtime(&agora);

        char nomePadraoData[32];
        strftime(nomePadraoData, sizeof(nomePadraoData), "output_%d-%m-%Y_%H-%M-%S.csv", t);
        strcpy(nomeSaida, nomePadraoData);
    }

    FILE *output = fopen(nomeSaida, "a");

    //Verifica se foi possivel criar ou abrir o arquivo de saida
    if(output == NULL){
        printf("Erro ao criar o Arquivo de saida!");
        return ERRO;
    }

    for(int i = 0; i < qtdarquivos; i++){
        int resultado;
        if(i == 0) resultado = concatenar(arquivos[i], output, 1);
        else resultado = concatenar(arquivos[i], output, 0);

        if(resultado == ERRO){
            remove(nomeSaida);
            break;
        }
    }
    
    fclose(output);

    return 0;
}



