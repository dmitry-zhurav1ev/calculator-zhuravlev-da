#include <stdio.h>
#include <ctype.h>

typedef enum { NUMBER, OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, BRACKET_LEFT, BRACKET_RIGHT } TokenType;

struct Token
{
    TokenType type;
    int value;
};

int get_priority(TokenType type)
{
    switch (type)
    {
        case OP_MULT: case OP_DIV: return 2;
        case OP_PLUS: case OP_MINUS: return 1;
        default: return 0;
    }
}

int main()
{
    char input[1024];
    int input_len = 0;
    int c;

    while ((c = getchar()) != EOF && input_len < sizeof(input) - 1)
        { input[input_len++] = c; }
    input[input_len] = '\0';

    struct Token tokens[512];
    int token_count = 0;
    for (int i = 0; i < input_len; )
    {
        if (isspace(input[i])) { i++; continue; }
        if (isdigit(input[i]))
        {
            int num = 0;
            while (i < input_len && isdigit(input[i]))
                { num = num * 10 + (input[i++] - '0'); }
            tokens[token_count++] = (struct Token){NUMBER, num};
        }
        else
        {
            TokenType t;
            switch(input[i])
            {
                case '+': t = OP_PLUS; break;
                case '-': t = OP_MINUS; break;
                case '*': t = OP_MULT; break;
                case '/': t = OP_DIV; break;
                case '(': t = BRACKET_LEFT; break;
                case ')': t = BRACKET_RIGHT; break;
                default: i++; continue;
            }
            tokens[token_count++] = (struct Token){t, 0};
            i++;
        }
    }

    struct Token output[512], op_stack[512];
    int output_len = 0;
    int op_top = -1;

    for (int i = 0; i < token_count; i++)
    {
        struct Token t = tokens[i];
        if (t.type == NUMBER) { output[output_len++] = t; }
        else if (t.type == BRACKET_LEFT) { op_stack[++op_top] = t; }
        else if (t.type == BRACKET_RIGHT)
        {
            while (op_top >= 0 && op_stack[op_top].type != BRACKET_LEFT)
                { output[output_len++] = op_stack[op_top--]; }
            if (op_top >= 0) { op_top--; }
        }
        else
        {
            while (op_top >= 0 && op_stack[op_top].type != BRACKET_LEFT && get_priority(op_stack[op_top].type) >= get_priority(t.type))
                { output[output_len++] = op_stack[op_top--]; }
            op_stack[++op_top] = t;
        }
    }
    while (op_top >= 0) { output[output_len++] = op_stack[op_top--]; }

    int stack[512];
    int stack_top = -1;

    for (int i = 0; i < output_len; i++)
    {
        struct Token t = output[i];
        if (t.type == NUMBER) { stack[++stack_top] = t.value; }
        else
        {
            int b = stack[stack_top--];
            int a = stack[stack_top--];
            if (t.type == OP_PLUS) { stack[++stack_top] = a + b; }
            else if (t.type == OP_MINUS) { stack[++stack_top] = a - b; }
            else if (t.type == OP_MULT) { stack[++stack_top] = a * b; }
            else { stack[++stack_top] = a / b; }
        }
    }
    printf("%d\n", stack[0]);
    return 0;
}
