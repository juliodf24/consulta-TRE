#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "consulta.h"

ListaUnidades* criarListaUnidade() {
    ListaUnidades *lista = malloc(sizeof(ListaUnidades));
    lista->tamanho = 0;
    lista->dados = NULL;
    return lista;
}

void adicionarNaListaUnidade(ListaUnidades *lista, UnidadeJurisdiciona_Struct item) {
    lista->dados = realloc(lista->dados,(lista->tamanho + 1) * sizeof(UnidadeJurisdiciona_Struct));
    lista->dados[lista->tamanho] = item;
    lista->tamanho++;
}

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
int alimentarStruct(FILE *arquivo, UnidadeJurisdiciona_Struct * UnidadeStruct){
    int teste = 0;

    teste += fscanf(arquivo, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",%d,\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",%lf,%lf,%lf,%lf,%lf",
        UnidadeStruct->sigla_tribunal,
        UnidadeStruct->procedimento,
        UnidadeStruct->ramo_justica,
        UnidadeStruct->sigla_grau,
        UnidadeStruct->uf_oj,
        UnidadeStruct->municipio_oj,
        &UnidadeStruct->id_ultimo_oj,
        UnidadeStruct->nome,
        UnidadeStruct->mesano_cnm1,
        UnidadeStruct->mesano_sent,
        &UnidadeStruct->casos_novos_2026,
        &UnidadeStruct->julgados_2026,
        &UnidadeStruct->prim_sent2026,
        &UnidadeStruct->suspensos_2026,
        &UnidadeStruct->dessobrestados_2026
    );

    teste += fscanf(arquivo,",%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
        &UnidadeStruct->cumprimento_meta1,
        &UnidadeStruct->distm2_a,
        &UnidadeStruct->julgm2_a,
        &UnidadeStruct->suspm2_a,
        &UnidadeStruct->cumprimento_meta2a,
        &UnidadeStruct->distm2_ant,
        &UnidadeStruct->julgm2_ant,
        &UnidadeStruct->suspm2_ant,
        &UnidadeStruct->desom2_ant,
        &UnidadeStruct->cumprimento_meta2ant,
        &UnidadeStruct->distm4_a,
        &UnidadeStruct->julgm4_a
    );

    teste += fscanf(arquivo,",%lf,%lf,%lf,%lf,%lf,%lf\n",
        &UnidadeStruct->suspm4_a,
        &UnidadeStruct->cumprimento_meta4a,
        &UnidadeStruct->distm4_b,
        &UnidadeStruct->julgm4_b,
        &UnidadeStruct->suspm4_b,
        &UnidadeStruct->cumprimento_meta4b
    );
    printf("teste: %d\nLinha Aqui: %d\n", teste, UnidadeStruct->id_ultimo_oj);
    return teste;

}
int escreverAPartirDeStruct(FILE *arquivo, UnidadeJurisdiciona_Struct * UnidadeStruct){
    int teste = 0;

    teste += fprintf(arquivo, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,\"%s\",\"%s\",\"%s\",%.0lf,%.0lf,%.0lf,%.0lf,%.0lf",
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

    teste += fprintf(arquivo,",%.0lf,%.0lf,%.0lf,%.0lf,%.0lf,%.0lf,%.0lf,%.0lf,%.0lf,%.0lf,%.0lf,%.0lf",
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

    teste += fprintf(arquivo,",%.0lf,%.0lf,%.0lf,%.0lf,%.0lf,%.0lf\n",
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

int pularLinha(ARQUIVO* arquivo){
    char linha[TamanhoLinha];
    fgets(linha, sizeof(linha), arquivo->fileArquivo);
}

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
    UnidadeJurisdiciona_Struct * unidade = malloc(1 * sizeof(UnidadeJurisdiciona_Struct));

    // pular linha
    char linha[TamanhoLinha];
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
    int linhasPuladas = 0;
    int itensEncontrados = 0;
  
    // 33 quantidade de itens lidos ou seja quantidade de colunas no csv
    int camposLidos = alimentarStruct(arquivoDeBusca, unidade);

    do{
        if(camposLidos == 33){
            if(strcmp(opcao, "-m") == 0){
                if(strcmp(palavraBusca, unidade->municipio_oj) == 0){
                    escreverAPartirDeStruct(output, unidade);
                    itensEncontrados++;
                }
                linhasLidas++;
            }
        } else {
            linhasPuladas++;
        }
        camposLidos = alimentarStruct(arquivoDeBusca, unidade);        
    } while(camposLidos > 0);

    printf("Acabou camposLidos: %d\n", camposLidos);
    
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

    printf("Linhas Lidas: %d\nLinhas Puladas: %d\nItens encontrados: %d\nArquivo resultante: %s\n", linhasLidas,linhasPuladas, itensEncontrados, nomeSaida);

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

// retorna o local se achar, senão -1
int temNaLista(char lista[50][100], char* palavra){
    for(int i = 0; i < 50; i++){
        if(strcmp(lista[i], palavra) == 0){
            return i;
        }
    }
    return -1;
}

// adiciona e retorna local se nao -1
int adicionarNaLista(char lista[50][100], char* palavra){
    for(int i = 0; i < 50; i++){
        if(strlen(lista[i]) == 0){
            strcpy(lista[i], palavra);
            return i;
        }
    }
    return -1;
}

int cmd_resumir(int argc, char **argv){
    if(argc < 2) return ERRO;

    ARQUIVO* arquivoBusca = abrirArquivo(argv[1], "r");
    pularLinha(arquivoBusca);
    ARQUIVO* arquivoSaida = abrirArquivo("", "w");
    UnidadeJurisdiciona_Struct* unidade = malloc(sizeof(UnidadeJurisdiciona_Struct));
    char lista[50][100] = {0};
    memset(lista, 0, sizeof(lista));

    
    int leu = alimentarStruct(arquivoBusca->fileArquivo, unidade);
    if(temNaLista(lista, unidade->sigla_tribunal) == -1){
        adicionarNaLista(lista, unidade->sigla_tribunal);
    }

    printf("leu: %d\nunidade: %s\nLISTA: %s", leu,unidade->sigla_tribunal,lista[0]);

    fecharArquivo(arquivoBusca);
    excluirArquivo(arquivoSaida);

}


