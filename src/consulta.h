#ifndef CONSULTA_H
#define CONSULTA_H
#define ERRO -1

typedef struct {
	char *cmd; //string do comando Ex: "-h"
    // armazena um ponteiro para uma função Ex: cmd_help
	int (*fn)(int, char **); // int para receber o argc e char** para receber o endereco de argv
} comando_struct;

typedef struct {
    char sigla_tribunal[50];
    char procedimento[255];
    char ramo_justica[255];
    char sigla_grau[255];
    char uf_oj[4];
    char municipio_oj[255];
    int id_ultimo_oj;
    char nome[255];
    char mesano_cnm1[255];
    char mesano_sent[255];

    int casos_novos_2026;
    int julgados_2026;
    int prim_sent2026;
    int suspensos_2026;
    int dessobrestados_2026;
    int cumprimento_meta1;
    int distm2_a;
    int julgm2_a;
    int suspm2_a;
    int cumprimento_meta2a;
    int distm2_ant;
    int julgm2_ant;
    int suspm2_ant;
    int desom2_ant;
    int cumprimento_meta2ant;
    int distm4_a;
    int julgm4_a;
    int suspm4_a;
    int cumprimento_meta4a;
    int distm4_b;
    int julgm4_b;
    int suspm4_b;
    int cumprimento_meta4b;

} UnidadeJurisdiciona_Struct;


int cmd_help(int argc, char **argv);
int cmd_buscar(int argc, char **argv);
int cmd_versao(int argc, char **argv);
int cmd_concatenar(int argc, char **argv);

#endif
