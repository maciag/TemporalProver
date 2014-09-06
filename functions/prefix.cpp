#include "../functions/prefix.h"

/// Kody błędów
const int ERRCODE_OK = 0;
const int ERRCODE_UNEXPECTED_END = 1;
const int ERRCODE_UNEXPECTED_VAR = 2;
const int ERRCODE_UNEXPECTED_BIN_OPER = 3;
const int ERRCODE_UNEXPECTED_UN_OPER = 4;
const int ERRCODE_UNEXPECTED_LBRACK = 5;
const int ERRCODE_UNEXPECTED_RBRACK = 6;
const int ERRCODE_INVALID_TOKEN = 7;
const int ERRCODE_BRACKET_UNCLOSED = 8;
const int ERRCODE_SPARE_RBRACK = 9;

void setOperator_C(char *op, int precedence, bool unary)
{

}

void unsetOperator_C(char *op)
{

}

void resetOperators_C()
{

}

void clearOperators_C()
{

}

int getOperatorArgCount_C(char *op)
{
    return 0;
}

int getErrorCode_C()
{
    return 0;
}

char* getErrorToken_C()
{
    return 0;
}

int getErrorPosition_C()
{
    return 0;
}

bool validate_C(char *infix)
{
    return false;
}

char* toPrefix_C(char *infix)
{
    return 0;
}

char* toInfix_C(char *prefix)
{
    return 0;
}

