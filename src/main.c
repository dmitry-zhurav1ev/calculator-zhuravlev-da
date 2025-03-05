#include "calculator.h"

int main(int argc, char** argv)
{
    CalcMode mode = INT_MODE;
    if (argc == 2 && strcmp(argv[1], "--float") == 0)
        mode = FLOAT_MODE;

    char input[1024];
    size_t input_len = 0;
    int c;

    // Read input
    while ((c = getchar()) != EOF && input_len < sizeof(input) - 1) {
        if (!is_valid_char(c))
            return 1;
        input[input_len++] = c;
    }
    input[input_len] = '\0';

    // Validate expression
    if (!validate_expression(input, input_len))
        return 1;

    // Tokenization
    struct Token tokens[512];
    int token_count = 0;

    for (size_t i = 0; i < input_len;) {
        if (isspace(input[i])) {
            i++;
            continue;
        }

        if (isdigit(input[i])) {
            double num = 0;
            while (i < input_len && isdigit(input[i]))
                num = num * 10 + (input[i++] - '0');

            tokens[token_count].type = NUMBER;
            if (mode == INT_MODE)
                tokens[token_count++].value.int_value = (int)num;
            else
                tokens[token_count++].value.float_value = num;
        } else {
            TokenType t;
            switch (input[i]) {
            case '+':
                t = OP_PLUS;
                break;
            case '-':
                t = OP_MINUS;
                break;
            case '*':
                t = OP_MULT;
                break;
            case '/':
                t = OP_DIV;
                break;
            case '(':
                t = BRACKET_LEFT;
                break;
            case ')':
                t = BRACKET_RIGHT;
                break;
            default:
                return 1;
            }
            tokens[token_count++] = (struct Token) { t, { 0 } };
            i++;
        }
    }

    // Process tokens
    double result;
    if (process_tokens(tokens, token_count, mode, &result))
        return 1;

    // Output result
    if (mode == INT_MODE)
        printf("%d\n", (int)result);
    else
        printf("%.4f\n", result);

    return 0;
}