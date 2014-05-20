#include <map>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>

#include "functions/prefix.h"
#include "LexicalTable.hpp"

using namespace std;

class StringFormula
{

	static map<string, token> tokenMap;

	string rawContent;
	string inflixContent;
	vector<token> tokenArray;
	vector<string> symbolArray;

	token parseSymbol(string symbol);
	vector<string> splitString(const string& s, const string& delim, const bool keep_empty = true);
	//TODO Wyznaczanie klasycznej postaci infiksowej (rawContent) na podstawie zapisu w notacji polskiej

public:

	enum decomposeType {
		branching = 0,
		stacking,
		branching_stacking,
		single
	};

	StringFormula(string content);
	StringFormula(vector<string> symbolTable);
	~StringFormula();
	token getToken(int idx);
	string getSymbol(int idx);
	string getRaw();
	vector<token> getTokenArray();
	vector<string> getSymbolArray();
	bool isBranching();
	bool isAtomic();
	void negate();
	decomposeType decompose(vector<StringFormula> &elements);
	StringFormula subFormula(int begin, int end);

	friend ostream& operator<<(ostream& os, const StringFormula& stringFormula);

};
