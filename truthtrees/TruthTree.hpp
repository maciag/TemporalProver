#include "../include/tree.hh"
#include "../functions/prefix.h"
#include <string>
#include <iostream>
#include "FormulaNode.hpp"
#include <string.h>

using namespace std;

class TruthTree {

	tree<FormulaNode> mainTree;
	tree<FormulaNode>::iterator root;
	tree<FormulaNode>::iterator currentNode;

	tree<FormulaNode>::iterator findFormula(StringFormula formula);
	bool existsPath(tree<FormulaNode>::iterator it1, tree<FormulaNode>::iterator it2);
	string nodeToFormattedString(tree<FormulaNode>::iterator it);
	static void initParserOperators();

public:

	TruthTree(string formula);
	TruthTree(char *formula);
	TruthTree(StringFormula formula);
	TruthTree(FormulaNode formulaNode);

	void decomposeAll();
	bool decomposeStep();
	void eliminateNodes();
	bool getResult();

	static bool changeOperator(token op, string symbol);
	static void resetOperators();

	string toFormattedString();

	friend ostream& operator<<(ostream& os, const TruthTree& truthTree);

};
