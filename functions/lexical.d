import std.algorithm;
import std.conv;
import std.string;
import std.uni;

/// Kody błędów
extern(C) const int ERRCODE_OK = 0;
extern(C) const int ERRCODE_UNEXPECTED_END = 1;
extern(C) const int ERRCODE_UNEXPECTED_VAR = 2;
extern(C) const int ERRCODE_UNEXPECTED_BIN_OPER = 3;
extern(C) const int ERRCODE_UNEXPECTED_UN_OPER = 4;
extern(C) const int ERRCODE_UNEXPECTED_LBRACK = 5;
extern(C) const int ERRCODE_UNEXPECTED_RBRACK = 6;
extern(C) const int ERRCODE_INVALID_TOKEN = 7;
extern(C) const int ERRCODE_BRACKET_UNCLOSED = 8;
extern(C) const int ERRCODE_SPARE_RBRACE = 9;

/// Informacje o błędzie - kod, początek i koniec
int errorCode;
int errorPosition;
string errorToken;

/// Struktura opisująca operator
struct Operator
{
	string operator;
	int precedence;
	bool unary = false;
}

/// Mapa operatorów. NIE ZAWIERA NAWIASÓW!
Operator[string] operators;

/**
 * Ustawia operator
 * 
 * Params:
 * operator = ciąg znaków operatora
 * precedence = priorytet operatora
 * unary = jeden argument (jak false, to dwa). Operatory unarne stoją z przodu argumentu
 */
public void setOperator(string operator, int precedence, bool unary)
{
	Operator o = {operator, precedence, unary};
	operators[operator] = o;
}

/**
 * Usuwa operator z listy
 * 
 * Params:
 * string operator = operator do usunięcia
 */
public void unsetOperator(string operator)
{
	operators.remove(operator);
}

/**
 * Dodaje standardowe operatory
 */
public void resetOperators()
{
	clearOperators();
	setOperator("&", 3, false);
	setOperator("|", 3, false);
	setOperator("^", 3, false);
	setOperator(">", 2, false);
	setOperator("=", 1, false);
	setOperator("!", 5, true);
	setOperator("U", 4, false);
	setOperator("X", 5, true);
	setOperator("G", 5, true);
	setOperator("F", 5, true);
}

/**
 * Czyści listę operatorów
 */
public void clearOperators()
{
	operators.clear();
}

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
	return (c.isAlpha() && c.isLower()) || c.isNumber() || c == '_';
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
	foreach(string key; operators.keys)
	{
		if(s == key)
			return true;
	}
	
	return false;
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
	if(op1 == op2)
		return !operators[op1].unary;
	
	else
	{
		try
		{
			return operators[op1].precedence >= operators[op2].precedence;
		}
		catch(RangeError)  // Pewnie któryś jest nawiasem
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
	try
	{
		return operators[operator].unary ? 1 : 2;
	}
	catch(RangeError)
	{
		return 0;
	}
}

/**
 * Funkcja ustawia informacje o błędzie
 * 
 * Params:
 * errcode = kod błędu
 * token = token, przy którym wystąpił błąd
 * initialLength = początkowa długość wyrażenia
 * currentLength = aktualna długość wyrażenia
 */
void setError(const int errcode, char[] token, size_t initialLength, size_t currentLength)
{
	errorCode = errcode;
	errorToken = token.idup;
	errorPosition = cast(int) (initialLength - (token.length + currentLength));
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
	
	// Nawias
	else if(src[pos] == '(' || src[pos] == ')')
	{
		// Nic nie robimy, po prostu bierze jeden znak
	}
	
	// Operator
	else if(!src[pos].isWhite())
	{
		while(pos < src.length-1)
		{
			pos++;
			if(src[0..pos].isOperator() || src[pos].isVarChar() || src[pos].isWhite())
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
	size_t initialLength = cast(int) infix.length;  // do obliczania pozycji błędu
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
			// Zabroniony po operatorach jedno- i dwuargumentowych, nawiasie otwierającym i na początku
			if(argumentCount(previousType) > 0 || previousType == "(" || previousType == "")
			{
				setError(ERRCODE_UNEXPECTED_END, token, initialLength, infix.length);
				return false;
			}
			
			break;
		}
		
		// Zmienna
		else if(token[0].isVarChar())
		{
			/*
			 * Jeżeli pierwszy znak jest znakiem zmiennej, to pozostałe też - inaczej
			 * byłyby rozdzielone przez metodę lparse.
			 */
			 
			 // Zabroniona po nawiasie zamykającym i zmiennej
			 if(previousType == ")" || previousType == "var")
			 {
				setError(ERRCODE_UNEXPECTED_VAR, token, initialLength, infix.length);
				return false;
			 }
			 
			 previousType = "var".dup;
		}
		
		// Operator dwuargumentowy
		else if(argumentCount(token) == 2)
		{
			// Zabroniony po operatorze jedno- lub dwuargumentowym, nawiasie otwierającym i na początku
			if(argumentCount(previousType) > 0 || previousType == "(" || previousType == "")
			{
				setError(ERRCODE_UNEXPECTED_BIN_OPER, token, initialLength, infix.length);
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
				setError(ERRCODE_UNEXPECTED_UN_OPER, token, initialLength, infix.length);
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
				setError(ERRCODE_UNEXPECTED_LBRACK, token, initialLength, infix.length);
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
				setError(ERRCODE_SPARE_RBRACE, token, initialLength, infix.length);
				return false;
			}
			
			// Zabroniony po operatorze jedno- lub dwuargumentowym, nawiasie otwierającym i na początku
			if(argumentCount(previousType) > 0 || previousType == "(" || previousType == "")
			{
				setError(ERRCODE_UNEXPECTED_RBRACK, token, initialLength, infix.length);
				return false;
			}
			
			previousType = token;
		}
		
		// Nieznany token
		else
		{
			setError(ERRCODE_INVALID_TOKEN, token, initialLength, infix.length);
			return false;
		}
	}
	
	// Balans nawiasów
	if(bracketBalance > 0)  // Przypadek <0 rozpatrzony przy napotkanym nawiasie
	{
		setError(ERRCODE_BRACKET_UNCLOSED, token, initialLength, infix.length);
		return false;
	}
	
	else
	{
		errorCode = ERRCODE_OK;
		return true;
	}
}

// FUNKCJE WYWOŁYWANE Z C

/**
 * Ustawia operator
 * 
 * Params:
 * operator = ciąg znaków operatora
 * precedence = priorytet operatora
 * unary = jeden argument (jak false, to dwa). Operatory unarne stoją z przodu argumentu
 */
public void setOperator_C(char* operator, int precedence, bool unary)
{
	core.runtime.Runtime.initialize();
	
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
	core.runtime.Runtime.initialize();
	
	string dstr = toImpl!(string, char*)(operator);
	unsetOperator(dstr);
}

/**
 * Dodaje standardowe operatory
 */
extern(C) public void resetOperators_C()
{
	core.runtime.Runtime.initialize();
	
	resetOperators();
}

/**
 * Czyści listę operatorów
 */
extern(C) public void clearOperators_C()
{
	import core.runtime;
	Runtime.initialize();
	
	clearOperators();
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
	import core.runtime;
	Runtime.initialize();
	
	char[] dstr = toImpl!(string, char*)(infix);
	return validate(dstr);
}