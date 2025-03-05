#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { NUMBER,
    OP_PLUS,
    OP_MINUS,
    OP_MULT,
    OP_DIV,
    BRACKET_LEFT,
    BRACKET_RIGHT } TokenType;
typedef enum { INT_MODE,
    FLOAT_MODE } CalcMode;

struct Token {
    TokenType type;
    union {
        int int_value;
        double float_value;
    } value;
};

// Function declarations
int get_priority(TokenType type);
int is_valid_char(int c);
int validate_expression(const char* input, int len);
int process_tokens(struct Token* tokens, int token_count, CalcMode mode, double* result);

#ifdef __cplusplus
}
#endif

#endif // CALCULATOR_H