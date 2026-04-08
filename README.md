# consulta-TRE

Ferramenta em linha de comando para manipulação e consulta de dados do Tribunal Regional Eleitoral (TRE) a partir de arquivos `.csv`.

## Sobre o projeto

O **consulta-TRE** permite realizar operações como:

- Busca de dados por município
- Concatenação de múltiplos arquivos CSV
- Exportação de resultados para novos arquivos
- Processamento em lote de diretórios

---

## Compilação

```bash
gcc src/main.c src/comando.c -o tre
```

---


## Comandos disponíveis

### Buscar por município

```bash
./tre -b dados.csv -m ARAPIRACA
```

**Parâmetros:**
- `-b <arquivo>` → arquivo CSV de entrada  
- `-m <municipio>` → nome do município para busca  

---

### Concatenar arquivos CSV

```bash
./tre -c dados1.csv dados2.csv ...
```

---

### Concatenar e salvar com nome personalizado

```bash
./tre -c dados1.csv dados2.csv -o saida.csv
```

**Parâmetros adicionais:**
- `-o <arquivo>` → define o arquivo de saída  

---

### concatenar diretório

```bash
./tre -c -p dados
```

**Parâmetros:**
- `-p <pasta>` → diretório contendo arquivos `.csv`  

---

### Ajuda

```bash
./tre -h
```

---

### Versão

```bash
./tre -v
```

---

## Exemplos completos

```bash
# Buscar município
./tre -b dados.csv -m ARAPIRACA

# Concatenar arquivos
./tre -c dados1.csv dados2.csv

# Concatenar e exportar
./tre -c dados1.csv dados2.csv -o resultado.csv

# Processar pasta inteira
./tre -c -p ./dados

# Ver ajuda
./tre -h

# Ver versão
./tre -v
```

---

