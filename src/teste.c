#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "consulta.h"

void replace(char *str, char antiga, char nova) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == antiga) {
            str[i] = nova;
        }
    }
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

char* proximoCampo(char **linha) {
    if (*linha == NULL) return NULL;

    char *inicio = *linha;
    char *fim;
    // campo com aspas
    if (*inicio == '\"') {
        inicio++; 
        fim = strchr(inicio, '\"');

        if (fim) {
            *fim = '\0';
            *linha = fim + 2;
        }
    } else {
    // campo sem aspas
        fim = strchr(inicio, ',');
        if (fim) {
            *fim = '\0';
            *linha = fim + 1;
        } else {
            *linha = NULL;
        }
    }

    return inicio;
}
void printUnidade(Unidade_Struct *u) {
    if (u == NULL) {
        printf("Struct NULL\n");
        return;
    }

    printf("sigla_tribunal: %s\n", u->sigla_tribunal);
    printf("procedimento: %s\n", u->procedimento);
    printf("ramo_justica: %s\n", u->ramo_justica);
    printf("sigla_grau: %s\n", u->sigla_grau);
    printf("uf_oj: %s\n", u->uf_oj);
    printf("municipio_oj: %s\n", u->municipio_oj);

    printf("id_ultimo_oj: %f\n", u->id_ultimo_oj);

    printf("nome: %s\n", u->nome);
    printf("mesano_cnm1: %s\n", u->mesano_cnm1);
    printf("mesano_sent: %s\n", u->mesano_sent);

    printf("casos_novos_2026: %f\n", u->casos_novos_2026);
    printf("julgados_2026: %f\n", u->julgados_2026);
    printf("prim_sent2026: %f\n", u->prim_sent2026);
    printf("suspensos_2026: %f\n", u->suspensos_2026);
    printf("dessobrestados_2026: %f\n", u->dessobrestados_2026);

    printf("cumprimento_meta1: %f\n", u->cumprimento_meta1);

    printf("distm2_a: %f\n", u->distm2_a);
    printf("julgm2_a: %f\n", u->julgm2_a);
    printf("suspm2_a: %f\n", u->suspm2_a);
    printf("cumprimento_meta2a: %f\n", u->cumprimento_meta2a);

    printf("distm2_ant: %f\n", u->distm2_ant);
    printf("julgm2_ant: %f\n", u->julgm2_ant);
    printf("suspm2_ant: %f\n", u->suspm2_ant);
    printf("desom2_ant: %f\n", u->desom2_ant);
    printf("cumprimento_meta2ant: %f\n", u->cumprimento_meta2ant);

    printf("distm4_a: %f\n", u->distm4_a);
    printf("julgm4_a: %f\n", u->julgm4_a);
    printf("suspm4_a: %f\n", u->suspm4_a);
    printf("cumprimento_meta4a: %f\n", u->cumprimento_meta4a);

    printf("distm4_b: %f\n", u->distm4_b);
    printf("julgm4_b: %f\n", u->julgm4_b);
    printf("suspm4_b: %f\n", u->suspm4_b);
    printf("cumprimento_meta4b: %f\n", u->cumprimento_meta4b);

    printf("====================================\n");
}


int lerStruct(ARQUIVO *arquivo, Unidade_Struct* unidade){
    char linha[TamanhoLinha];
    
    if (fgets(linha, sizeof(linha), arquivo->fileArquivo) == NULL) {
        return ERRO;
    }

    replace(linha, '\"', ' ');

    char linha_copia[TamanhoLinha];
    strcpy(linha_copia, linha);

    char *token = strtok(linha_copia, ",");
    int count = 0;

    while (token != NULL) {
        count++;
        token = strtok(NULL, ",");
    }

    printf("linha: %s\n", linha);

    if (count != 33) {
        printf("Erro: esperado 33 campos, mas veio %d\n", count);
        printf("linha: %s\n", linha);
        return ERRO;
    }

    token = strtok(linha, ",");

    strcpy(unidade->sigla_tribunal, token);
    token = strtok(NULL, ",");

    strcpy(unidade->procedimento, token);
    token = strtok(NULL, ",");

    strcpy(unidade->ramo_justica, token);
    token = strtok(NULL, ",");

    strcpy(unidade->sigla_grau, token);
    token = strtok(NULL, ",");

    strcpy(unidade->uf_oj, token);
    token = strtok(NULL, ",");

    strcpy(unidade->municipio_oj, token);
    token = strtok(NULL, ",");

    unidade->id_ultimo_oj = atof(token);
    token = strtok(NULL, ",");

    strcpy(unidade->nome, token);
    token = strtok(NULL, ",");

    strcpy(unidade->mesano_cnm1, token);
    token = strtok(NULL, ",");

    strcpy(unidade->mesano_sent, token);
    token = strtok(NULL, ",");

    unidade->casos_novos_2026 = atof(token);
    token = strtok(NULL, ",");

    unidade->julgados_2026 = atof(token);
    token = strtok(NULL, ",");

    unidade->prim_sent2026 = atof(token);
    token = strtok(NULL, ",");

    unidade->suspensos_2026 = atof(token);
    token = strtok(NULL, ",");

    unidade->dessobrestados_2026 = atof(token);
    token = strtok(NULL, ",");

    unidade->cumprimento_meta1 = atof(token);
    token = strtok(NULL, ",");

    unidade->distm2_a = atof(token);
    token = strtok(NULL, ",");

    unidade->julgm2_a = atof(token);
    token = strtok(NULL, ",");

    unidade->suspm2_a = atof(token);
    token = strtok(NULL, ",");

    unidade->cumprimento_meta2a = atof(token);
    token = strtok(NULL, ",");

    unidade->distm2_ant = atof(token);
    token = strtok(NULL, ",");

    unidade->julgm2_ant = atof(token);
    token = strtok(NULL, ",");

    unidade->suspm2_ant = atof(token);
    token = strtok(NULL, ",");

    unidade->desom2_ant = atof(token);
    token = strtok(NULL, ",");

    unidade->cumprimento_meta2ant = atof(token);
    token = strtok(NULL, ",");

    unidade->distm4_a = atof(token);
    token = strtok(NULL, ",");

    unidade->julgm4_a = atof(token);
    token = strtok(NULL, ",");

    unidade->suspm4_a = atof(token);
    token = strtok(NULL, ",");

    unidade->cumprimento_meta4a = atof(token);
    token = strtok(NULL, ",");

    unidade->distm4_b = atof(token);
    token = strtok(NULL, ",");

    unidade->julgm4_b = atof(token);
    token = strtok(NULL, ",");

    unidade->suspm4_b = atof(token);
    token = strtok(NULL, ",");

    unidade->cumprimento_meta4b = atof(token);

    return 1;
}

int cmd_search(int argc, char **argv){
    if(argc < 4){
        return ERRO;
    }
    ARQUIVO* arquivoBusca = abrirArquivo(argv[1], "r");
    ARQUIVO* arquivoSaida = abrirArquivo(argv[3], "w");
    excluirArquivo(arquivoSaida);
    char linha[TamanhoLinha];
    if (fgets(linha, sizeof(linha), arquivoBusca->fileArquivo) == NULL) {
        return ERRO;
    }

    Unidade_Struct* unidade = malloc(sizeof(Unidade_Struct));
    printUnidade(unidade);


    lerStruct(arquivoBusca, unidade);

    free(unidade);
    fecharArquivo(arquivoBusca);
    return 0;
}