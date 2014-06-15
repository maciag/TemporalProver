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
		branching = 0, stacking, branching_stacking, single, none, error
	};

	StringFormula(string content);
	StringFormula(vector<string> symbolTable);
	~StringFormula();

	string getRaw();
	vector<token> getTokenArray();
	vector<string> getSymbolArray();
	bool decompose(vector<StringFormula> &elements);
	decomposeType getType();

	friend ostream& operator<<(ostream& os, const StringFormula& stringFormula);

private:
	static map<string, token> tokenMap;

	string rawContent;
	string inflixContent;
	vector<token> tokenArray;
	vector<string> symbolArray;
	decomposeType type;

	token parseSymbol(string symbol);
	vector<string> splitString(const string& s, const string& delim,
			const bool keep_empty = true);
	void evalDecompositionType();
	StringFormula subFormula(int begin, int end);
	token getToken(int idx);
	string getSymbol(int idx);
	void negate();
	//TODO Wyznaczanie klasycznej postaci infiksowej (rawContent) na podstawie zapisu w notacji polskiej

};

#endif /* STRINGFORMULA_HPP_ */
