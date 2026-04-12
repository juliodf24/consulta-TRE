# Consulta TRE - Ferramenta de Linha de Comando

## Descrição

Este projeto foi desenvolvido como trabalho da disciplina de Estrutura
de Dados.\
Consiste em uma ferramenta de linha de comando para **manipulação e
consulta de dados dos Tribunais Regionais Eleitorais (TRE)** do Brasil,
armazenados em arquivos CSV.

A aplicação permite realizar buscas, consolidação de arquivos e geração
de métricas a partir dos dados.

------------------------------------------------------------------------

## Funcionalidades

-   **Busca por município**\
    Permite consultar registros específicos a partir do nome de um
    município.

-   **Concatenação de arquivos CSV**\
    Junta múltiplos arquivos CSV em um único arquivo consolidado.

-   **Geração de resumos e métricas**\
    Calcula indicadores de desempenho com base nos dados fornecidos.

------------------------------------------------------------------------


## Compilação

``` bash
gcc src/main.c src/consulta.c -o tre.exe
```

------------------------------------------------------------------------

## Execução

### Ajuda

``` bash
./tre.exe -h
```

### Versão

``` bash
./tre.exe -v
```

### Busca por Município

``` bash
./tre.exe -b <arquivo_entrada> -m <nome_municipio>
```

**Exemplos:**

``` bash
./tre.exe -b dados.csv -m ARAPIRACA
./tre.exe -b dados.csv -m "BELA VISTA"
```

------------------------------------------------------------------------

### Concatenar Arquivos

``` bash
./tre.exe -c dados1.csv dados2.csv -o meuarquivo.csv
```

------------------------------------------------------------------------

### Resumo com Métricas

``` bash
./tre.exe -r <arquivo_entrada>
```

------------------------------------------------------------------------

## Observações

-   Linhas podem ser ignoradas em caso de erro no CSV
-   Problemas comuns:
    -   Número de colunas incorreto
    -   Dados inválidos

------------------------------------------------------------------------