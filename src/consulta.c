#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "consulta.h"
#include <math.h>


ListaUnidades* criarListaUnidade() {
    ListaUnidades *lista = malloc(sizeof(ListaUnidades));
    lista->tamanho = 0;
    lista->dados = NULL;
    return lista;
}

void adicionarNaListaUnidade(ListaUnidades *lista, UnidadeJurisdiciona_Struct* unidade){
        int itemExistente = buscarUnidade(lista, unidade->sigla_tribunal);

        if(itemExistente != -1){
            lista->dados[itemExistente].julgados_2026 += unidade->julgados_2026;
            lista->dados[itemExistente].casos_novos_2026 += unidade->casos_novos_2026;
            lista->dados[itemExistente].dessobrestados_2026 += unidade->dessobrestados_2026;
            lista->dados[itemExistente].suspensos_2026 += unidade->suspensos_2026;
            lista->dados[itemExistente].julgm2_a += unidade->julgm2_a;
            lista->dados[itemExistente].distm2_a += unidade->distm2_a;
            lista->dados[itemExistente].suspm2_a += unidade->suspm2_a;
            lista->dados[itemExistente].julgm2_ant += unidade->julgm2_ant;
            lista->dados[itemExistente].distm2_ant += unidade->distm2_ant;
            lista->dados[itemExistente].suspm2_ant += unidade->suspm2_ant;
            lista->dados[itemExistente].desom2_ant += unidade->desom2_ant;
            lista->dados[itemExistente].julgm4_a += unidade->julgm4_a;
            lista->dados[itemExistente].distm4_a += unidade->distm4_a;
            lista->dados[itemExistente].suspm4_a += unidade->suspm4_a;
            lista->dados[itemExistente].julgm4_b += unidade->julgm4_b;
            lista->dados[itemExistente].distm4_b += unidade->distm4_b;
            lista->dados[itemExistente].suspm4_b += unidade->suspm4_b;

            return;
        }

        lista->dados = realloc(lista->dados,
            (lista->tamanho + 1) * sizeof(UnidadeJurisdiciona_Struct));

        lista->dados[lista->tamanho] = *unidade;
        lista->tamanho++;
}

int buscarUnidade(ListaUnidades *lista, char *sigla){
    for(int i = 0; i < lista->tamanho; i++){
        if(strcmp(lista->dados[i].sigla_tribunal, sigla) == 0){
            return i;
        }
    }
    return -1;
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
    char linha[TamanhoLinha];
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        return 0;
    }

    teste = sscanf(linha,"\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",%d,\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
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
        &UnidadeStruct->dessobrestados_2026,
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
        &UnidadeStruct->julgm4_a, 
        &UnidadeStruct->suspm4_a,
        &UnidadeStruct->cumprimento_meta4a,
        &UnidadeStruct->distm4_b,
        &UnidadeStruct->julgm4_b,
        &UnidadeStruct->suspm4_b,
        &UnidadeStruct->cumprimento_meta4b
    );
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
    printf("=========================================\n");
    printf(" Ferramenta TRE - Consulta de Dados CSV\n");
    printf("=========================================\n\n");

    printf("Uso:\n");
    printf("  ./tre.exe [comando] [opcoes]\n\n");

    printf("Comandos disponiveis:\n\n");

    printf("1. Ajuda (-h)\n");
    printf("   Exibe esta mensagem de ajuda\n");
    printf("   Exemplo:\n");
    printf("     ./tre.exe -h\n\n");

    printf("2. Versao (-v)\n");
    printf("   Exibe a versao do sistema\n");
    printf("   Exemplo:\n");
    printf("     ./tre.exe -v\n\n");

    printf("3. Busca por Municipio (-b)\n");
    printf("   Busca dados de unidades por municipio\n");
    printf("   Uso:\n");
    printf("     ./tre.exe -b <arquivo_entrada> -m <nome_municipio>\n");
    printf("   Parametros:\n");
    printf("     <arquivo_entrada>   Caminho do arquivo CSV\n");
    printf("     <nome_municipio>    Nome exato do municipio\n");
    printf("   Exemplos:\n");
    printf("     ./tre.exe -b dados.csv -m ARAPIRACA\n");
    printf("     ./tre.exe -b dados.csv -m \"BELA VISTA\"\n\n");

    printf("4. Concatenar Arquivos (-c)\n");
    printf("   Junta multiplos arquivos CSV em um unico arquivo\n");
    printf("   Uso:\n");
    printf("     ./tre.exe -c <arquivo1> <arquivo2> ... <arquivoN>\n");
    printf("     ./tre.exe -c -p <diretorio1> <diretorio2> ...\n");
    printf("   Opcoes:\n");
    printf("     -p    Concatenar todos os CSV de uma pasta\n");
    printf("     -o    Define nome do arquivo de saida\n");
    printf("   Observacao:\n");
    printf("     Caso nao informado, o nome sera gerado automaticamente\n");
    printf("   Exemplo:\n");
    printf("     ./tre.exe -c dados1.csv dados2.csv -o saida.csv\n\n");

    printf("5. Resumir com metricas (-r)\n");
    printf("   Gera resumo com calculos de desempenho\n");
    printf("   Uso:\n");
    printf("     ./tre.exe -r <arquivo_entrada>\n\n");

    printf("Observacoes:\n");
    printf("  Linhas podem ser ignoradas caso tenha falha na leitura\n");

    printf("=========================================\n");
    return 0;
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
            linhasLidas++;
            linhasPuladas++;
        }
        camposLidos = alimentarStruct(arquivoDeBusca, unidade);        
    } while(camposLidos > 1);
    
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
    printf("versão 1.1.0 BETA\n");
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
// remove e retorna a posição removida, se não achar retorna -1
int removerDaLista(char lista[50][100], char* palavra){
    for(int i = 0; i < 50; i++){
        if(strcmp(lista[i], palavra) == 0){
            lista[i][0] = '\0'; 
            return i;
        }
    }
    return -1;
}

