#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "consulta.h"


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
    // retirar o "-b"
    argc--;
    argv++;

    char nomeArquivo[] = argv[0];
    FILE *arquivoDeBusca = fopen(nomeArquivo, "r");
    if(arquivoDeBusca == 0){
        perror("Falha ao acessar o arquivo de busca");
        return ERRO;
    }
    UnidadeJurisdiciona_Struct * unidadeStruct = malloc(1 * sizeof(UnidadeJurisdiciona_Struct));

    fscanf(arquivoDeBusca, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];\n", 
        unidadeStruct->sigla_tribunal,
        unidadeStruct->procedimento,
        unidadeStruct->ramo_justica,
        unidadeStruct->sigla_grau,
        unidadeStruct->uf_oj,
        unidadeStruct->municipio_oj,
        unidadeStruct->id_ultimo_oj,
        unidadeStruct->nome,
        unidadeStruct->mesano_cnm1,
        unidadeStruct->mesano_sent,
        unidadeStruct->casos_novos_2026,
        unidadeStruct->julgados_2026,
        unidadeStruct->prim_sent2026,
        unidadeStruct->suspensos_2026,
        unidadeStruct->dessobrestados_2026
    );
};

int cmd_versao(int argc, char **argv){
    printf("versão 1.0.0 BETA\n");
};

int cmd_concatenar(int argc, char **argv) {
    
    char arquivos[1024][120];
    int qtdarquivos = 0;
    char pastas[10][120];
    int qtdPastas = -1;
    char nomeSaida[120];
    int leituraPasta = 0;
    int nomeSaidaPersonalizado = 0;
    // retirar o "-c"
    argc--;
    argv++;
    

    for(int i = 0; i < argc; i++){
        if(strcmp("-p", argv[i]) == 0){
            if(i+1 < argc){
                leituraPasta = 1;
                continue;
            } else{
                break;
            }
        } 
        if(strcmp("-o",argv[i]) == 0 && i + 1 < argc){
            strcpy(nomeSaida, argv[i+1]);
            nomeSaidaPersonalizado = 1;
            break;
        } else{
            if(leituraPasta == 1){
                if (qtdPastas < 10) {
                    qtdPastas++;
                    strcpy(pastas[qtdPastas], argv[i]);
                }else{
                    printf("Limite de 10 pastas atingido!\n");
                    break;
                }
            }else{
                if (qtdarquivos < 1024) {
                    strcpy(arquivos[qtdarquivos], argv[i]);
                    qtdarquivos++;
                }else{
                    printf("Limite de 1024 Arquivos atingido!\n");
                    break;
                }
            }
        }
    }
    if(qtdPastas >= 0){
        for(int i = 0; i <= qtdPastas; i++){
            DIR *dir = opendir(pastas[i]);
            if(dir == 0){
                printf("falha ao abrir o diretorio!\n");
                perror("Erro");
                return ERRO;
            }
            struct dirent * arquivo;
            while ((arquivo = readdir(dir))){
                char caminhoCompleto[sizeof(pastas[i])+sizeof(arquivo->d_name)+1] = "";
                sprintf(caminhoCompleto, "%s/%s", pastas[i], arquivo->d_name);
                if (qtdarquivos < 1024) {
                    if (strstr(caminhoCompleto, ".csv") != NULL) {
                        strcpy(arquivos[qtdarquivos], caminhoCompleto);
                        qtdarquivos++;
                    } else {
                        continue;
                    }
                }else{
                    printf("Limite de 1024 Arquivos atingido!\n");
                    closedir(dir);
                    break;
                }
            } 
            closedir(dir);
        }
    }

    if(qtdarquivos <= 1){
        return ERRO;
    }
        
    // cria o nome de saida padrão no formato output_Dia-Mês-Ano_Hora-Minutos-Segundos
    if(nomeSaidaPersonalizado == 0){
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



