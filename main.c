#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define QTD_PALAVRAS 11
#define TAM_TS 211

// Estados
typedef enum {
    q0, qIDENTIFICADOR, qNUMERO_INTEIRO, qPONTO_DECIMAL, qNUM_REAL, qINICIO_EXPOENTE, 
    qSINAL_EXPOENTE, qNUMERO_EXPOENTE, qDOIS_PONTOS, qMENOR, qMAIOR, qCOMENTARIO,
    ATRIBUICAO, MENOR_IGUAL, DIFERENTE, MAIOR_IGUAL,
    DEAD
} State;

// Tipo do caractere
typedef enum {
    LETRA, DIGITO, ESPACO, OPERADOR, DELIMITADOR, MENOR, 
    MAIOR, DOIS_PONTOS, IGUAL, LBRACE, RBRACE, INDEFINIDO
} CharType;

// Tipo do token
typedef enum {
    KW_PROGRAM, KW_VAR, KW_INTEGER, KW_REAL,
    KW_BEGIN, KW_END, KW_IF, KW_THEN,
    KW_ELSE, KW_WHILE, KW_DO,
    ID, NUM_INT, NUM_REAL,
    OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE,
    OP_AD, OP_MIN, OP_MUL, OP_DIV, OP_ASS,
    SMB_SEM, SMB_COM, SMB_OPA, SMB_CPA, SMB_COL, SMB_DOT
} TokenType;

// struct da lista encadeada de símbolos
typedef struct Simbolo {
    char lexema[100];
    char tipo[20];
    struct Simbolo *prox;
} Simbolo;

// criacao da tabela com o tamanho de 211 (numero primo pra diminuir numero de colisoes)
Simbolo* tabela[TAM_TS];

// funcao do hash
int hash(char *str) {
    int h = 0;
    for (int i = 0; str[i]; i++) {
        h = (h * 31 + tolower(str[i])) % TAM_TS;
    }
    return h;
}

// pega o tipo do caractere
CharType getCharType(char c) {

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return LETRA;
    if (c >= '0' && c <= '9') return DIGITO;
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') return ESPACO; 
    if (c == '+' || c == '-' || c == '*' || c == '/') return OPERADOR;
    if (c == '.' || c == ';' || c == ',' || c == '(' || c == ')') return DELIMITADOR;
    if (c == '<') return MENOR;
    if (c == '>') return MAIOR;
    if (c == ':') return DOIS_PONTOS;
    if (c == '=') return IGUAL;
    if (c == '{') return LBRACE;
    if (c == '}') return RBRACE;

    return INDEFINIDO;
}

// funcao das transicoes
State verificador(State s, char c, CharType tipo) {

    switch (s) {

        case q0:
            if (tipo == ESPACO) return q0;
            if (tipo == LETRA) return qIDENTIFICADOR;
            if (tipo == DIGITO) return qNUMERO_INTEIRO;
            if (tipo == DOIS_PONTOS) return qDOIS_PONTOS;
            if (tipo == MENOR) return qMENOR;
            if (tipo == MAIOR) return qMAIOR;
            if (tipo == LBRACE) return qCOMENTARIO;
            if (tipo == DELIMITADOR) return q0;
            return DEAD;

        case qIDENTIFICADOR:
            if (tipo == LETRA || tipo == DIGITO) return qIDENTIFICADOR;
            return DEAD;

        case qNUMERO_INTEIRO:
            if (tipo == DIGITO) return qNUMERO_INTEIRO;
            if (c == '.') return qPONTO_DECIMAL;
            return DEAD;

        case qPONTO_DECIMAL:
            if (tipo == DIGITO) return qNUM_REAL;
            return DEAD;

        case qNUM_REAL:
            if (tipo == DIGITO) return qNUM_REAL;
            if (c == 'E' || c == 'e') return qINICIO_EXPOENTE;
            return DEAD;

        case qINICIO_EXPOENTE:
            if (c == '+' || c == '-') return qSINAL_EXPOENTE;
            if (tipo == DIGITO) return qNUMERO_EXPOENTE;
            return DEAD;

        case qSINAL_EXPOENTE:
            if (tipo == DIGITO) return qNUMERO_EXPOENTE;
            return DEAD;

        case qNUMERO_EXPOENTE:
            if (tipo == DIGITO) return qNUMERO_EXPOENTE;
            return DEAD;

        case qDOIS_PONTOS:
            if (tipo == IGUAL) return ATRIBUICAO;
            return DEAD;

        case qMENOR:
            if (tipo == IGUAL) return MENOR_IGUAL;
            if (tipo == MAIOR) return DIFERENTE;
            return DEAD;

        case qMAIOR:
            if (tipo == IGUAL) return MAIOR_IGUAL;
            return DEAD;

        case qCOMENTARIO:
            if (tipo == RBRACE) return q0;
            return qCOMENTARIO;

        default:
            return DEAD;
    }
}

