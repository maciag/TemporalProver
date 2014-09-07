#include "prefix.h"

#include "lexical.h"
#include <iostream>

QString toPrefix_C(QString infix)
{
    return toPrefix(infix);
}

char *toPrefix_C(char *infix)
{
	return toPrefix(QString(infix)).toUtf8().data();
}

QString toInfix_C(QString prefix)
{
	return toInfix(prefix);
}

char *toInfix_C(char *prefix)
{
	return toInfix(QString(prefix)).toUtf8().data();
}

QString pop(QString & stack)
{
	if(stack.length() == 0)
		return "";
	
	if(stack.right(1) == "\t")  // Obcinamy ostatnią spację
		stack = stack.left(stack.length()-1);
	
	int lastSpace = stack.lastIndexOf("\t");  // Znajdujemy początek słowa
	
	if(lastSpace == -1)
	{
		QString word = stack;
		stack = "";
		return word;
	}
	else
	{
		QString word = stack.mid(lastSpace+1);
		stack = stack.left(lastSpace+1);
		return word;
	}
}

void push(QString & stack, QString word)
{
	stack += (word + "\t");
}

void buildPrefixExpression(QString & stack, QString oper)
{
	int argc = argumentCount(oper);
	QStringList args;
	
	for(int i = 0; i < argc; i++)
	{
		args.append(pop(stack));
	}
	
	QString res = oper;
	
	for(int i = args.length()-1; i >= 0; i--)
	{
		res += " " + args[i];
	}
	
	push(stack, res);
}

void buildInfixExpression(QString & stack, QString oper)
{
	int argc = argumentCount(oper);
	QStringList args;
	
	for(int i = 0; i < argc; i++)
	{
		args.append(pop(stack));
	}
	
	QString res;
	
	if(argc == 1)
	{
		QStringList arg0 = args[0].split("\n");  // Wypakowujemy operator
		
		if(hasNotLowerPrecedence(oper, arg0[1]))
			res = oper + (oper.length() > 1 ? " " : "") + "(" + arg0[0].trimmed() + ")";
		else
			res = oper + (oper.length ()> 1 ? " " : "") + arg0[0].trimmed();
	}
	
	else if(argc == 2)
	{
		// UWAGA, zamieniam argumenty miejscami!!!
		QStringList arg0 = args[1].split("\n");  // Wypakowujemy operatory
		QStringList arg1 = args[0].split("\n");
		
		if(hasNotLowerPrecedence(oper, arg1[1]))
			res = "(" + arg1[0].trimmed() + ") ";
		else
			res = arg1[0].trimmed() + " ";
		
		res += oper;
		
		if(hasNotLowerPrecedence(oper, arg0[1]))
			res += " (" + arg0[0].trimmed() + ") ";
		else
			res += " " + arg0[0].trimmed();
	}
	
	push(stack, res + "\n" + oper);  // Zapisujemy z operatorem
}

QString toPrefix(QString infix)
{
	QString operatorStack, operandStack;
	
	push(operatorStack, "(");
	infix += ")";
	
	QString word;
	while((word = lparse(infix)) != "")
	{
		if(word == "(")
			push(operatorStack, word);
		
		else if(word == ")")
		{
			// Zbieramy, dopóki nie będzie nawiasu
			QString popped = pop(operatorStack);
			
			while(popped != "(")
			{
				buildPrefixExpression(operandStack, popped);
				popped = pop(operatorStack);
			}
		}
		
		else if(isOperator(word))
		{
			QString popped = pop(operatorStack);
			
			while(hasNotLowerPrecedence(popped, word))
			{
				// Zdejmowanie odpowiedniej ilości argumentów dla operatora i odłożenie wyrażenia
				buildPrefixExpression(operandStack, popped);
				popped = pop(operatorStack);
			}
			
			push(operatorStack, popped);
			push(operatorStack, word);
		}
		
		else
		{
			push(operandStack, word);
		}
	}
	
	return pop(operandStack);
}

QString toInfix(QString prefix)
{
	QString stack;
	
	QString word;
	while((word = rparse(prefix)) != "")
	{
		if(isOperator(word))
		{
			buildInfixExpression(stack, word);
		}
		
		else
		{
			push(stack, word + "\n");  // Dopisujemy pustą linię
		}
	}
	
	return stack.split("\n")[0];
}