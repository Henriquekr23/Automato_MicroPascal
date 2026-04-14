# Analisador Léxico para MicroPascal

### Descrição

Este projeto implementa um analisador léxico para uma linguagem baseada em MicroPascal. O analisador é responsável por ler um arquivo fonte .pas, identificar os lexemas presentes e classificá-los em tokens, além de construir uma tabela de símbolos e registrar possíveis erros léxicos.

A implementação foi feita em linguagem C, utilizando um Autômato Finito Determinístico (AFD) para reconhecimento dos padrões da linguagem.

### Objetivos
	•	Implementar um analisador léxico funcional
	•	Aplicar conceitos de autômatos finitos
	•	Identificar tokens válidos da linguagem
	•	Construir uma tabela de símbolos
	•	Detectar e reportar erros léxicos

### Estrutura do Projeto

O projeto é composto pelos seguintes arquivos principais:

	•	main.c — implementação do analisador léxico
	•	program.pas — arquivo de entrada (código fonte em Pascal)
	•	saida.lex — arquivo contendo os tokens reconhecidos
	•	saida.tabela — tabela de símbolos gerada
	•	saida.err — arquivo com erros léxicos encontrados
	•	AFD_MicroPascal.dot — representação gráfica do autômato

### Funcionamento do Analisador

O analisador lê o arquivo de entrada caractere por caractere e utiliza um AFD para determinar a qual token cada sequência pertence.

Durante a execução, o sistema:

	1.	Identifica o tipo de cada caractere (letra, dígito, operador, etc.)
	2.	Realiza transições entre estados do autômato
	3.	Forma lexemas válidos
	4.	Classifica os lexemas em tokens
	5.	Armazena identificadores na tabela de símbolos
	6.	Registra erros léxicos quando necessário

### Tokens Reconhecidos

O analisador reconhece os seguintes tipos de tokens:

Palavras-chave:

	•	program
	•	var
	•	integer
	•	real
	•	begin
	•	end
	•	if
	•	then
	•	else
	•	while
	•	do

Identificadores:

Sequências iniciadas por letra, seguidas de letras ou dígitos.

Números:

	•	Inteiros (ex: 10, 25)
	•	Reais (ex: 3.14)
	•	Reais com expoente (ex: 2.5E10)

Observação: o expoente é reconhecido apenas após números reais.

Operadores:

	•	Aritméticos: +, -, *, /
	•	Relacionais: <, <=, >, >=, <>, =
	•	Atribuição: :=

Delimitadores:

	•	; , ( ) .

Comentários:

	•	Delimitados por { e }
	•	São ignorados pelo analisador (não geram tokens)

### Tabela de Símbolos

A tabela de símbolos armazena:

	•	Identificadores encontrados no código
	•	Palavras-chave previamente carregadas

A estrutura utilizada é uma tabela hash com encadeamento para resolução de colisões.

### Tratamento de Erros

O analisador identifica:

	•	Caracteres inválidos
	•	Sequências mal formadas
	•	Comentários não fechados

Os erros são registrados no arquivo saida.err, indicando linha e coluna.

### Controle de Posição

Cada token reconhecido contém:

	•	Linha
	•	Coluna inicial

A coluna inicial é controlada no momento em que o lexema começa a ser formado, garantindo precisão na localização.

### Autômato Finito (AFD)

O reconhecimento dos tokens é baseado em um AFD implementado na função verificador.

Estados principais:

	•	q0 — estado inicial
	•	qIDENTIFICADOR
	•	qNUMERO_INTEIRO
	•	qNUM_REAL
	•	qINICIO_EXPOENTE
	•	qSINAL_EXPOENTE
	•	qNUMERO_EXPOENTE
	•	qDOIS_PONTOS
	•	qMENOR / qMAIOR
	•	qCOMENTARIO

O arquivo .dot representa graficamente esse autômato e pode ser visualizado com ferramentas como Graphviz.

### Considerações Finais

Este projeto demonstra, na prática, a aplicação de conceitos fundamentais de compiladores, como análise léxica e autômatos finitos. A implementação buscou ser fiel aos requisitos propostos, mantendo organização, clareza e eficiência.