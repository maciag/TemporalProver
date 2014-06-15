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

	StringFormula *form; // UWAGA, WSKAŹNIK!!! Musiałem, żeby przechwycić wyjątek i wypisać błąd.

	try {
		form = new StringFormula(line);
	} catch (runtime_error) {
		int errCode = getErrorCode_C();
		string token = getErrorToken_C();
		int errPos = getErrorPosition_C();

		cout << errPos << ": ";

		// Switch nie może być, bo wartości nie są wyrażeniem stałym
		if (errCode == ERRCODE_UNEXPECTED_END)
			cout << "Nieoczekiwany koniec formuły" << endl;

		else if (errCode == ERRCODE_UNEXPECTED_VAR)
			cout << "Nieoczekiwana zmienna '" << token << "'" << endl;

		else if (errCode == ERRCODE_UNEXPECTED_BIN_OPER)
			cout << "Nieoczekiwany operator binarny '" << token << "'" << endl;

		else if (errCode == ERRCODE_UNEXPECTED_UN_OPER)
			cout << "Nieoczekiwany operator unarny '" << token << "'" << endl;

		else if (errCode == ERRCODE_UNEXPECTED_LBRACK)
			cout << "Nieoczekiwany nawias '('" << endl;

		else if (errCode == ERRCODE_UNEXPECTED_RBRACK)
			cout << "Nieoczekiwany nawias ')'" << endl;

		else if (errCode == ERRCODE_INVALID_TOKEN)
			cout << "Nieznany token: '" << token << "'" << endl;

		else if (errCode == ERRCODE_BRACKET_UNCLOSED)
			cout << "Brakuje nawiasu zamykającego" << endl;

		else if (errCode == ERRCODE_SPARE_RBRACE)
			cout << "Nieoczekiwany nawias zamykający" << endl;

		return 0;
	}
	StringFormula formula = *form;

	TruthTree truthTree(formula);
	int idx = 0;
	do {
		cout << idx << endl;
		cout << truthTree;
		cout << endl;
		idx++;
	} while (truthTree.decomposeStep());

	return 0;
}
