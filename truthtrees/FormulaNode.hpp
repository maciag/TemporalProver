#ifndef FORMULANODE_HPP_
#define FORMULANODE_HPP_

#include "StringFormula.hpp"

class FormulaNode {

	vector<StringFormula> formulas;
	vector<bool> isChecked;
	bool allChecked;

public:
	FormulaNode(vector<StringFormula> formulas);
	FormulaNode(vector<string> rawFormulas);
	FormulaNode();
	~FormulaNode();
	void appendFormula(string rawFormula);
	void appendFormula(StringFormula formula);
	bool isAllChecked();

	StringFormula::decomposeType decomposeNext(vector<FormulaNode> &childNodes);
	friend ostream& operator<<(ostream& os, const FormulaNode& formulaNode);
};

#endif /* FORMULANODE_HPP_ */
