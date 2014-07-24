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
	
	auto lastSpace = stack.lastIndexOf("\t");  // Znajdujemy początek słowa
	
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
 * Funkcja wrzucająca słowo na stos.
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
 * Funkcja budująca wyrażenie prefiksowe dla operatora i stosu
 * 
 * Params:
 * stack = Stos w stringu
 * operator = Operator
 */
void buildPrefixExpression(ref char[] stack, char[] operator)
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
 * Funkcja budująca wyrażenie infiksowe dla operatora i stosu
 * 
 * Funkcja każdemu wyrażeniu na stosie przyporządkowuje główny operator
 * tego wyrażenia. Podczas składania sprawdzane są główne operatory
 * argumentów i te, które mają niższy priorytet od operatora będącego
 * argumentem tej metody, są ujmowane w nawiasy. Dzięki temu
 * nie są wstawiane zbędne nawiasy.
 * 
 * Params:
 * stack = Stos w stringu
 * operator = Operator
 */
void buildInfixExpression(ref char[] stack, char[] operator)
{
	int argc = operator.argumentCount();
	char[][] args;
	args.length = argc;
	
	for(int i = argc-1; i>=0; i--)
	{
		args[i] = stack.pop();
	}
	
	char[] res;
	
	if(argc == 1)
	{
		char[][] arg0 = args[0].split("\n");  // Wypakowujemy operator
		
		if(operator.hasNotLowerPrecedence(arg0[1]))
			res = operator ~ (operator.length > 1 ? " " : "") ~ "(" ~ arg0[0].strip() ~ ")";
		else
			res = operator ~ (operator.length > 1 ? " " : "") ~ arg0[0].strip();
	}
	
	else if(argc == 2)
	{
		char[][] arg0 = args[0].split("\n");  // Wypakowujemy operatory
		char[][] arg1 = args[1].split("\n");
		
		if(operator.hasNotLowerPrecedence(arg1[1]))
			res = "(" ~ arg1[0].strip() ~ ") ";
		else
			res = arg1[0].strip() ~ " ";
		
		res ~= operator;
		
		if(operator.hasNotLowerPrecedence(arg0[1]))
			res ~= " (" ~ arg0[0].strip() ~ ") ";
		else
			res ~= " " ~ arg0[0].strip();
	}
	
	stack.push(res ~ "\n" ~ operator);  // Zapisujemy z operatorem
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
				buildPrefixExpression(operandStack, popped);
				popped = operatorStack.pop();
			}
		}
		
		else if(word.isOperator())
		{
			char[] popped = operatorStack.pop();
			
			while(popped.hasNotLowerPrecedence(word))
			{
				// Zdejmowanie odpowiedniej ilości argumentów dla operatora i odłożenie wyrażenia
				buildPrefixExpression(operandStack, popped);
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
 * Funkcja konwertująca formułę do postaci infiksowej
 * 
 * Do wyrażeń na stosie dopisywana jest druga linia z głównym operatorem
 * (lub pusta dla zmiennych) - potrzebne przy ustalaniu wymaganych nawiasów.
 *
 * Params:
 * prefix = Formuła w notacji infiksowej
 * 
 * Returns:
 * Formuła w notacji infiksowej
 */
char[] toInfix(char[] prefix)
{
	/*
	 * Tworzenie tablic dynamicznych.
	 * - Używamy atrybutu scope, by zaalokować zmienne na stosie i usunąć je po wyjściu z funkcji
	 * - Zmienna res nie jest alokowana na stosie, ponieważ zostanie zwrócona z funkcji
	 */
	scope char[] stack;
	
	char[] word;
	while((word = rparse(prefix)) != "")
	{
		if(word.isOperator())
		{
			buildInfixExpression(stack, word);
		}
		
		else
		{
			stack.push(word ~ "\n");  // Dopisujemy pustą linię
		}
	}
	
	return stack.split("\n")[0];
}