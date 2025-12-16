# Trabalho Prático 1 — Algoritmos e Estruturas de Dados 2

Este trabalho prático foi desenvolvido no contexto da disciplina de Algoritmos e Estruturas de Dados 2 e tem como objetivo a implementação e comparação de duas estruturas clássicas para indexação e busca em grandes volumes de dados textuais: a árvore Patricia e a tabela hash.

Essas estruturas são amplamente utilizadas em aplicações que demandam acesso eficiente à informação, como sistemas de recuperação de texto, mecanismos de busca e indexação de documentos. O projeto aplica esses conceitos na construção de um índice invertido, estrutura responsável por mapear palavras aos documentos em que aparecem, permitindo consultas rápidas e organizadas.

O trabalho foi implementado em linguagem C, com foco em modularização, uso de Tipos Abstratos de Dados (TADs) e organização clara do código-fonte.

## Organização do Projeto

O código está organizado de forma modular, com separação entre arquivos de definição e implementação. As estruturas de dados, funções auxiliares e rotinas principais estão distribuídas em arquivos `.h` e `.c`, permitindo maior clareza, reutilização e manutenção do código.

O projeto utiliza um Makefile para automatizar o processo de compilação, garantindo facilidade de execução em ambientes Linux.

## Estruturas de Dados Implementadas

A árvore Patricia foi utilizada para armazenar palavras de forma compacta, explorando prefixos comuns entre strings. Sua implementação permite inserções e buscas eficientes, reduzindo comparações redundantes e otimizando o uso de memória. Cada termo armazenado está associado a uma lista de ocorrências, contendo informações sobre os documentos e posições em que aparece.

A tabela hash foi implementada utilizando tratamento de colisões por encadeamento. Uma função de hash é responsável por distribuir as palavras de forma uniforme entre os buckets da tabela, permitindo acesso rápido durante operações de inserção e busca. Em casos de colisão, listas encadeadas são utilizadas para armazenar múltiplas entradas em uma mesma posição.

Ambas as estruturas foram empregadas na construção do índice invertido e posteriormente comparadas quanto ao desempenho.

## Análise de Desempenho

Para avaliar a eficiência das estruturas, foram implementados contadores de desempenho responsáveis por medir o número de comparações realizadas durante as operações de inserção e consulta.

Essas métricas permitiram analisar o comportamento de cada estrutura em diferentes cenários, con
