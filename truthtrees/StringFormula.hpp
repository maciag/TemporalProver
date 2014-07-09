#ifndef STRINGFORMULA_HPP_
#define STRINGFORMULA_HPP_

#include <map>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>

#include "../functions/prefix.h"
#include "LexicalTable.hpp"

using namespace std;

class StringFormula {

public:

	enum decomposeType {
		branching = 0, stacking, branching_stacking, branching_substacking, single, next, none, error
	};

	StringFormula(string content);
	StringFormula(vector<string> symbolTable);
	StringFormula();
	~StringFormula();

	string getRaw();
	vector<token> getTokenArray();
	vector<string> getSymbolArray();
	bool decompose(vector<StringFormula> &elements);
	decomposeType getType();
	bool isPreposition();
	bool isPrepositionNegation(StringFormula other);
	StringFormula nextFormula();
	bool isEmpty();
	StringFormula needSatisfaction();

	friend ostream& operator<<(ostream& os, const StringFormula& stringFormula);
	bool operator==(const StringFormula &other) const;
	bool operator!=(const StringFormula &other) const;

	static vector<string> splitString(const string& s, const string& delim,
				const bool keep_empty = true);

private:

	string rawContent;
	string inflixContent;
	vector<token> tokenArray;
	vector<string> symbolArray;
	decomposeType type;

	token parseSymbol(string symbol);
	void evalDecompositionType();
	StringFormula subFormula(int begin, int end);
	token getToken(int idx);
	string getSymbol(int idx);
	void pushOperatorFront(token op);
	void negate();


	static StringFormula compose(StringFormula left, token op, StringFormula right);

	//TODO Wyznaczanie klasycznej postaci infiksowej (rawContent) na podstawie zapisu w notacji polskiej

};

#endif /* STRINGFORMULA_HPP_ */
