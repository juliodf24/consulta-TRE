#ifndef CONSULTA_H
#define CONSULTA_H

typedef struct {
	char *cmd; //string do comando Ex: "-h"
    // armazena um ponteiro para uma função Ex: cmd_help
	int (*fn)(int, char **); // int para receber o argc e char** para receber o endereco de argv
} comando_struct;

int cmd_help(int argc, char **argv);
int cmd_buscar(int argc, char **argv);
int cmd_versao(int argc, char **argv);
int cmd_concatenar(int argc, char **argv);

// incluindo Comandos
// O static indica que seu valor é persistente e que ele só é acessível dentro deste arquivo
static comando_struct comandos[] = {
    {"-h", cmd_help},
    {"-c", cmd_concatenar},
    {"-b", cmd_buscar},
    {"-v", cmd_versao}
}; // cadastra os comandos na lista de comandos

#endif
