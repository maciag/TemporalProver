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

	vector<string> subformulas = StringFormula::splitString(line, ";");
	FormulaNode root;

	for (int i = 0; i < subformulas.size(); i++) {
		try {
			StringFormula *form; // UWAGA, WSKAŹNIK!!! Musiałem, żeby przechwycić wyjątek i wypisać błąd.
			form = new StringFormula(subformulas[i]);
			root.appendFormula(*form);
			delete form;
		} catch (runtime_error &) {
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
				cout << "Nieoczekiwany operator binarny '" << token << "'"
						<< endl;

			else if (errCode == ERRCODE_UNEXPECTED_UN_OPER)
				cout << "Nieoczekiwany operator unarny '" << token << "'"
						<< endl;

			else if (errCode == ERRCODE_UNEXPECTED_LBRACK)
				cout << "Nieoczekiwany nawias '('" << endl;

			else if (errCode == ERRCODE_UNEXPECTED_RBRACK)
				cout << "Nieoczekiwany nawias ')'" << endl;

			else if (errCode == ERRCODE_INVALID_TOKEN)
				cout << "Nieznany token: '" << token << "'" << endl;

			else if (errCode == ERRCODE_BRACKET_UNCLOSED)
				cout << "Brakuje nawiasu zamykającego" << endl;

			else if (errCode == ERRCODE_SPARE_RBRACK)
				cout << "Nieoczekiwany nawias zamykający" << endl;

			return 0;
		}
	}

	TruthTree truthTree(root);
	int idx = 0;
	do {
		cout << idx << endl;
		cout << truthTree;
		cout << endl;
		idx++;
	} while (truthTree.decomposeStep());

	idx++;
	cout << idx << endl;
	cout << truthTree;
	cout << endl;

	idx++;
	truthTree.eliminateNodes();
	cout << idx << endl;
	cout << truthTree;
	cout << endl;

	/*for (int i = 0; i < 6; i++) {
	 cout << i << endl;
	 cout << truthTree;
	 cout << endl;
	 truthTree.decomposeStep();
	 }*/

	string stringForm = truthTree.toFormattedString();

	cout << endl << "W postaci sformatowanego stringa" << endl << stringForm
			<< endl;

	if (truthTree.getResult())
		cout << "Formuła wejściowa spełnialna" << endl;
	else
		cout << "Formuła wejściowa niespełnialna" << endl;

	return 0;
}
