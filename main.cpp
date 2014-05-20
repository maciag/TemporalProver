#include "TruthTree.hpp"
#include "StringFormula.hpp"
#include <iostream>
#include <string>
#include <map>
using namespace std;

int main() {

	cout << "Podaj formułę:" << endl;

	string line;
	getline(cin, line);

	StringFormula formula(line);
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
