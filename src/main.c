#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmd_help(int argc, char **argv){
    printf("ajuda\n");
};
int cmd_buscar(int argc, char **argv){
    printf("buscar\n");
};
int cmd_versao(int argc, char **argv){
    printf("versao\n");
};


typedef struct {
	char *cmd; //string do comando Ex: "-h"
    // armazena um ponteiro para uma função Ex: cmd_help
	int (*fn)(int, char **); // int para receber o argc e char** para receber o endereco de argv
} comando_struct;

// O static indica que seu valor é persistente e que ele só é acessível dentro deste arquivo
static comando_struct comandos[] = {
    {"-h", cmd_help},
    {"-b", cmd_buscar},
    {"-v", cmd_versao}
}; // cadastra os comandos na lista de comandos

static comando_struct *encontrarComando(char *texto){
    for (size_t i = 0; i < (sizeof(comandos)/sizeof(comandos[0])); i++){
        comando_struct *comando = &comandos[i];
        if(strcmp(texto, comando->cmd) == 0){
            return comando;
        }
    } return NULL;
} // recebe uma string e retorna o comando no formato comando_struct caso exista, se não retorna null

int main(int argc, char *argv[]){

    char *cmd; 
    
    if (argc < 2) {
        cmd = "-h";
    } else {
        cmd = argv[1];
    }

    comando_struct *comando = encontrarComando(cmd);

    if(comando){
        comando->fn(argc--, argv++);
    } else{
        printf("Comando não encontrado!\n");
        encontrarComando("-h")->fn(argc--, argv++);
        exit(0);
    }

    return 0;
}
