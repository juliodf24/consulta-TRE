#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consulta.h"

// Lista de comandos
// O static indica que seu valor é persistente e que ele só é acessível dentro deste arquivo
static comando_struct comandos[] = {
    {"-h", cmd_help},
    {"-c", cmd_concatenar},
    {"-b", cmd_buscar},
    {"-v", cmd_versao}
}; // cadastra os comandos na lista de comandos

// recebe uma string e retorna o comando no formato comando_struct caso exista, se não retorna null
static comando_struct *encontrarComando(char *texto){
    for (size_t i = 0; i < (sizeof(comandos)/sizeof(comandos[0])); i++){
        comando_struct *comando = &comandos[i];
        if(strcmp(texto, comando->cmd) == 0){
            return comando;
        }
    } return NULL;
} 

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
