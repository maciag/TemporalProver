#include "../functions/lexical.h"

#include <QMap>
#include <cctype>
#include <iostream>

/// Informacje o błędzie - kod, początek i koniec
int errorCode;
int errorPosition;
QString errorToken;

/// Mapa operatorów. NIE ZAWIERA NAWIASÓW!
QMap<QString, Operator*> operators;

void setOperator_C(QString op, int precedence, bool unary)
{
	Operator *o = new Operator();
	
	o->oper = QString(op);
	o->precedence = precedence;
	o->unary = unary;
	
	operators[QString(op)] = o;
}

void unsetOperator_C(QString op)
{
	operators.remove(op);
}

void resetOperators_C()
{
	clearOperators_C();
	setOperator_C("&", 3, false);
	setOperator_C("|", 3, false);
	setOperator_C("^", 3, false);
	setOperator_C(">", 2, false);
	setOperator_C("=", 1, false);
	setOperator_C("!", 5, true);
	setOperator_C("U", 4, false);
	setOperator_C("X", 5, true);
	setOperator_C("G", 5, true);
	setOperator_C("F", 5, true);
}

void clearOperators_C()
{
	operators.clear();
}

int getOperatorArgCount_C(QString op)
{
	if(operators.contains(op))
	{
		return operators[op]->unary ? 1 : 2;
	}
	else
	{
		return 0;
	}
}

int getErrorCode_C()
{
	return errorCode;
}

QString getErrorToken_C()
{
	return errorToken;
}

int getErrorPosition_C()
{
	return errorPosition;
}

bool validate_C(QString infix)
{
	return validate(infix);
}

bool isVarChar(char c)
{
	return (isalpha(c) && islower(c)) || isdigit(c) || c == '_';
}

bool isOperator(QString s)
{
	QMap<QString, Operator*>::iterator iter;
	
	for(iter = operators.begin(); iter != operators.end(); ++iter)
	{
		if(s == iter.key())
			return true;
	}
	
	return false;
}

bool hasNotLowerPrecedence(QString op1, QString op2)
{
	Operator *oper1 = 0;
	Operator *oper2 = 0;
	
	QMap<QString, Operator*>::iterator iter;
	
	for(iter = operators.begin(); iter != operators.end(); ++iter)
	{
		if(op1 == iter.key())
			oper1 = iter.value();
		
		if(op2 == iter.key())
			oper2 = iter.value();
	}
	
	if(oper1 != 0 && oper2 != 0)
	{
		if(oper1->precedence == oper2->precedence)
			return !oper1->unary;
		
		else
		{
			return oper1->precedence >= oper2->precedence;
		}
	}
	else return false;  // Pewnie któryś jest nawiasem
}

int argumentCount(QString op)
{
	QMap<QString, Operator*>::iterator iter;
	
	for(iter = operators.begin(); iter != operators.end(); ++iter)
	{
		if(iter.value()->oper == op)
		{
			return iter.value()->unary ? 1 : 2;
		}
	}
	
	return 0;
}

void setError(const int errcode, QString token, int initialLength, int currentLength)
{
	errorCode = errcode;
	errorToken = token;
	errorPosition = (int) (initialLength - (token.length() + currentLength));
}

QString parse(QString & src, bool right)
{
	if(src.length() == 0)
		return "";
	
	int pos = right ? (int) src.length()-1 : 0;
	
	// Obcinamy białe znaki
	while(isspace(src[pos].toLatin1()) && (right ? (pos > 0) : (pos < src.length()-1)))
		right ? pos-- : pos++;
	
	int whiteCutPos = pos;
	
	// Nazwa zmiennej
	if(isVarChar(src[pos].toLatin1()))
	{
		while(right ? (pos > 0) : (pos < src.length()-1))
		{
			right ? pos-- : pos++;
			if(!isVarChar(src[pos].toLatin1()))
			{
				right ? pos++ : pos--;  // Ten znak nie wchodzi, więc wracamy
				break;
			}
		}
	}
	
	// Nawias
	else if(src[pos].toLatin1() == '(' || src[pos].toLatin1() == ')')
	{
		// Nic nie robimy, po prostu bierze jeden znak
	}
	
	// Operator
	else if(!isspace(src[pos].toLatin1()))
	{
		while(right ? (pos > 0) : (pos < src.length()-1))
		{
			right ? pos-- : pos++;
			
			QString current = right ? src.mid(pos, whiteCutPos-pos) : src.mid(whiteCutPos, pos-whiteCutPos);
			
			if(isOperator(current) || isVarChar(src[pos].toLatin1()) || isspace(src[pos].toLatin1()))
			{
				right ? pos++ : pos--;  // Ten znak nie wchodzi, więc wracamy
				break;
			}
		}
	}
	
	/*
	 * Zostają tylko nawiasy i wykrzyknik, które zawsze zwracamy pojedynczo,
	 * więc nic więcej nie trzeba robić. Tak więc po prostu zwracamy wartość
	 * od indeksu pos do końca i obcinamy białe znaki
	 */
	QString word = right ? src.mid(pos).trimmed() : src.left(pos+1).trimmed();
	src = right ? src.left(pos) : src.mid(pos+1);  // Realokacja, ale chyba nie da się jej uniknąć
	
	return word;
}


