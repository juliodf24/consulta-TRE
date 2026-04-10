#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "consulta.h"

ARQUIVO* abrirArquivo(char* nome, char* tipoAbertura) {
    char nomeArquivo[LimiteNomeArquivo + 1];
    if (strlen(nome) == 0) {
        time_t agora = time(0);
        struct tm *t = localtime(&agora);
        strftime(nomeArquivo, sizeof(nomeArquivo),"output_%d-%m-%Y_%H-%M-%S.csv", t);
    } else {
        strcpy(nomeArquivo, nome);
    }
    FILE* arquivoAberto = fopen(nomeArquivo, tipoAbertura);
    if (!arquivoAberto) {
        perror("Erro ao abrir arquivo");
        return 0;
    }
    ARQUIVO* arq = malloc(sizeof(ARQUIVO));
    if (!arq) {
        perror("Erro ao alocar memória");
        fclose(arquivoAberto);
        return 0;
    }
    arq->fileArquivo = arquivoAberto;
    strcpy(arq->nomeArquivo, nomeArquivo);
    return arq;
}

int fecharArquivo(ARQUIVO *arquivo) {
    if (!arquivo) return 0;
    if (arquivo->fileArquivo) {
        fclose(arquivo->fileArquivo);
    }
    free(arquivo);
    return 0;
}

int excluirArquivo(ARQUIVO *arquivo){
    if (!arquivo) return 0;
    if (arquivo->fileArquivo) {
        fclose(arquivo->fileArquivo);
    }
    remove(arquivo->nomeArquivo);
    free(arquivo);
    return 0;
}

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

    ARQUIVO* arquivoBusca = abrirArquivo(nomeArquivo, "r");
    
    Unidade_Struct * unidade = malloc(1 * sizeof(Unidade_Struct));

    // pular linha
    char linha[1024];
    fgets(linha, sizeof(linha), arquivoBusca->fileArquivo);
    
    // arquivo de saida
    char nomeSaida[500];
    snprintf(nomeSaida, sizeof(nomeSaida), "%s.csv", palavraBusca);

    ARQUIVO* arquivoSaida = abrirArquivo(nomeArquivo, "w");
    
    fputs(linha, arquivoSaida->fileArquivo);

    int linhasLidas = 0;
    int itensEncontrados = 0;
  
    // 33 quantidade de itens lidos ou seja quantidade de colunas no csv
    while (alimentarStruct(arquivoBusca->fileArquivo, unidade) == 33) {
        if(strcmp(opcao, "-m") == 0){
            if(strcmp(palavraBusca, unidade->municipio_oj) == 0){
                escreverAPartirDeStruct(arquivoSaida->fileArquivo, unidade);
                itensEncontrados++;
            }
        }
        linhasLidas++;
    }
    
    if(linhasLidas < 1){
        perror("Nehuma linha lida");
        fecharArquivo(arquivoBusca);
        excluirArquivo(arquivoSaida);
        remove(nomeSaida);
        return ERRO;
    }
    if(itensEncontrados < 1){
        printf("Nehum item encontrado");
        fecharArquivo(arquivoBusca);
        excluirArquivo(arquivoSaida);
        remove(nomeSaida);
        return ERRO;
    }

    printf("Linhas Lidas: %d\nItens encontrados: %d\nArquivo resultante: %s\n", linhasLidas, itensEncontrados, nomeSaida);

    fecharArquivo(arquivoBusca);
    fecharArquivo(arquivoSaida);
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
    ARQUIVO* arquivoOutput = abrirArquivo("", "a");

    //Verifica se foi possivel criar ou abrir o arquivo de saida
    if(arquivoOutput->fileArquivo == NULL){
        printf("Erro ao criar o Arquivo de saida!");
        return ERRO;
    }

    for(int i = 0; i < qtdarquivos; i++){
        int resultado;
        if(i == 0) resultado = concatenar(arquivos[i], arquivoOutput->fileArquivo, 1);
        else resultado = concatenar(arquivos[i], arquivoOutput->fileArquivo, 0);

        if(resultado == ERRO){
            remove(nomeSaida);
            break;
        }
    }
    
    printf("Arquivo Gerado: %s\n", arquivoOutput->nomeArquivo);
    fecharArquivo(arquivoOutput);
    return 0;
}



