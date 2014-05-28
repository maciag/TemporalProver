#include "truthtrees/TruthTree.hpp"
#include "truthtrees/StringFormula.hpp"
#include "functions/prefix.h"
#include <iostream>
#include <string>
#include <map>
using namespace std;

int main() {
	
	// Ustawiam domyślny zestaw operatorów
	resetOperators_C();
	
	cout << "Podaj formułę:" << endl;
	
	string line;
	getline(cin, line);
	
	StringFormula *form;  // UWAGA, WSKAŹNIK!!! Musiałem, żeby przechwycić wyjątek i wypisać błąd.
	
	try
	{
		form = new StringFormula(line);
	}
	catch(runtime_error)
	{
		int errCode = getErrorCode_C();
		string token = getErrorToken_C();
		int errPos = getErrorPosition_C();
		
		cout << errPos << ": ";
		
		// Switch nie może być, bo wartości nie są wyrażeniem stałym
		if(errCode == ERRCODE_UNEXPECTED_END)
			cout << "Nieoczekiwany koniec formuły" << endl;
		
		else if(errCode == ERRCODE_UNEXPECTED_VAR)
			cout << "Nieoczekiwana zmienna '" << token << "'" << endl;
		
		else if(errCode == ERRCODE_UNEXPECTED_BIN_OPER)
			cout << "Nieoczekiwany operator binarny '" << token << "'" << endl;
		
		else if(errCode == ERRCODE_UNEXPECTED_UN_OPER)
			cout << "Nieoczekiwany operator unarny '" << token << "'" << endl;
		
		else if(errCode == ERRCODE_UNEXPECTED_LBRACK)
			cout << "Nieoczekiwany nawias '('" << endl;
		
		else if(errCode == ERRCODE_UNEXPECTED_RBRACK)
			cout << "Nieoczekiwany nawias ')'" << endl;
		
		else if(errCode == ERRCODE_INVALID_TOKEN)
			cout << "Nieznany token: '" << token << "'" << endl;
		
		else if(errCode == ERRCODE_BRACKET_UNCLOSED)
			cout << "Brakuje nawiasu zamykającego" << endl;
		
		else if(errCode == ERRCODE_SPARE_RBRACE)
			cout << "Nieoczekiwany nawias zamykający" << endl;
		
		
		return 0;
	}
	StringFormula formula = *form;
	cout << formula << endl;
	vector<StringFormula> elements;
	StringFormula::decomposeType type = formula.decompose(elements);
	
	cout << "Składowe elementy:" << endl;
	
	for (int i = 0; i < elements.size(); i++)
		cout << elements[i] << endl;
	
	switch (type) {
		case StringFormula::decomposeType::branching:
			cout << "Dekompozycja rozgałęziająca" << endl;
			break;
			
		case StringFormula::decomposeType::stacking:
			cout << "Dekompozycja odkładająca na stos" << endl;
			break;
			
		case StringFormula::decomposeType::branching_stacking:
			cout << "Dekompozycja rozgałęziająca i odkładająca na stos" << endl;
			break;
			
		case StringFormula::decomposeType::single:
			cout << "Dekompozycja odkładająca na stos jeden element" << endl;
			break;
			
	}
	
	return 0;
}
