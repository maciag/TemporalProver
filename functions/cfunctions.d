import core.runtime;
import std.conv;
import std.string;

import lexical;
import prefix;

/**
 * Ustawia operator
 * 
 * Params:
 * operator = ciąg znaków operatora
 * precedence = priorytet operatora
 * unary = jeden argument (jak false, to dwa). Operatory unarne stoją z przodu argumentu
 */
extern(C) public void setOperator_C(char* operator, int precedence, bool unary)
{
	Runtime.initialize();
	
	string dstr = toImpl!(string, char*)(operator);
	setOperator(dstr, precedence, unary);
}

/**
 * Usuwa operator z listy
 * 
 * Params:
 * operator = operator do usunięcia
 */
extern(C) public void unsetOperator_C(char* operator)
{
	Runtime.initialize();
	
	string dstr = toImpl!(string, char*)(operator);
	unsetOperator(dstr);
}

/**
 * Dodaje standardowe operatory
 */
extern(C) public void resetOperators_C()
{
	Runtime.initialize();
	
	resetOperators();
}

/**
 * Czyści listę operatorów
 */
extern(C) public void clearOperators_C()
{
	Runtime.initialize();
	
	clearOperators();
}

/**
 * Zwraca liczbę argumentów operatora
 * 
 * Returns:
 * liczba argumentów lub 0, gdy operator jest zmienną lub nie istnieje
 */
extern(C) int getOperatorArgCount_C(char *operator)
{
	Runtime.initialize();
	
	string dstr = toImpl!(string, char*)(operator);
	return getOperatorArgCount(dstr);
}

/**
 * Zwraca kod błędu
 * 
 * Returns:
 * kod błędu
 */
extern(C) int getErrorCode_C()
{
	return errorCode;
}

/**
 * Zwraca token
 * 
 * Returns:
 * token lub pusty ciąg znaków, gdy formuła nieprawidłowo się kończy lub błąd dotyczy nawiasów
 */
extern(C) immutable(char)* getErrorToken_C()
{
	return toStringz(errorToken);
}

/**
 * Zwraca pozycję błędu
 * 
 * Returns:
 * pozycja błędu lub koniec formuły, gdy formuła nieprawidłowo się kończy lub błąd dotyczy nawiasów
 */
extern(C) int getErrorPosition_C()
{
	return errorPosition;
}

/**
 * Funkcja walidująca postać infiksową - funkcja do wywołania z języka C/C++
 *
 * Params:
 * infix = Formuła w notacji infiksowej
 * 
 * Returns:
 * true, jeżeli formuła jest poprawna
 */
extern(C) bool validate_C(char* infix)
{
	Runtime.initialize();
	
	char[] dstr = toImpl!(char[], char*)(infix);
	return validate(dstr);
}

/**
 * Funkcja konwertująca formułę do postaci prefiksowej - funkcja do wywołania z języka C/C++
 *
 * Params:
 * infix = Formuła w notacji infiksowej
 * 
 * Returns:
 * Formuła w notacji prefiksowej
 */
extern(C) immutable(char)* toPrefix_C(char* infix)
{
	Runtime.initialize();
	
	char[] dstr = toImpl!(char[], char*)(infix);
	dstr = toPrefix(dstr);
	
	return toStringz(dstr);
}

/**
 * Funkcja konwertująca formułę do postaci infiksowej - funkcja do wywołania z języka C/C++
 *
 * Params:
 * prefix = Formuła w notacji prefiksowej
 * 
 * Returns:
 * Formuła w notacji infiksowej
 */
extern(C) immutable(char)* toInfix_C(char* prefix)
{
	Runtime.initialize();
	
	char[] dstr = toImpl!(char[], char*)(prefix);
	dstr = toInfix(dstr);
	
	return toStringz(dstr);
}