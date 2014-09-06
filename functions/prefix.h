#ifndef PREFIX_H
#define PREFIX_H


extern const int ERRCODE_OK;
extern const int ERRCODE_UNEXPECTED_END;
extern const int ERRCODE_UNEXPECTED_VAR;
extern const int ERRCODE_UNEXPECTED_BIN_OPER;
extern const int ERRCODE_UNEXPECTED_UN_OPER;
extern const int ERRCODE_UNEXPECTED_LBRACK;
extern const int ERRCODE_UNEXPECTED_RBRACK;
extern const int ERRCODE_INVALID_TOKEN;
extern const int ERRCODE_BRACKET_UNCLOSED;
extern const int ERRCODE_SPARE_RBRACK;

void setOperator_C(char *op, int precedence, bool unary);
void unsetOperator_C(char *op);
void resetOperators_C();
void clearOperators_C();
int getOperatorArgCount_C(char *op);

int getErrorCode_C();
char* getErrorToken_C();
int getErrorPosition_C();

bool validate_C(char *infix);
char* toPrefix_C(char *infix);
char* toInfix_C(char *prefix);


#endif
