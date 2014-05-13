import std.conv;
import std.string;
import std.uni;

/**
 * Funkcja sprawdza, czy znak należy do nazwy zmiennej
 * 
 * Params:
 * c = znak
 * 
 * Returns:
 * Czy należy do nazwy zmiennej
 */
bool isVarChar(char c)
{
	return c.isAlpha() ||  c.isNumber();
}

/**
 * Funkcja sprawdza, czy znak należy do operatora. Wykluczamy nawiasy i wykrzykniki, które
 * nie mogą wchodzić w skład innych operatorów
 * 
 * Params:
 * c = znak
 * 
 * Returns:
 * Czy należy do nazwy operatora
 */
bool isOpChar(char c)
{
	return !c.isAlpha() && !c.isNumber() && !c.isWhite && c != '(' && c != ')' && c !='!';
}

/**
 * Funkcja sprawdza, czy ciąg znaków jest operatorem
 * 
 * Params:
 * s = ciąg znaków
 * 
 * Returns:
 * Czy należy do nazwy operatora
 */
bool isOperator(char[] s)
{
	return s == "&" || s == "|" || s == "^" || s == ">" || s == "=" || s == "!";
}

/**
 * Funkcja sprawdza, czy op1 ma niemniejszy priorytet od op2
 * 
 * Params:
 * op1 = operator 1
 * op2 = operator 2
 * 
 * Returns:
 * Czy op1 ma niemniejszy priorytet od op2
 */
bool hasNotLowerPrecedence(char[] op1, char[] op2)
{
	switch(op1)
	{
		case "!":
		{
			/*
			 * Negację wykonujemy od prawej, więc (jakkolwiek to brzmi) musi mieć
			 * priorytet mniejszy od samej siebie, zaś większy od każdego innego
			 * operatora.
			 */
			return op2 != "!";
		}
		
		case "&":
		case "|":
		case "^":
		{
			return op2 == "&" || op2 == "|" || op2 == "^" || op2 == ">" || op2 == "=";
		}
		
		case ">":
		case "=":
		{
			return op2 == ">" || op2 == "=";
		}
		
		default:
		{
			return false;
		}
	}
}

/**
 * Funkcja zwraca ilość argumentów operatora
 * 
 * Params:
 * operator = Operator
 * 
 * Returns:
 * Ilość argumentów
 */
int argumentCount(char[] operator)
{
	switch(operator)
	{
		case "!":
		{
			return 1;
		}
		
		case "&":
		case "|":
		case "^":
		case ">":
		case "=":
		{
			return 2;
		}
		
		default:
		{
			return 0;  // To nie powinno się zdarzyć
		}
	}
}

/**
 * Funkcja pobierająca pierwsze słowo formuły z prawej lewej. Słowo to jest
 * odcięte ze źródłowego ciągu znaków. Białe znaki są pomijane, ale zawsze
 * dzielą słowa.
 * 
 * Możliwe słowa:
 * - ciąg znaków alfanumerycznych - zmienna
 * - nawias
 * - ciąg znaków niealfanumerycznych z wyłączeniem nawiasów - operator
 * 
 * Params
 * src = wejściowy ciąg znaków. Przekazanie przez referencję.
 * 
 * Returns:
 * Słowo
 */
char[] lparse(ref char[] src)
{
	if(src.length == 0)
		return "".dup;
	
	int pos = 0;
	
	// Obcinamy białe znaki
	while(src[pos].isWhite() && pos < src.length-1)
		pos++;
	
	// Nazwa zmiennej
	if(src[pos].isVarChar())
	{
		while(pos < src.length-1)
		{
			pos++;
			if(!src[pos].isVarChar())
			{
				pos--;  // Ten znak nie wchodzi, więc wracamy
				break;
			}
		}
	}
	
	// Operator
	else if(src[pos].isOpChar())
	{
		while(pos < src.length-1)
		{
			pos++;
			if(!src[pos].isOpChar())
			{
				pos--;  // Ten znak nie wchodzi, więc wracamy
				break;
			}
		}
	}
	
	/*
	 * Zostają tylko nawiasy i wykrzyknik, które zawsze zwracamy pojedynczo,
	 * więc nic więcej nie trzeba robić. Tak więc po prostu zwracamy wartość
	 * od indeksu pos do końca i obcinamy białe znaki
	 */
	char[] word = src[0..pos+1].stripLeft();
	src = src[pos+1..$];  // Realokacja, ale chyba nie da się jej uniknąć
	return word;
}

/**
 * Funkcja walidująca postać infiksową
 *
 * Params:
 * infix = Formuła w notacji infiksowej
 * 
 * Returns:
 * true, jeżeli formuła jest poprawna
 */
bool validate(char[] infix)
{
	char[] previousType = "".dup;  // operator lub "var", jeżeli zniemma
	char[] token;
	
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
			// Zabroniony po operatorze dwuargumentowym, negacji, nawiasie otwierającym i na początku
			if(previousType == "&" || previousType == "|" || previousType == "^" ||
				previousType == ">" || previousType == "=" || previousType == "!" ||
				previousType == "(" || previousType == "")
			{
				return false;
			}
			
			break;
		}
		
		// Zmienna
		else if(token[0].isVarChar)
		{
			/*
			 * Jeżeli pierwszy znak jest alfanumeryczny, to pozostałe też - inaczej
			 * byłyby rozdzielone przez metodę lparse.
			 */
			 
			 // Zabroniona po nawiasie zamykającym i zmiennej
			 if(previousType == ")" || previousType == "var")
			 {
				return false;
			 }
			 
			 previousType = "var".dup;
		}
		
		// Operator dwuargumentowy
		else if(token == "&" || token == "|" || token == "^" || token == ">" || token == "=")
		{
			// Zabroniony po operatorze dwuargumentowym, negacji, nawiasie otwierającym i na początku
			if(previousType == "&" || previousType == "|" || previousType == "^" ||
				previousType == ">" || previousType == "=" || previousType == "!" ||
				previousType == "(" || previousType == "")
			{
				return false;
			}
			
			previousType = token;
		}
		
		// Negacja
		else if(token == "!")
		{
			// Zabroniony po zmiennej i nawiasie zamykającym
			if(previousType == "var" || previousType == ")")
			{
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
				return false;
			}
			
			previousType = token;
		}
		
		// Nawias zamykający
		else if(token == ")")
		{
			bracketBalance--;
			
			// Zabroniony po operatorze dwuargumentowym, negacji, nawiasie otwierającym i na początku
			if(previousType == "&" || previousType == "|" || previousType == "^" ||
				previousType == ">" || previousType == "=" || previousType == "!" ||
				previousType == "(" || previousType == "")
			{
				return false;
			}
			
			previousType = token;
		}
		
		// Nieznany token
		else
		{
				return false;
		}
	}
	
	return bracketBalance == 0;
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
	import core.runtime;
	Runtime.initialize();
	
	char[] dstr = toImpl!(string, char*)(infix);
	return validate(dstr);
}