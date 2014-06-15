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

public:

	TruthTree(string formula);
	TruthTree(char *formula);
	TruthTree(StringFormula formula);
	TruthTree(FormulaNode formulaNode);

	void decomposeAll();
	bool decomposeStep();

	friend ostream& operator<<(ostream& os, const TruthTree& truthTree);

};
