# Projeto Acadêmico em C++

## 📚 Descrição

Este é um projeto acadêmico desenvolvido em C++, que implementa as etapas de análise léxica, sintática e semântica de um compilador simples.

## ✅ Pré-requisitos

- Sistema operacional: Linux
- Compilador: GCC (g++)

## ⚙️ Instalação do GCC no Linux

No terminal, execute o seguinte comando para instalar o GCC (caso ainda não tenha):

### Para distribuições baseadas em Debian/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential
```

## ⚙️ Build do projeto

Navegue até o diretório raiz do projeto e execute o comando abaixo:

```bash
g++ core/lexical.cpp core/parser.cpp core/semantic.cpp main.cpp -o main
```

## 🚀 Como executar o projeto

```bash
./main
```