#ifndef CONSULTA_H
#define CONSULTA_H
#define ERRO -1
#define LimiteNomeArquivo 300
#define TamanhoLinha 2050

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

    double casos_novos_2026;
    double julgados_2026;
    double prim_sent2026;
    double suspensos_2026;
    double dessobrestados_2026;
    double cumprimento_meta1;
    double distm2_a;
    double julgm2_a;
    double suspm2_a;
    double cumprimento_meta2a;
    double distm2_ant;
    double julgm2_ant;
    double suspm2_ant;
    double desom2_ant;
    double cumprimento_meta2ant;
    double distm4_a;
    double julgm4_a;
    double suspm4_a;
    double cumprimento_meta4a;
    double distm4_b;
    double julgm4_b;
    double suspm4_b;
    double cumprimento_meta4b;

} UnidadeJurisdiciona_Struct;


int cmd_help(int argc, char **argv);
int cmd_buscar(int argc, char **argv);
int cmd_versao(int argc, char **argv);
int cmd_concatenar(int argc, char **argv);
int cmd_resumir(int argc, char **argv);

typedef struct{
    FILE *fileArquivo;
    char nomeArquivo[LimiteNomeArquivo+1];
} ARQUIVO;

// caso o arquivo não extista ele sera criado
ARQUIVO* abrirArquivo(char* nome, char* tipoAbertura);
int fecharArquivo(ARQUIVO *arquivo);
int excluirArquivo(ARQUIVO *arquivo);

typedef struct {
    UnidadeJurisdiciona_Struct *dados;
    int tamanho;
} ListaUnidades;

ListaUnidades* criarListaUnidade();

void adicionarNaListaUnidade(ListaUnidades *lista, UnidadeJurisdiciona_Struct* unidade);
int buscarUnidade(ListaUnidades *lista, char *sigla);

#endif
