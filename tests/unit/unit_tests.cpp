#include <gtest/gtest.h>
extern "C" {
#include "calculator.h"
}

// Тесты для get_priority
TEST(PriorityTest, BasicOperators)
{
    EXPECT_EQ(get_priority(OP_PLUS), 1);
    EXPECT_EQ(get_priority(OP_MINUS), 1);
    EXPECT_EQ(get_priority(OP_MULT), 2);
    EXPECT_EQ(get_priority(OP_DIV), 2);
}

TEST(PriorityTest, NonOperators)
{
    EXPECT_EQ(get_priority(BRACKET_LEFT), 0);
    EXPECT_EQ(get_priority(BRACKET_RIGHT), 0);
    EXPECT_EQ(get_priority(NUMBER), 0);
}

// Тесты для is_valid_char
TEST(ValidationTest, ValidCharacters)
{
    EXPECT_TRUE(is_valid_char('1'));
    EXPECT_TRUE(is_valid_char('+'));
    EXPECT_TRUE(is_valid_char(' '));
    EXPECT_TRUE(is_valid_char('('));
}

TEST(ValidationTest, InvalidCharacters)
{
    EXPECT_FALSE(is_valid_char('@'));
    EXPECT_FALSE(is_valid_char('a'));
    EXPECT_FALSE(is_valid_char(','));
    EXPECT_FALSE(is_valid_char('\\'));
}

// Тесты для validate_expression
TEST(ExpressionTest, ValidExpressions)
{
    const char* expr1 = "(2+3)*4";
    EXPECT_TRUE(validate_expression(expr1, strlen(expr1)));

    const char* expr2 = "123 + 456/789";
    EXPECT_TRUE(validate_expression(expr2, strlen(expr2)));
}

TEST(ExpressionTest, InvalidExpressions)
{
    const char* expr1 = "(2+3*4";
    EXPECT_FALSE(validate_expression(expr1, strlen(expr1)));

    const char* expr2 = "2++3";
    EXPECT_FALSE(validate_expression(expr2, strlen(expr2)));

    const char* expr3 = "5 + a";
    EXPECT_FALSE(validate_expression(expr3, strlen(expr3)));
}

// Тесты для process_tokens
TEST(CalculationTest, BasicAddition)
{
    Token tokens[3];

    // Первый токен: NUMBER 2
    tokens[0].type = NUMBER;
    tokens[0].value.int_value = 2;

    // Второй токен: NUMBER 3
    tokens[1].type = NUMBER;
    tokens[1].value.int_value = 3;

    // Третий токен: OP_PLUS
    tokens[2].type = OP_PLUS;

    double result;
    ASSERT_EQ(process_tokens(tokens, 3, INT_MODE, &result), 0);
    EXPECT_EQ(result, 5);
}

TEST(CalculationTest, FloatDivision)
{
    Token tokens[3];

    tokens[0].type = NUMBER;
    tokens[0].value.float_value = 5.0;

    tokens[1].type = NUMBER;
    tokens[1].value.float_value = 2.0;

    tokens[2].type = OP_DIV;

    double result;
    ASSERT_EQ(process_tokens(tokens, 3, FLOAT_MODE, &result), 0);
    EXPECT_NEAR(result, 2.5, 0.0001);
}

TEST(CalculationTest, DivisionByZero)
{
    Token tokens[3];

    tokens[0].type = NUMBER;
    tokens[0].value.int_value = 5;

    tokens[1].type = NUMBER;
    tokens[1].value.int_value = 0;

    tokens[2].type = OP_DIV;

    double result;
    EXPECT_EQ(process_tokens(tokens, 3, INT_MODE, &result), 1);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}