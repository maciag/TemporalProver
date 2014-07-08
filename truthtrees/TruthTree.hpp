#include "../include/tree.hh"
#include "../functions/prefix.h"
#include <string>
#include <iostream>
#include "FormulaNode.hpp"

using namespace std;

class TruthTree {

	tree<FormulaNode> mainTree;
	tree<FormulaNode>::iterator root;
	tree<FormulaNode>::iterator currentNode;

	tree<FormulaNode>::iterator findFormula(StringFormula formula);
	bool existsPath(tree<FormulaNode>::iterator it1, tree<FormulaNode>::iterator it2);

public:

	TruthTree(string formula);
	TruthTree(char *formula);
	TruthTree(StringFormula formula);
	TruthTree(FormulaNode formulaNode);

	void decomposeAll();
	bool decomposeStep();
	void eliminateNodes();
	bool getResult();

	string toFormattedString();

	friend ostream& operator<<(ostream& os, const TruthTree& truthTree);

};
