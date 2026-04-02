#ifndef CONSULTA_H
#define CONSULTA_H

typedef struct {
	char *cmd; //string do comando Ex: "-h"
    // armazena um ponteiro para uma função Ex: cmd_help
	int (*fn)(int, char **); // int para receber o argc e char** para receber o endereco de argv
} comando_struct;

typedef struct{
	char* sigla_tribunal;
	char* procedimento;
	char* ramo_justica;
	char* sigla_grau;
	char* uf_oj;
	char* municipio_oj;
	char* id_ultimo_oj;
	char* nome;
	char* mesano_cnm1;
	char* mesano_sent;
	char* casos_novos_2026;
	char* julgados_2026;
	char* prim_sent2026;
	char* suspensos_2026;
	char* dessobrestados_2026;
	char* cumprimento_meta1;
	char* distm2_a;
	char* julgm2_a;
	char* suspm2_a;
	char* cumprimento_meta2a;
	char* distm2_ant;
	char* julgm2_ant;
	char* suspm2_ant;
	char* desom2_ant;
	char* cumprimento_meta2ant;
	char* distm4_a;
	char* julgm4_a;
	char* suspm4_a;
	char* cumprimento_meta4a;
	char* distm4_b;
	char* julgm4_b;
	char* suspm4_b;
	char* cumprimento_meta4b;
}UnidadeJurisdiciona_Struct;


int cmd_help(int argc, char **argv);
int cmd_buscar(int argc, char **argv);
int cmd_versao(int argc, char **argv);
int cmd_concatenar(int argc, char **argv);

#endif
