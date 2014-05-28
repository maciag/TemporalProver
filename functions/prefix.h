#ifndef PREFIX_H
#define PREFIX_H

extern "C" const int ERRCODE_OK;
extern "C" const int ERRCODE_UNEXPECTED_END;
extern "C" const int ERRCODE_UNEXPECTED_VAR;
extern "C" const int ERRCODE_UNEXPECTED_BIN_OPER;
extern "C" const int ERRCODE_UNEXPECTED_UN_OPER;
extern "C" const int ERRCODE_UNEXPECTED_LBRACK;
extern "C" const int ERRCODE_UNEXPECTED_RBRACK;
extern "C" const int ERRCODE_INVALID_TOKEN;
extern "C" const int ERRCODE_BRACKET_UNCLOSED;
extern "C" const int ERRCODE_SPARE_RBRACE;

extern "C" void setOpeator_C(char *op, int precedence, bool unary);
extern "C" void unsetOpeator_C(char *op);
extern "C" void resetOperators_C();
extern "C" void clearOperators_C();

extern "C" int getErrorCode_C();
extern "C" char* getErrorToken_C();
extern "C" int getErrorPosition_C();

extern "C" bool validate_C(char *infix);
extern "C" char* toPrefix_C(char *infix);
#endif