// retorna o token em string
const char* tokenToString(TokenType t) {
    switch (t) {
        case KW_PROGRAM: return "KW_PROGRAM";
        case KW_VAR: return "KW_VAR";
        case KW_INTEGER: return "KW_INTEGER";
        case KW_REAL: return "KW_REAL";
        case KW_BEGIN: return "KW_BEGIN";
        case KW_END: return "KW_END";
        case KW_IF: return "KW_IF";
        case KW_THEN: return "KW_THEN";
        case KW_ELSE: return "KW_ELSE";
        case KW_WHILE: return "KW_WHILE";
        case KW_DO: return "KW_DO";

        case ID: return "ID";
        case NUM_INT: return "NUM_INT";
        case NUM_REAL: return "NUM_REAL";

        case OP_AD: return "OP_AD";
        case OP_MIN: return "OP_MIN";
        case OP_MUL: return "OP_MUL";
        case OP_DIV: return "OP_DIV";
        case OP_ASS: return "OP_ASS";
        case OP_LT: return "OP_LT";
        case OP_LE: return "OP_LE";
        case OP_GT: return "OP_GT";
        case OP_GE: return "OP_GE";
        case OP_NE: return "OP_NE";
        case OP_EQ: return "OP_EQ";

        case SMB_SEM: return "SMB_SEM";
        case SMB_COM: return "SMB_COM";
        case SMB_OPA: return "SMB_OPA";
        case SMB_CPA: return "SMB_CPA";
        case SMB_COL: return "SMB_COL";
        case SMB_DOT: return "SMB_DOT";
    }
    return "UNKNOWN";
}

