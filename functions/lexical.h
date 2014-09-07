#ifndef LEXICAL_H
#define LEXICAL_H

#include <QString>

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

/// Struktura opisująca operator
struct Operator
{
	QString oper;
	int precedence;
	bool unary = false;
};

void setOperator_C(QString op, int precedence, bool unary);
void unsetOperator_C(QString op);
void resetOperators_C();
void clearOperators_C();
int getOperatorArgCount_C(QString op);
int getErrorCode_C();
QString getErrorToken_C();
int getErrorPosition_C();
bool validate_C(QString infix);


bool isVarChar(char c);
bool isOperator(QString s);
bool hasNotLowerPrecedence(QString op1, QString op2);
int argumentCount(QString op);
void setError(const int errcode, QString token, int initialLength, int currentLength);
QString parse(QString & src, bool right);
QString lparse(QString & src);
QString rparse(QString & src);
bool validate(QString infix);

#endif