double safe_div(double numerador, double denominador) {
    if (!isfinite(denominador) || denominador == 0) {
        return 0.0; 
    }
    return numerador / denominador;
}

int cmd_resumir(int argc, char **argv){
    if(argc < 2) return ERRO;

    ARQUIVO* arquivoBusca = abrirArquivo(argv[1], "r");
    if (arquivoBusca == NULL) {
        return ERRO;
    }
    pularLinha(arquivoBusca);

    ARQUIVO* arquivoSaida = abrirArquivo("", "w");
    if (arquivoSaida == NULL) {
        fecharArquivo(arquivoBusca);
        return ERRO;
    }

    UnidadeJurisdiciona_Struct* unidade = malloc(sizeof(UnidadeJurisdiciona_Struct));
    ListaUnidades* listaDeUnidades = criarListaUnidade(); 
    if (unidade == NULL || listaDeUnidades == NULL) {
        free(unidade);
        if (listaDeUnidades != NULL) {
            free(listaDeUnidades->dados);
            free(listaDeUnidades);
        }
        fecharArquivo(arquivoBusca);
        fecharArquivo(arquivoSaida);
        return ERRO;
    }

    char lista[50][100] = {0};

    int linhasLidas = 0;
    int linhasPuladas = 0;

    int camposLidos;

    while((camposLidos = alimentarStruct(arquivoBusca->fileArquivo, unidade)) > 0){
        linhasLidas++;

        if(camposLidos == 33){
            if(temNaLista(lista, unidade->sigla_tribunal) == -1){
                adicionarNaLista(lista, unidade->sigla_tribunal);
            }
        } else {
            linhasPuladas++;
        }
    }
    printf("linhas lidas: %d\nlinhas puladas: %d\n", linhasLidas, linhasPuladas);

    rewind(arquivoBusca->fileArquivo);
    pularLinha(arquivoBusca);

    while((camposLidos = alimentarStruct(arquivoBusca->fileArquivo, unidade)) > 0){
        if(camposLidos == 33){
            for(int i = 0; i < 50; i++){
                if(strlen(lista[i]) > 0){
                    if(strcmp(unidade->sigla_tribunal, lista[i]) == 0){
                        adicionarNaListaUnidade(listaDeUnidades, unidade);
                    }
                }
            }
        }
    }
    fprintf(arquivoSaida->fileArquivo, "\"sigla_tribunal\",\"Meta1\",\"Meta2A\",\"Meta2Ant\",\"Meta4A\",\"Meta4B\"\n");
    for(int i = 0; i < listaDeUnidades->tamanho; i++){
        UnidadeJurisdiciona_Struct u = listaDeUnidades->dados[i];     

        double meta1   = safe_div(u.julgados_2026, (u.casos_novos_2026 + u.dessobrestados_2026 - u.suspensos_2026)) * 100.0;
        double meta2A  = safe_div(u.julgm2_a, (u.distm2_a - u.suspm2_a)) * (1000.0 / 7.0);
        double meta2ant = safe_div(u.julgm2_ant, (u.distm2_ant - u.suspm2_ant - u.desom2_ant)) * 100.0;
        double meta4A  = safe_div(u.julgm4_a, (u.distm4_a - u.suspm4_a)) * 100.0;
        double meta4B  = safe_div(u.julgm4_b, (u.distm4_b - u.suspm4_b)) * 100.0;

        fprintf(arquivoSaida->fileArquivo,
                "%s;%.2lf;%.2lf;%.2lf;%.2lf;%.2lf;\n",
                u.sigla_tribunal,
                meta1,
                meta2A,
                meta2ant,
                meta4A,
                meta4B
        );
    }

    printf("resultado: %s\n", arquivoSaida->nomeArquivo);

    free(unidade);
    free(listaDeUnidades->dados);
    free(listaDeUnidades);
    fecharArquivo(arquivoBusca);
    fecharArquivo(arquivoSaida);

    return 0;
}