// transforma string para minusculo, evitando case sensitive na hora de buscar por simbolo
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// funcao de busca por simbolo
Simbolo* buscarSimbolo(char *lexema) {

    int idx = hash(lexema);
    Simbolo *atual = tabela[idx];

    while (atual != NULL) {
        if (strcmp(atual->lexema, lexema) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL;
}

// funcao de inserir um simbolo
void inserirSimbolo(char *lexema, const char *tipo) {

    if (buscarSimbolo(lexema) != NULL)
        return;

    int idx = hash(lexema);

    Simbolo *novo = (Simbolo*) malloc(sizeof(Simbolo));

    strcpy(novo->lexema, lexema);
    strcpy(novo->tipo, tipo);

    novo->prox = tabela[idx];
    tabela[idx] = novo;
}

// converte o token na keyword respectiva
TokenType getKeywordToken(char *lexema) {

    if (strcmp(lexema, "program") == 0) return KW_PROGRAM;
    if (strcmp(lexema, "var") == 0) return KW_VAR;
    if (strcmp(lexema, "integer") == 0) return KW_INTEGER;
    if (strcmp(lexema, "real") == 0) return KW_REAL;
    if (strcmp(lexema, "begin") == 0) return KW_BEGIN;
    if (strcmp(lexema, "end") == 0) return KW_END;
    if (strcmp(lexema, "if") == 0) return KW_IF;
    if (strcmp(lexema, "then") == 0) return KW_THEN;
    if (strcmp(lexema, "else") == 0) return KW_ELSE;
    if (strcmp(lexema, "while") == 0) return KW_WHILE;
    if (strcmp(lexema, "do") == 0) return KW_DO;

    return -1;
}

// funçao para escrever o token no arquivo .lex
void imprimirToken(State estAtual, char *lexema, int linha, int colInicial, FILE *arqLex) {

    // já salva em minúsculo, mas preserva o lexema original
    char temp[100];
    strcpy(temp, lexema);
    toLowerCase(temp);

    TokenType token;

    if (estAtual == qIDENTIFICADOR) {

        TokenType kw = getKeywordToken(temp);

        if (kw != -1) {
            token = kw;
        } else {
            token = ID;
            inserirSimbolo(lexema, "ID");
        }

    } else if (estAtual == qNUMERO_INTEIRO) {
        token = NUM_INT;

    } else if (estAtual == qNUM_REAL) {
        token = NUM_REAL;

    } else if (estAtual == ATRIBUICAO) {
        token = OP_ASS;

    } else if (estAtual == MENOR_IGUAL) {
        token = OP_LE;

    } else if (estAtual == MAIOR_IGUAL) {
        token = OP_GE;

    } else if (estAtual == DIFERENTE) {
        token = OP_NE;

    } else if (estAtual == qMENOR) {
        token = OP_LT;

    } else if (estAtual == qMAIOR) {
        token = OP_GT;

    } else if (estAtual == qDOIS_PONTOS) {
        token = SMB_COL; 

    } else {
        return;
    }

    fprintf(arqLex, "<%s, %s> %d %d\n",
        tokenToString(token), lexema, linha, colInicial);
}

// funcao de verificacao de estado final
bool ehEstadoFinal(State s) {
    return (
        s == qIDENTIFICADOR || s == qNUMERO_INTEIRO || s == qNUM_REAL ||
        s == qDOIS_PONTOS || s == qMENOR || s == qMAIOR ||
        s == ATRIBUICAO || s == MENOR_IGUAL || s == DIFERENTE || s == MAIOR_IGUAL
    );
}

// escreve no arquivo de tabela de simbolos, todos os simbolos da lista encadeada tabela
void imprimirTS(FILE *arqTS) {
    for (int i = 0; i < TAM_TS; i++) {

        Simbolo *atual = tabela[i];

        while (atual != NULL) {
            fprintf(arqTS, "%s -> %s\n",
                atual->lexema, atual->tipo);
            atual = atual->prox;
        }
    }
}

// tira todo o lixo de memoria da tabela de simbolos para criar uma nova
void zerarTS() {
    for (int i = 0; i < TAM_TS; i++) {
        tabela[i] = NULL;
    }
}

// cria uma nova tabela de simbolos com valores pré estabelecidos de palavras chave
void inicializarTS() {
    const char* palavras[] = {
        "program", "var", "integer", "real",
        "begin", "end", "if", "then",
        "else", "while", "do"
    };
    for (int i = 0; i < QTD_PALAVRAS; i++) {
        TokenType tk = getKeywordToken((char*)palavras[i]);
        inserirSimbolo((char*)palavras[i], (char*)tokenToString(tk));
    }
}

void liberarTS() {
    for (int i = 0; i < TAM_TS; i++) {
        Simbolo *atual = tabela[i];
        while (atual) {
            Simbolo *tmp = atual;
            atual = atual->prox;
            free(tmp);
        }
    }
}

// MAIN
int main() {

    zerarTS();
    inicializarTS();

    // ler arquivo pascal
    FILE *arquivo = fopen("program.pas", "r");

    // escrever nos arquivos de lex tabela de simbolos e erros
    FILE *arqLex = fopen("saida.lex", "w");
    FILE *arqTS  = fopen("saida.tabela", "w");
    FILE *arqErr = fopen("saida.err", "w");

    if (!arqLex || !arqTS || !arqErr) {
        printf("Erro ao criar arquivos de saída!\n");
        return 1;
    }

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }

    char c;
    int linha = 1, coluna = 0, colInicial = 0;

    State estAtual = q0;

    char lexema[100];
    int pos = 0;

    // buffer para otimizacao da leitura de cada carectere
    char linhaBuffer[256];

    // loop while que pega um caráctere por vez
    while (fgets(linhaBuffer, sizeof(linhaBuffer), arquivo)) {

        for (int i = 0; linhaBuffer[i] != '\0'; i++){
            
            char c = linhaBuffer[i];
            coluna++;
            CharType tipo = getCharType(c);

            // pega o valor da coluna do inicio da palavra
            if (pos == 0 && tipo != ESPACO) {
                colInicial = coluna;
            }

            // se for comentario ignora para nao precisar verificar todo o texto dentro
            if (estAtual == qCOMENTARIO) {
                if (c == '}') {
                    estAtual = q0;
                }
                continue;
            }

            // se for espaço ignora
            if (tipo == ESPACO) {

                if (ehEstadoFinal(estAtual)) {
                    imprimirToken(estAtual, lexema, linha, colInicial, arqLex);

                    estAtual = q0;
                    pos = 0;
                    lexema[0] = '\0';
                }

                continue;
            }

            // ponto após número (pode ser real ou erro)
            if (c == '.' && estAtual == qNUMERO_INTEIRO) {

                State proximo = verificador(estAtual, c, tipo);

                estAtual = proximo;
                lexema[pos++] = c;
                lexema[pos] = '\0';

                continue;
            }

            // se for delimitador
            if (tipo == DELIMITADOR) {

                if (estAtual == qPONTO_DECIMAL) {

                    fprintf(arqErr, "Erro lexico: numero real mal formado '%s' linha %d coluna %d\n",
                        lexema, linha, colInicial);

                    fprintf(arqLex, "<NUM_REAL_MALFORMADO, %s> %d %d\n",
                        lexema, linha, colInicial);

                }
                else if (ehEstadoFinal(estAtual)) {
                    imprimirToken(estAtual, lexema, linha, colInicial, arqLex);
                }

                TokenType token;

                if (c == ';') token = SMB_SEM;
                else if (c == ',') token = SMB_COM;
                else if (c == '(') token = SMB_OPA;
                else if (c == ')') token = SMB_CPA;
                else if (c == '.') token = SMB_DOT;
                else {
                    fprintf(arqErr, "Erro lexico: delimitador desconhecido '%c'\n", c);
                    continue;
                }

                fprintf(arqLex, "<%s, %c> %d %d\n",
                    tokenToString(token), c, linha, coluna);

                estAtual = q0;
                pos = 0;
                lexema[0] = '\0';

                continue;
            }

            if (tipo == IGUAL && estAtual != qDOIS_PONTOS) {
                fprintf(arqLex, "<%s, %c> %d %d\n",
                    tokenToString(OP_EQ), c, linha, coluna);
                continue;
            }

            // se for operador
            if (tipo == OPERADOR) {

                if (ehEstadoFinal(estAtual)) {
                    imprimirToken(estAtual, lexema, linha, colInicial, arqLex);
                }

                TokenType token;

                if (c == '+') token = OP_AD;
                else if (c == '-') token = OP_MIN;
                else if (c == '*') token = OP_MUL;
                else token = OP_DIV;

                fprintf(arqLex, "<%s, %c> %d %d\n",
                    tokenToString(token), c, linha, coluna);

                estAtual = q0;
                pos = 0;
                lexema[0] = '\0';

                continue;
            }

            State proximo = verificador(estAtual, c, tipo);

            if (proximo == DEAD) {

                if (estAtual == qPONTO_DECIMAL) {

                    fprintf(arqErr, "Erro lexico: numero real mal formado '%s' linha %d coluna %d\n",
                            lexema, linha, colInicial);

                    // 🔥 imprime o número mesmo com erro
                    fprintf(arqLex, "<NUM_REAL_MALFORMADO, %s> %d %d\n",
                            lexema, linha, colInicial);

                    estAtual = q0;
                    pos = 0;
                    lexema[0] = '\0';

                    i--;
                    coluna--;

                    continue;
                }

                if (ehEstadoFinal(estAtual)) {

                    imprimirToken(estAtual, lexema, linha, colInicial, arqLex);

                    estAtual = q0;
                    pos = 0;

                    // só volta o caractere se ele NÃO for espaço
                    if (getCharType(c) != ESPACO) {
                        i--;
                        coluna--;
                    }
                } else {

                    fprintf(arqErr, "Erro lexico: '%c' linha %d coluna %d\n", c, linha, coluna);

                    estAtual = q0;
                    pos = 0;
                    lexema[0] = '\0';
                }

            } else {
                estAtual = proximo;

                lexema[pos++] = c;
                lexema[pos] = '\0';
            }
        }
        
        linha++;
        coluna = 0;
    }

    if (estAtual == qCOMENTARIO) {
        fprintf(arqErr, "Erro lexico: comentario nao fechado na linha %d\n", linha);
    }

    imprimirTS(arqTS);

    fclose(arquivo);
    fclose(arqLex);
    fclose(arqTS);
    fclose(arqErr);

    liberarTS();

    return 0;
}