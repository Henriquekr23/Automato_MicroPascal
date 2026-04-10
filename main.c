#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    q0, qIDENTIFICADOR, qSINAL, qNUMERO_INTEIRO, qPONTO_DECIMAL, qNUM_REAL, qINICIO_EXPOENTE, 
    qSINAL_EXPOENTE, qNUMERO_EXPOENTE, qDOIS_PONTOS, qMENOR, qMAIOR, qCOMENTARIO,
    ATRIBUICAO, MENOR_IGUAL, DIFERENTE, MAIOR_IGUAL,
    DEAD
} State;

typedef enum {
    LETRA, DIGITO, ESPACO, OPERADOR, DELIMITADOR, MENOR, MAIOR, DOIS_PONTOS, IGUAL, LBRACE, RBRACE, INDEFINIDO
} CharType;

// CLASSIFICAÇÃO DE CARACTERE (Alfabeto)
CharType getCharType(char c) {

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return LETRA;
    if (c >= '0' && c <= '9') return DIGITO;
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') return ESPACO; 
    if (c == '+' || c == '-' || c == '*' || c == '/') return OPERADOR;
    if (c == ';' || c == ',' || c == '(' || c == ')') return DELIMITADOR;
    if (c == '<') return MENOR;
    if (c == '>') return MAIOR;
    if (c == ':') return DOIS_PONTOS;
    if (c == '=') return IGUAL;
    if (c == '{') return LBRACE;
    if (c == '}') return RBRACE;

    return INDEFINIDO;
}

// TRANSIÇÕES
State verificador(State s, char c) {

    CharType tipo = getCharType(c);

    switch (s) {

        case q0:
            if (tipo == ESPACO) return q0;
            if (tipo == LETRA) return qIDENTIFICADOR;
            if (tipo == DIGITO) return qNUMERO_INTEIRO;
            if (c == '+' || c == '-') return qSINAL;
            if (tipo == DOIS_PONTOS) return qDOIS_PONTOS;
            if (tipo == MENOR) return qMENOR;
            if (tipo == MAIOR) return qMAIOR;
            if (tipo == LBRACE) return qCOMENTARIO;
            if (tipo == DELIMITADOR) return q0;
            return DEAD;

        case qIDENTIFICADOR:
            if (tipo == LETRA || tipo == DIGITO) return qIDENTIFICADOR;
            return DEAD;

        case qSINAL:
            if (tipo == DIGITO) return qNUMERO_INTEIRO;
            return DEAD;

        case qNUMERO_INTEIRO:
            if (tipo == DIGITO) return qNUMERO_INTEIRO;
            if (c == '.') return qPONTO_DECIMAL;
            if (c == 'E' || c == 'e') return qINICIO_EXPOENTE;
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

// ESTADOS FINAIS
bool ehEstadoFinal(State s) {
    return (
        s == qIDENTIFICADOR || s == qNUMERO_INTEIRO || s == qNUM_REAL ||
        s == qNUMERO_EXPOENTE ||
        s == qDOIS_PONTOS || s == qMENOR || s == qMAIOR ||
        s == ATRIBUICAO || s == MENOR_IGUAL || s == DIFERENTE || s == MAIOR_IGUAL
    );
}

// NOME DO ESTADO
const char* nomeEstado(State s) {
    switch(s) {
        case qIDENTIFICADOR: return "IDENTIFICADOR";
        case qNUMERO_INTEIRO: return "NUMERO INTEIRO";
        case qNUM_REAL: return "NUMERO REAL";
        case ATRIBUICAO: return "ATRIBUICAO";
        case MENOR_IGUAL: return "MENOR IGUAL";
        case DIFERENTE: return "DIFERENTE";
        case MAIOR_IGUAL: return "MAIOR IGUAL";
        case qDOIS_PONTOS: return "DOIS PONTOS";
        case qMENOR: return "MENOR QUE";
        case qMAIOR: return "MAIOR QUE";
        default: return "INVALIDO";
    }
}

int main() {

    FILE *arquivo = fopen("program.pas", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }

    char c;
    int linha = 1, coluna = 0;

    State estAtual = q0;

    char lexema[100];
    int pos = 0;

    while ((c = fgetc(arquivo)) != EOF) {

        coluna++;
        CharType tipo = getCharType(c);

        // IGNORA ESPAÇO
        if (tipo == ESPACO) {

            if (ehEstadoFinal(estAtual)) {
                printf("<%s, %s> %d %d\n",
                    nomeEstado(estAtual), lexema, linha, coluna);

                estAtual = q0;
                pos = 0;
                lexema[0] = '\0';
            }

            if (c == '\n') {
                linha++;
                coluna = 0;
            }

            continue;
        }

        // DELIMITADORES
        if (tipo == DELIMITADOR) {

            if (ehEstadoFinal(estAtual)) {
                printf("<%s, %s> %d %d\n",
                    nomeEstado(estAtual), lexema, linha, coluna);
            }

            printf("<DELIM, %c> %d %d\n", c, linha, coluna);

            estAtual = q0;
            pos = 0;
            lexema[0] = '\0';

            continue;
        }

        // OPERADORES
        if (tipo == OPERADOR) {

            if (ehEstadoFinal(estAtual)) {
                printf("<%s, %s> %d %d\n",
                    nomeEstado(estAtual), lexema, linha, coluna);
            }

            printf("<OP, %c> %d %d\n", c, linha, coluna);

            estAtual = q0;
            pos = 0;
            lexema[0] = '\0';

            continue;
        }

        State proximo = verificador(estAtual, c);

        if (proximo == DEAD) {
            if (ehEstadoFinal(estAtual)) {

                printf("<%s, %s> %d %d\n",
                    nomeEstado(estAtual), lexema, linha, coluna);

                estAtual = q0;
                pos = 0;

                // só volta o caractere se ele NÃO for espaço
                if (getCharType(c) != ESPACO)
                    fseek(arquivo, -1, SEEK_CUR);

            } else {

                printf("Erro léxico: '%c' linha %d coluna %d\n", c, linha, coluna);

                estAtual = q0;
                pos = 0;
                lexema[0] = '\0';
            }
        } else {
            estAtual = proximo;

            lexema[pos++] = c;
            lexema[pos] = '\0';
        }

        if (c == '\n') {
            linha++;
            coluna = 0;
        }
    }
    fclose(arquivo);
    return 0;
}