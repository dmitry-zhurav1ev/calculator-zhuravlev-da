#include "calculator.h"

int get_priority(TokenType type)
{
    switch (type) {
    case OP_MULT:
    case OP_DIV:
        return 2;
    case OP_PLUS:
    case OP_MINUS:
        return 1;
    default:
        return 0;
    }
}

int is_valid_char(int c)
{
    return isdigit(c) || strchr("()*+-/ \t\n\r\f\v", c) != NULL;
}

int validate_expression(const char* input, int len)
{
    int brackets = 0;
    int expect_operand = 1;

    for (int i = 0; i < len; i++) {
        if (!is_valid_char(input[i]))
            return 0;

        if (input[i] == '(') {
            brackets++;
            expect_operand = 1;
        } else if (input[i] == ')') {
            if (--brackets < 0)
                return 0;
            expect_operand = 0;
        } else if (strchr("+-*/", input[i])) {
            if (expect_operand)
                return 0;
            expect_operand = 1;
        } else if (isdigit(input[i])) {
            while (i < len && isdigit(input[i]))
                i++;
            i--;
            expect_operand = 0;
        }
    }
    return brackets == 0 && !expect_operand;
}

int process_tokens(struct Token* tokens, int token_count, CalcMode mode, double* result)
{
    struct Token output[512], op_stack[512];
    int output_len = 0, op_top = -1;

    // Convert to RPN
    for (int i = 0; i < token_count; i++) {
        struct Token t = tokens[i];
        if (t.type == NUMBER) {
            output[output_len++] = t;
        } else if (t.type == BRACKET_LEFT) {
            op_stack[++op_top] = t;
        } else if (t.type == BRACKET_RIGHT) {
            while (op_top >= 0 && op_stack[op_top].type != BRACKET_LEFT)
                output[output_len++] = op_stack[op_top--];
            if (op_top >= 0)
                op_top--;
        } else {
            while (op_top >= 0 && op_stack[op_top].type != BRACKET_LEFT && get_priority(op_stack[op_top].type) >= get_priority(t.type))
                output[output_len++] = op_stack[op_top--];
            op_stack[++op_top] = t;
        }
    }
    while (op_top >= 0)
        output[output_len++] = op_stack[op_top--];

    // Evaluate RPN
    union {
        int int_stack[512];
        double float_stack[512];
    } stack;
    int stack_top = -1;

    for (int i = 0; i < output_len; i++) {
        struct Token t = output[i];
        if (t.type == NUMBER) {
            stack_top++;
            if (mode == INT_MODE)
                stack.int_stack[stack_top] = t.value.int_value;
            else
                stack.float_stack[stack_top] = t.value.float_value;
        } else {
            double b, a;
            if (mode == INT_MODE) {
                b = stack.int_stack[stack_top--];
                a = stack.int_stack[stack_top--];
            } else {
                b = stack.float_stack[stack_top--];
                a = stack.float_stack[stack_top--];
            }

            if (fabs(b) < 1e-4)
                return 1;

            double res;
            switch (t.type) {
            case OP_PLUS:
                res = a + b;
                break;
            case OP_MINUS:
                res = a - b;
                break;
            case OP_MULT:
                res = a * b;
                break;
            case OP_DIV:
                res = a / b;
                break;
            default:
                return 1;
            }

            if ((mode == INT_MODE && (res < -2e9 || res > 2e9)) || (mode == FLOAT_MODE && fabs(res) > 2e9))
                return 1;

            stack_top++;
            if (mode == INT_MODE)
                stack.int_stack[stack_top] = (int)res;
            else
                stack.float_stack[stack_top] = res;
        }
    }

    if (mode == INT_MODE)
        *result = stack.int_stack[0];
    else
        *result = stack.float_stack[0];

    return 0;
}