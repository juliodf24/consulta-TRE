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

// alimenta a struct com uma linha e anda o ponterio do arquivo 1 linha 
int alimentarStruct(FILE *arquivo, Unidade_Struct *u) {
    int catacteresLidos = 0;
    char linha[2048];
    fgets(linha, sizeof(linha), arquivo);
    printf("Linha: %s\nCaracteres Lidos: %d", linha, catacteresLidos);
    if (!catacteresLidos)
        return 0;

    

    return 0;
}
int escreverAPartirDeStruct(FILE *arquivo, Unidade_Struct * UnidadeStruct){
    int teste = 0;

    teste += fprintf(arquivo, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,\"%s\",\"%s\",\"%s\",%d,%d,%d,%d,%d",
        UnidadeStruct->sigla_tribunal,
        UnidadeStruct->procedimento,
        UnidadeStruct->ramo_justica,
        UnidadeStruct->sigla_grau,
        UnidadeStruct->uf_oj,
        UnidadeStruct->municipio_oj,
        UnidadeStruct->id_ultimo_oj,
        UnidadeStruct->nome,
        UnidadeStruct->mesano_cnm1,
        UnidadeStruct->mesano_sent,
        UnidadeStruct->casos_novos_2026,
        UnidadeStruct->julgados_2026,
        UnidadeStruct->prim_sent2026,
        UnidadeStruct->suspensos_2026,
        UnidadeStruct->dessobrestados_2026
    );

    teste += fprintf(arquivo,",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
        UnidadeStruct->cumprimento_meta1,
        UnidadeStruct->distm2_a,
        UnidadeStruct->julgm2_a,
        UnidadeStruct->suspm2_a,
        UnidadeStruct->cumprimento_meta2a,
        UnidadeStruct->distm2_ant,
        UnidadeStruct->julgm2_ant,
        UnidadeStruct->suspm2_ant,
        UnidadeStruct->desom2_ant,
        UnidadeStruct->cumprimento_meta2ant,
        UnidadeStruct->distm4_a,
        UnidadeStruct->julgm4_a
    );

    teste += fprintf(arquivo,",%d,%d,%d,%d,%d,%d\n",
        UnidadeStruct->suspm4_a,
        UnidadeStruct->cumprimento_meta4a,
        UnidadeStruct->distm4_b,
        UnidadeStruct->julgm4_b,
        UnidadeStruct->suspm4_b,
        UnidadeStruct->cumprimento_meta4b
    );
    return teste;
}

int cmd_help(int argc, char **argv){
    printf("ajuda\n");
};

int cmd_buscar(int argc, char **argv){
    // retirar o "-b"
    argc--;
    argv++;

    if(argc < 3){
        return ERRO;
    }

    char nomeArquivo[255];
    char opcao[5];
    char palavraBusca[255];
    
    if(strcmp(argv[0], "-m") == 0){
        return ERRO;
    }

    strcpy(nomeArquivo, argv[0]);

    argc--;
    argv++;

    if(strcmp(argv[0], "-m") == 0){
        strcpy(opcao, argv[0]);
        strcpy(palavraBusca, argv[1]);
    }


    FILE *arquivoDeBusca = fopen(nomeArquivo, "r");
    if(arquivoDeBusca == 0){
        perror("Falha ao acessar o arquivo de busca");
        return ERRO;
    }
    Unidade_Struct * unidade = malloc(1 * sizeof(Unidade_Struct));

    // pular linha
    char linha[1024];
    fgets(linha, sizeof(linha), arquivoDeBusca);
    

    // arquivo de saida
    char nomeSaida[500];
    snprintf(nomeSaida, sizeof(nomeSaida), "%s.csv", palavraBusca);

    FILE *output = fopen(nomeSaida, "w");
    if(output == NULL){
        printf("Erro ao criar o Arquivo de saida!");
        return ERRO;
    }
    fputs(linha, output);

    int linhasLidas = 0;
    int itensEncontrados = 0;
  
    // 33 quantidade de itens lidos ou seja quantidade de colunas no csv
    while (alimentarStruct(arquivoDeBusca, unidade) == 33) {
        if(strcmp(opcao, "-m") == 0){
            if(strcmp(palavraBusca, unidade->municipio_oj) == 0){
                escreverAPartirDeStruct(output, unidade);
                itensEncontrados++;
            }
        }
        linhasLidas++;
    }
    
    if(linhasLidas < 1){
        perror("Nehuma linha lida");
        fclose(arquivoDeBusca);
        fclose(output);
        remove(nomeSaida);
        return ERRO;
    }
    if(itensEncontrados < 1){
        printf("Nehum item encontrado");
        fclose(arquivoDeBusca);
        fclose(output);
        remove(nomeSaida);
        return ERRO;
    }

    printf("Linhas Lidas: %d\nItens encontrados: %d\nArquivo resultante: %s\n", linhasLidas, itensEncontrados, nomeSaida);

    fclose(arquivoDeBusca);
    fclose(output);
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



