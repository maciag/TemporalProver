import std.array;
import std.conv;
import std.string;

import lexical;

import std.stdio;  // Debug.

/**
 * Funkcja wyciągająca słowo ze stosu (odcina spację na końcu i słowo)
 * 
 * Params:
 * stack = Stos w stringu
 * 
 * Returns:
 * Słowo
 */
char[] pop(ref char[] stack)
{
	if(stack.length == 0)
		return new char[0];
	
	if(stack[$-1] == '\t')  // Obcinamy ostatnią spację
		stack.length--;
	
	long lastSpace = stack.lastIndexOf("\t");  // Znajdujemy początek słowa
	
	if(lastSpace == -1)
	{
		char[] word = stack;
		stack.length = 0;
		return word;
	}
	else
	{
		char[] word = stack[lastSpace+1..$];
		stack.length = lastSpace+1;
		return word;
	}
}

/**
 * Funkcja wrzucająca słowo na stod.
 * 
 * Params:
 * stack = Stos w stringu
 * word = Słowo
 */
void push(ref char[] stack, char[] word)
{
	stack ~= (word ~ '\t');
}

/**
 * Funkcja budująca wyrażenie dla operatora i stosu
 */
void buildExpression(ref char[] stack, char[] operator)
{
	int argc = operator.argumentCount();
	char[][] args;
	args.length = argc;
	
	for(int i = argc-1; i>=0; i--)
	{
		args[i] = stack.pop();
	}
	
	char[] res = operator;
	
	foreach(char[] arg; args)
	{
		res ~= " " ~ arg;
	}
	
	stack.push(res);
}

/**
 * Funkcja konwertująca formułę do postaci prefiksowej
 *
 * Params:
 * infix = Formuła w notacji infiksowej
 * 
 * Returns:
 * Formuła w notacji prefiksowej
 */
char[] toPrefix(char[] infix)
{
	/*
	 * Tworzenie tablic dynamicznych.
	 * - Używamy atrybutu scope, by zaalokować zmienne na stosie i usunąć je po wyjściu z funkcji
	 * - Zmienna res nie jest alokowana na stosie, ponieważ zostanie zwrócona z funkcji
	 */
	scope char[] operatorStack, operandStack;
	
	/*
	 * Tablice będą miały zerową długość, ale zarezerwujemy od razu miejsce tak, aby przy zmianie
	 * rozmiaru nie były realokowane. Nawet przy formule długości miliona znaków trzy tablice
	 * zajmą maksymalnie 4MB pamięci, co nie powinno powodować problemów na współczesnym sprzęcie.
	 */
	operatorStack.reserve(infix.length * 2);
	operandStack.reserve(infix.length * 2);
	
	// Konwersja
	operatorStack.push("(".dup);
	infix ~= ")";
	
	char[] word;
	while((word = lparse(infix)) != "")
	{
		if(word == "(")
			operatorStack.push(word);
		
		else if(word == ")")
		{
			// Zbieramy, dopóki nie będzie nawiasu
			char[] popped = operatorStack.pop();
			
			while(popped != "(")
			{
				buildExpression(operandStack, popped);
				popped = operatorStack.pop();
			}
		}
		
		else if(word.isOperator())
		{
			char[] popped = operatorStack.pop();
			
			while(popped.hasNotLowerPrecedence(word))
			{
				// Zdejmowanie odpowiedniej ilości argumentów dla operatora i odłożenie wyrażenia
				buildExpression(operandStack, popped);
				popped = operatorStack.pop();
			}
			
			operatorStack.push(popped);
			operatorStack.push(word);
		}
		
		else
		{
			operandStack.push(word);
		}
	}
	
	return operandStack.pop();
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
	import core.runtime;
	Runtime.initialize();
	
	char[] dstr = toImpl!(string, char*)(infix);
	dstr = toPrefix(dstr);
	
	return toStringz(dstr);
}