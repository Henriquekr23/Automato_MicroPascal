#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    q0, qID, qNUM_INT, qCOLON, qLT, qGT,
    qCOMMENT, qNUM_REAL_DOT, qNUM_REAL,
    OP_ASS, OP_LE, OP_NE, OP_GE, DEAD
} State;

typedef enum {
    LETRA, NUMERO, ESPACO, OPERADOR, DELIMITADOR,
    MENOR, MAIOR, DOIS_PONTOS, IGUAL,
    LBRACE, RBRACE, INDEFINIDO
} CharType;

// CLASSIFICAÇÃO DE CARACTERE
CharType getCharType(char c) {

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return LETRA;

    if (c >= '0' && c <= '9')
        return NUMERO;

    if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        return ESPACO;

    if (c == '+' || c == '-' || c == '*' || c == '/')
        return OPERADOR;

    if (c == ';' || c == ',' || c == '(' || c == ')')
        return DELIMITADOR;

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
            if (tipo == NUMERO) return qNUM_INT;
            if (tipo == DOIS_PONTOS) return qCOLON;
            if (tipo == MENOR) return qLT;
            if (tipo == MAIOR) return qGT;
            if (tipo == LBRACE) return qCOMMENT;
            return DEAD;

        case qID:
            if (tipo == LETRA || tipo == NUMERO) return qID;
            return DEAD;

        case qNUM_INT:
            if (tipo == NUMERO) return qNUM_INT;
            if (c == '.') return qNUM_REAL_DOT;
            return DEAD;

        case qNUM_REAL_DOT:
            if (tipo == NUMERO) return qNUM_REAL;
            return DEAD;

        case qNUM_REAL:
            if (tipo == NUMERO) return qNUM_REAL;
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

    char palavra[1024];
    State estAtual = q0;

    printf("Digite a palavra:\n");
    scanf("%1023s", palavra);

    for (int i = 0; palavra[i] != '\0'; i++) {
        estAtual = verificador(estAtual, palavra[i]);

        if (estAtual == DEAD) break;
    }

    if (ehEstadoFinal(estAtual)) {
        printf("Palavra aceita! Token: %s\n", nomeEstado(estAtual));
    } else {
        printf("Palavra rejeitada.\n");
    }

    return 0;
}