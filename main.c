#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    q0, qID, qNUM_INT, qCOLON, qLT, qGT, qCOMMENT, qNUM_REAL_DOT, qNUM_REAL, OP_ASS, OP_LE, OP_NE, OP_GE, DEAD
} State;

typedef enum {
    LETRA, DIGITO, ESPACO, OPERADOR, DELIMITADOR, MENOR, MAIOR, DOIS_PONTOS, IGUAL, LBRACE, RBRACE, INDEFINIDO
} CharType;

// CLASSIFICAÇÃO DE CARACTERE
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
            if (tipo == LETRA) return qID;
            if (tipo == DIGITO) return qNUM_INT;
            if (tipo == DOIS_PONTOS) return qCOLON;
            if (tipo == MENOR) return qLT;
            if (tipo == MAIOR) return qGT;
            if (tipo == LBRACE) return qCOMMENT;
            if (tipo == DELIMITADOR) return q0;
            return DEAD;

        case qID:
            if (tipo == LETRA || tipo == DIGITO) return qID;
            return DEAD;

        case qNUM_INT:
            if (tipo == DIGITO) return qNUM_INT;
            if (c == '.') return qNUM_REAL_DOT;
            return DEAD;

        case qNUM_REAL_DOT:
            if (tipo == DIGITO) return qNUM_REAL;
            return DEAD;

        case qNUM_REAL:
            if (tipo == DIGITO) return qNUM_REAL;
            return DEAD;

        case qCOLON:
            if (tipo == IGUAL) return OP_ASS;
            return DEAD;

        case qLT:
            if (tipo == IGUAL) return OP_LE;
            if (tipo == MAIOR) return OP_NE;
            return DEAD;

        case qGT:
            if (tipo == IGUAL) return OP_GE;
            return DEAD;

        case qCOMMENT:
            if (tipo == RBRACE) return q0;
            return qCOMMENT;

        default:
            return DEAD;
    }
}

// ESTADOS FINAIS
bool ehEstadoFinal(State s) {
    return (s == qID || s == qNUM_INT || s == qNUM_REAL ||
        s == qCOLON || s == qLT || s == qGT ||
        s == OP_ASS || s == OP_LE || s == OP_NE || s == OP_GE);
}

// NOME DO ESTADO
const char* nomeEstado(State s) {
    switch(s) {
        case qID: return "ID";
        case qNUM_INT: return "NUM_INT";
        case qNUM_REAL: return "NUM_REAL";
        case OP_ASS: return "OP_ASS";
        case OP_LE: return "OP_LE";
        case OP_NE: return "OP_NE";
        case OP_GE: return "OP_GE";
        case qCOLON: return "COLON";
        case qLT: return "LT";
        case qGT: return "GT";
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

    CharType tipo = getCharType(c);

    // TRATAR DELIMITADORES DIRETAMENTE
    if (tipo == DELIMITADOR) {
        printf("<DELIM, %c> %d %d\n", c, linha, coluna);
    }

    // TRATAR OPERADORES SIMPLES
    if (tipo == OPERADOR) {
        printf("<OP, %c> %d %d\n", c, linha, coluna);
    }

    while ((c = fgetc(arquivo)) != EOF) {

        coluna++;

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

//oi