QString lparse(QString & src)
{
	return parse(src, false);
}

QString rparse(QString & src)
{
	return parse(src, true);
}

bool validate(QString infix)
{
	int initialLength = infix.length();  // do obliczania pozycji błędu
	QString previousType = "";  // operator lub "var", jeżeli zniemma
	QString token;
	
	int bracketBalance = 0;
	
	while(true)  // Pętlę przerwę z wnętrza po sprawdzeniu pustego tokenu, czyli końca formuły
	{
		token = lparse(infix);
		
		/*
		 * Ponieważ mamy poprzedni, a nie następny token, adaptujemy reguły i
		 * określamy, czy sprawdzany token może stać po poprzednim
		 */
		
		// Koniec formuły
		if(token == "")
		{
			// Zabroniony po operatorach jedno- i dwuargumentowych, nawiasie otwierającym i na początku
			if(argumentCount(previousType) > 0 || previousType == "(" || previousType == "")
			{
				setError(ERRCODE_UNEXPECTED_END, token, initialLength, infix.length());
				return false;
			}
			
			break;
		}
		
		// Zmienna
		else if(isVarChar(token[0].toLatin1()))
		{
			/*
			 * Jeżeli pierwszy znak jest znakiem zmiennej, to pozostałe też - inaczej
			 * byłyby rozdzielone przez metodę lparse.
			 */
			
			// Zabroniona po nawiasie zamykającym i zmiennej
			if(previousType == ")" || previousType == "var")
			{
				setError(ERRCODE_UNEXPECTED_VAR, token, initialLength, infix.length());
				return false;
			}
			
			previousType = "var";
		}
		
		// Operator dwuargumentowy
		else if(argumentCount(token) == 2)
		{
			// Zabroniony po operatorze jedno- lub dwuargumentowym, nawiasie otwierającym i na początku
			if(argumentCount(previousType) > 0 || previousType == "(" || previousType == "")
			{
				setError(ERRCODE_UNEXPECTED_BIN_OPER, token, initialLength, infix.length());
				return false;
			}
			
			previousType = token;
		}
		
		// Operator jednoargumentowy
		else if(argumentCount(token) == 1)
		{
			// Zabroniony po zmiennej i nawiasie zamykającym
			if(previousType == "var" || previousType == ")")
			{
				setError(ERRCODE_UNEXPECTED_UN_OPER, token, initialLength, infix.length());
				return false;
			}
			
			previousType = token;
		}
		
		// Nawias otwierający
		else if(token == "(")
		{
			bracketBalance++;
			
			// Zabroniony po zmiennej i nawiasie zamykającym
			if(previousType == "var" || previousType == ")")
			{
				setError(ERRCODE_UNEXPECTED_LBRACK, token, initialLength, infix.length());
				return false;
			}
			
			previousType = token;
		}
		
		// Nawias zamykający
		else if(token == ")")
		{
			bracketBalance--;
			
			if(bracketBalance < 0)
			{
				setError(ERRCODE_SPARE_RBRACK, token, initialLength, infix.length());
				return false;
			}
			
			// Zabroniony po operatorze jedno- lub dwuargumentowym, nawiasie otwierającym i na początku
			if(argumentCount(previousType) > 0 || previousType == "(" || previousType == "")
			{
				setError(ERRCODE_UNEXPECTED_RBRACK, token, initialLength, infix.length());
				return false;
			}
			
			previousType = token;
		}
		
		// Nieznany token
		else
		{
			setError(ERRCODE_INVALID_TOKEN, token, initialLength, infix.length());
			return false;
		}
	}
	
	// Balans nawiasów
	if(bracketBalance > 0)  // Przypadek <0 rozpatrzony przy napotkanym nawiasie
	{
		setError(ERRCODE_BRACKET_UNCLOSED, token, initialLength, infix.length());
		return false;
	}
	
	else
	{
		errorCode = ERRCODE_OK;
		return true;
	}
}