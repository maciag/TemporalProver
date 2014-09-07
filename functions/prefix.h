#ifndef PREFIX_H
#define PREFIX_H

#include "lexical.h"

#include <QString>
#include <QStringList>

QString toPrefix_C(QString infix);
char *toPrefix_C(char *infix);
QString toInfix_C(QString prefix);
char *toInfix_C(char *prefix);

QString pop(QString & stack);
void push(QString & stack, QString word);
void buildPrefixExpression(QString & stack, QString oper);
void buildInfixExpression(QString & stack, QString oper);

QString toPrefix(QString infix);
QString toInfix(QString prefix);
#endif
